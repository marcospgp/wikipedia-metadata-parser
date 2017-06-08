package engine;

import java.util.*;
import static java.util.stream.Collectors.toCollection;;

public class Articles {

    private static long allArticles = 0, uniqueArticles = 0, allRevisions = 0;
    private static int articleWasFound = -1, articleWasUpdated = -1;
    /* este int serve para a função da hash poder dizer se encontrou ou não o artigo,
     * deste modo é possível dar update dos contadores dos articles
     * --------- só está inicializado para não dar warning
   	 */

    public static void setArticleWasFound(int was) {
    	articleWasFound = was;
    }

    public static void setArticleWasUpdated(int was) {
    	articleWasUpdated = was;
    }

    /**
     * @brief Conta o número de palavras e o tamanho do texto.
     *
     * Utiliza flags para contar palavras e ignorar espaços entre elas e carateres especiais.
     * Atualiza o valor de sizeBytes para não se perder tempo com strlen, ou seja, é 2 em 1.
     *
     * @param revisionText O texto a analisar, correspondente ao conteúdo do artigo.
     * @param sizeBytes Apontador a atualizar para dar o tamanho do artigo.
     * @return O número de palavras do artigo.
     */
    public static long wordCounter(String revisionText, long sizeBytes) {
        String str = revisionText;
        long size = 0L;
        long count = 0L;
        int foundLetter = 0;

        if (str != null) {
            for (char ch : str.toCharArray()){
                if (ch == ' ' || ch == '\n' || ch == '\t')
                    foundLetter = 0;
                else {
                    if (foundLetter == 0)
                    count++;
                    foundLetter = 1;
                }
                size++;
            }
        }

        sizeBytes = size;

        return count;
    }

    /**
     * @brief Verifica que aquele título tem aquele prefixo.
     *
     * Compara char a charm, tendo em conta também o tamanho, com vista à determinação pedida.
     *
     * @param prefix O prefixo em conta.
     * @param title O título a comparar.
     * @return O valor 1 caso seja e 0 caso não seja prefixo.
     */
    public static int isTitlePrefix(String prefix, String title) {

        String prefixo = prefix;
        String string = title;
        int i;

        if (prefixo.length() > string.length()) return 0;

        if (prefixo.equals(string.substring(0, prefixo.length()))) return 1;
        else return 0;

    }


    public static void onPageArticles(
        HashMap<Long, Article> articles,
        long articleId,
        String title,
        String revisionText,
        long revisionId,
        long revisionParentId,
        String revisionTimestamp
    ) {

        // nWords é atualizado pelo return do wordCounter que também dá o número de bytes do artigo pelo apontador
        long sizeBytes = 0L;
        long nWords = wordCounter(revisionText, sizeBytes);


        /* Esta função deve ser chamada quando é encontrada uma revisão de um artigo.
         * A função cria ou atualiza um artigo já existente na tabela. Caso seja feita uma
         * atualização, o tamanho do artigo e número de palavras só é atualizado se for
         * maior que o anterior. Os valores restantes são sempre atualizados.
         */
        Hashtable.insertOrUpdateArticle(articles, articleId, title, revisionId, revisionParentId, revisionTimestamp, sizeBytes, nWords, articleWasFound, articleWasUpdated);

        // Aumenta o allArticles sempre
        allArticles++;

        // Aumenta o caso de ser a primeira vez que o artigo entra
        if (articleWasFound == 0) {
            uniqueArticles++;
        }
        // Para o caso de ser revisão
        if(articleWasUpdated == 1) {
            allRevisions++;
        }
    }


    /**
     * @brief Função que obtém o número total de artigos analisados no decurso da execução.
     *
     * Retorna o seu campo allArticles de uma hashtable de artigos.
     *
     * @return O número de artigos analisados.
     */
    public static long get_all_articles() {

        return allArticles;
    }


    /**
     * @brief Função que obtém o número de artigos únicos existentes no decurso da execução.
     *
     * Retorna o seu campo uniqueArticles.
     *
     * @return O número de artigos únicos.
     */
    public static long get_unique_articles() {

        return uniqueArticles;
    }


    /**
     * @brief Função que obtém o número de revisões feitas no decurso da execução.
     *
     * Retorna o seu campo allRevisions.
     *
     * @return O número de revisões.
     */
    public static long get_all_revisions() {

        return allRevisions;
    }


    /**
     * @brief Função que obtém o título de um artigo através do seu ID.
     *
     * Verifica se aquele artigo existe e, caso exista, retorna o seu campo título.
     *
     * @see getArticle()
     *
     * @param article_id O ID do artigo.
     * @param qs A estrutura geral do programa.
     * @return O título do artigo ou @c NULL.
     */
    public static String get_article_title(long article_id, HashMap<Long, Article> articles) {
        Article ourArticle = articles.get(article_id);

        if (ourArticle != null) {
            return ourArticle.getArticleTitle();
        } else {
            return null;
        }
    }


    /**
     * @brief Função que obtém o timestamp de um artigo através do seu ID de artigo e de revisão.
     *
     * Verifica se aquele artigo existe e, caso exista, retorna o seu campo timestamp.
     *
     * @see getArticle()
     * @see getRevision()
     *
     * @param article_id O ID do artigo.
     * @param revision_id o ID da revisão.
     * @param qs A estrutura geral do programa.
     * @return O timestamp do artigo ou @c NULL.
     */
    public static String get_article_timestamp(long article_id, long revision_id, HashMap<Long, Article> articles) {
        Article ourArticle = articles.get(article_id);

        if (ourArticle != null) {
            Revision ourRevision = articles.get(article_id).getRevisionsHash().get(revision_id);
            if (ourRevision != null) {
                return ourRevision.getTimestamp();
            }
        }

        return null;
    }


    /**
     * @brief Função que obtém o top dos 20 maiores artigos.
     *
     * Cria um iterador da hashtable correspondente aos artigos.
     * Cria e inicializa um array de 20 artigos.
     * Itera pela hashtable e compara o tamanho do artigo a ser iterado
     * com o top, começando pelo último lugar deste.
     * Caso seja maior ou igual que o último, continua a fazer comparações com os acima.
     * Caso o tamanho dos artigos forem iguais, é feita a decisão pelo ID destes.
     * Caso entre no top, é feito o reajuste dos lugares abaixo.
     *
     * @see getHashtableIterator()
     * @see getNextFromIterator()
     *
     * @param qs A estrutura geral do programa.
     * @return Um array com os IDs dos 20 maiores artigos.
     */

    public static ArrayList<Long> getTop_20_largest_articles(HashMap<Long, Article> articles) {

        ArrayList<Article> top20sorted = new ArrayList<Article>();
        top20sorted = articles.values().stream()
            .sorted(Comparator.comparing(Article::getArticleSize))
            .limit(20)
            .collect(toCollection(ArrayList::new));

        ArrayList<Long> top20 = new ArrayList<Long>();

        for(Article u: top20sorted) {
            top20.add(u.getArticleId());
        }

        return top20;
    }

}
