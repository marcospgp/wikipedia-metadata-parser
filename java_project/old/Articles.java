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
	public static long wordCounter(String revisionText, long sizeBytes) {
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

		String prefixo = prefix.clone();
		String string = title.clone();
		int i;

		for (i = 0; (prefixo[i] != '\0'); i++) {
			if (string[i] != '\0') {
				if (prefixo[i] == string[i]) continue;
				else {
					//printf("0\n");
					return 0;
				}
			}
			// caso o prefix seja maior que o titulo
			else {
				//printf("0\n");
				return 0;
			}
		}

		//printf("1\n");
		return 1;

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
	public static String get_article_title(long article_id, HashMap<Article> articles) {
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
	public static String get_article_timestamp(long article_id, long revision_id, HashMap<Article> articles) {
		Article ourArticle = articles.get(article_id);

		if (ourArticle != null) {
			Revision ourRevision = articles.getRevisionsHash().get(revision_id);
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
	String[] getTop20LargestArticles(TAD_istruct qs) {

		/* Ideia para iterar
		Map<Integer, Integer> map = new HashMap<Integer, Integer>();
			for (Map.Entry<Integer, Integer> entry : map.entrySet()) {
			    System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());
			}
		*/

		/*
		// Iterating over values only
		for (Integer value : map.values()) {
		    System.out.println("Value = " + value);
		}
		*/

		// Getting top 20 largest articles.

		void *iterator = getHashtableIterator(qs->articles);

		Article[] top20 = new Article[20];


		long key = null;
		Article curArticle = null;

		int index;

		// Iterating through users hash table.

		// Iterar pela hash table de artigos.
		while (getNextFromIterator(iterator, &key, &curArticle)) {

			index = 19; // Rank onde o utilizador vai ser colocado

			if (curArticle->size >= (top20[19])->size) {

				// A article's size >= top20[19]'s size.

				// Diminuir o indíce enquanto o artigo encaixar num rank superior.
				while ((index >= 1) && (curArticle->size >= (top20[index - 1])->size)) {
					index--;
				}

				// This article will be compared to index.

				// Neste ponto sabemos que o score deste article >= top[index].

				if (curArticle->size == (top20[index])->size) {

					// The users size are equal.

					// Score deste article == top[index].
					// Comparar article id's.

					if (curArticle->id > (top20[index])->id) {

						// The article id lost.

						index++; // Colocar este artigo uma posição abaixo no rank.

						if (index > 19) {

							// User dropped to 21st place.

							continue; // Utilizador desceu para 21º, skipar.
						}

					} else {
						fprintf(stderr, "Erro ao comparar id dos artigos %ld e %ld\n", curArticle->id, (top20[index])->id);
					}

					// The article id won.
				}

				// Deslizar os outros artigos para baixo para termos espaço para o novo artigo no rank.

				int i;
				for (i = 19; i > index; i--) {

					top20[i] = top20[i - 1];
				}

				// Colocar o utlizador na sua nova posição no rank.
				top20[index] = curArticle;
			}
		}

		// Importante - libertar a memória do iterador no final!
		freeIterator(iterator);

		// Obter array de ids e retorná-lo.

		long *top20Ids = malloc(20 * sizeof(long));

		int j;
		for (j = 0; j < 20; j++) {
			top20Ids[j] = (top20[j])->id;
		}

		// Returning top20Ids.

		return top20Ids;
	}


}