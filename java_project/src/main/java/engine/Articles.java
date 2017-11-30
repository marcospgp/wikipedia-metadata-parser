package engine;

import java.util.*;
import static java.util.stream.Collectors.toCollection;;

public class Articles {

	private static long allArticles = 0L, uniqueArticles = 0L, allRevisions = 0L, sizeBytes = 0L;
	/* este int serve para a função da hash poder dizer se encontrou ou não o artigo,
	 * deste modo é possível dar update dos contadores dos articles
	 * --------- só está inicializado para não dar warning
	 */

	public static void setSizeBytes(long size) {
		sizeBytes = size;
	}

	public static long getSizeBytes() {
		return sizeBytes;
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
	public static long wordCounter(String revisionText) {
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

		setSizeBytes(size);

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

		if (title.startsWith(prefix)) return 1;
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

		sizeBytes = 0L; // inicializar size

		// nWords é atualizado pelo return do wordCounter que também dá o número de bytes do artigo pelo apontador
		long nWords = wordCounter(revisionText);

		// DEBUG - Estão a entrar bem os elementos
		/*
		System.out.println("Article ID: " + articleId);
		System.out.println("title: " + title);
		System.out.println("revisionId: " + revisionId);
		System.out.println("revisionParentId: " + revisionParentId);
		System.out.println("revisionTimestamp: " + revisionTimestamp +"\n");
		*/

		/* Esta função deve ser chamada quando é encontrada uma revisão de um artigo.
		 * A função cria ou atualiza um artigo já existente na tabela. Caso seja feita uma
		 * atualização, o tamanho do artigo e número de palavras só é atualizado se for
		 * maior que o anterior. Os valores restantes são sempre atualizados.
		 */

		int result = Hashtable.insertOrUpdateArticle(articles, articleId, title, revisionId, revisionParentId, revisionTimestamp, getSizeBytes(), nWords);

		// Aumenta o allArticles sempre
		allArticles++;

		// Result: 0 se foi inserido, 1 se foi atualizado, 2 se é duplicado
		if (result == 0) {

			uniqueArticles++;
			allRevisions++;

		} else if (result == 1) {

			allRevisions++;

		} else if (result == 2) {

			// Nothing to do here
		}

		return;
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
	 * FALTA EXPLICAR A FUNCAO
	 *
	 * @see getHashtableIterator()
	 * @see getNextFromIterator()
	 *
	 * @param qs A estrutura geral do programa.
	 * @return Um array com os IDs dos 20 maiores artigos.
	 */

	public static ArrayList<Long> getTop_20_largest_articles(HashMap<Long, Article> articles) {

		ArrayList<Article> top20sorted = new ArrayList<Article>();

		Comparator<Article> comparator = Comparator.comparingLong(Article::getArticleSize);
		Comparator<Article> reverseComparator = comparator.reversed();

		top20sorted = articles.values().stream()
			.sorted(reverseComparator)
			.limit(20)
			.collect(toCollection(ArrayList::new));

		ArrayList<Long> top20 = new ArrayList<Long>();

		for(Article a: top20sorted) {
			top20.add(a.getArticleId());
		}

		return top20;
	}


	/**
	 * @brief Função que obtém o top N dos artigos com mais palavras.
	 *
	 * FALTA EXPLICAR A FUNCAO
	 *
	 * @see getHashtableIterator()
	 * @see getNextFromIterator()
	 *
	 * @param qs A estrutura geral do programa.
	 * @return Um array com os IDs dos 20 maiores artigos.
	 */

	public static ArrayList<Long> getTop_N_largest_articles(HashMap<Long, Article> articles, int n) {

		ArrayList<Article> topnsorted = new ArrayList<Article>();

		Comparator<Article> comparator = Comparator.comparingLong(Article::getArticleNWords);
		Comparator<Article> reverseComparator = comparator.reversed();

		topnsorted = articles.values().stream()
			.sorted(reverseComparator)
			.limit(n)
			.collect(toCollection(ArrayList::new));

		ArrayList<Long> topn = new ArrayList<Long>();

		for(Article a: topnsorted) {
			topn.add(a.getArticleId());
		}

		return topn;
	}

	public static ArrayList<String> getTitlesWithPrefix(HashMap<Long, Article> articles, String prefix) {

		ArrayList<Article> articletitles = new ArrayList<Article>();

		articletitles = articles.values().stream()
		.filter(a -> isTitlePrefix(prefix,a.getArticleTitle()) == 1)
		.collect(toCollection(ArrayList::new));

		ArrayList<String> titles = new ArrayList<String>();

		for(Article a: articletitles) {
			titles.add(a.getArticleTitle());
		}

		Collections.sort(titles);

		return titles;
	}

}
