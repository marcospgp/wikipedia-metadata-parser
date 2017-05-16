public class Articles {

	private long allArticles = 0, uniqueArticles = 0, allRevisions = 0;


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
	long wordCounter(String revisionText, long sizeBytes) {
		String str = revisionText.clone();
		long i = 0;
		long count = 0;
		int foundLetter = 0;

		if (str) {
			for (i = 0; str[i] != '\0'; i++) {
				if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
					foundLetter = 0;
				else {
					if (foundLetter == 0)
	    	    	count++;
	    			foundLetter = 1;
				}
			}
		}

		sizeBytes = i;

		return count;
	}


	public static void onPageArticles(
		HashMap<Article> articles,
		long articleId,
	    String title,
	    String revisionText,
	    long revisionId,
	    long revisionParentId,
	    String revisionTimestamp
	) {

		// nWords é atualizado pelo return do wordCounter que também dá o número de bytes do artigo pelo apontador
		long sizeBytes;
		long nWords = wordCounter(revisionText, sizeBytes);

		/* este int serve para a função da hash poder dizer se encontrou ou não o artigo,
		 * deste modo é possível dar update dos contadores dos articles
		 * --------- só está inicializado para não dar warning
		 */
		int articleWasFound;
		int articleWasUpdated;

		/* Esta função deve ser chamada quando é encontrada uma revisão de um artigo.
		 * A função cria ou atualiza um artigo já existente na tabela. Caso seja feita uma
		 * atualização, o tamanho do artigo e número de palavras só é atualizado se for
		 * maior que o anterior. Os valores restantes são sempre atualizados.
		 */
		Hastable.insertOrUpdateArticle(articles, articleId, title, revisionId, revisionTimestamp, sizeBytes, nWords, articleWasFound, articleWasUpdated);

		// Aumenta o allArticles sempre
		allArticles++;

		// Aumenta o caso de ser a primeira vez que o artigo entra
		if (!(articleWasFound)) {
			uniqueArticles++;
		}
		// Para o caso de ser revisão
		if(articleWasUpdated) {
			allRevisions++;
		}
	}

	/**
	 * @brief Função que obtém o número total de artigos analisados no decurso da execução.
	 *
	 * Retorna o seu campo allArticles.
	 *
	 * @param qs A estrutura geral do programa.
	 * @return O número de artigos analisados.
	 */
	long get_all_articles(TAD_istruct qs) {
		struct TCD_istruct *ourTable = qs;

		return ourTable->allArticles;
	}
}