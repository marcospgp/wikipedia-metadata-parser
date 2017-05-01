#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#include "articles.h"


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
long wordCounter(char *revisionText, long *sizeBytes) {
	char *str = revisionText;
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

	*sizeBytes = i;

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
int isTitlePrefix(char* prefix, char* title) {

	char* prefixo = prefix;
	char* string = title;
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

/**
 * @brief Função que trata os dados relativos a artigos, recebidos do parsing
 * do XML, e os envia para a hashtable.
 *
 * Obtém informação sobre número de palavras e sobre tamanho do artigo recebido através
 * da função wordCounter e chama as funções de update da hashtable, com as informações que obteve.
 * Dependendo de ter atualizado ou não determinados campos da hashtable,
 * atualiza os diversos contadores de articles.
 *
 * @see wordCounter()
 * @see insertOrUpdateArticle()
 *
 * @param qs A estrutura geral do programa.
 * @param articleId A ID do artigo.
 * @param title O título do artigo.
 * @param revisionText O conteúdo do artigo.
 * @param revisionId O ID da revisão.
 * @param revisionParentId O ID da revisão pai.
 * @param revisionTimestamp O timestamp da revisão.
 * @return A estrutura geral atualizada.
 */

TAD_istruct onPageArticles(
    TAD_istruct qs,
    long articleId,
    char *title,
    char *revisionText,
    long revisionId,
    long revisionParentId,
    char *revisionTimestamp
) {


// nWords é atualizado pelo return do wordCounter que também dá o número de bytes do artigo pelo apontador
	long sizeBytes;
	long nWords = wordCounter(revisionText, &sizeBytes);


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
	qs = insertOrUpdateArticle(qs, articleId, title, revisionId, revisionTimestamp, sizeBytes, nWords, &articleWasFound, &articleWasUpdated);


// Aumenta o allArticles sempre
	qs->allArticles++;

	// Aumenta o caso de ser a primeira vez que o artigo entra
	if (!(articleWasFound)) {
		qs->uniqueArticles++;
	}
	// Para o caso de ser revisão
	if(articleWasUpdated) {
		qs->allRevisions++;
	}

	return qs;
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


/**
 * @brief Função que obtém o número de artigos únicos existentes no decurso da execução.
 *
 * Retorna o seu campo uniqueArticles.
 *
 * @param qs A estrutura geral do programa.
 * @return O número de artigos únicos.
 */
long get_unique_articles(TAD_istruct qs) {
	struct TCD_istruct *ourTable = qs;

	return ourTable->uniqueArticles;
}


/**
 * @brief Função que obtém o número de revisões feitas no decurso da execução.
 *
 * Retorna o seu campo allRevisions.
 *
 * @param qs A estrutura geral do programa.
 * @return O número de revisões.
 */
long get_all_revisions(TAD_istruct qs) {
	struct TCD_istruct *ourTable = qs;

	return ourTable->allRevisions;
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
char* get_article_title(long article_id, TAD_istruct qs) {
	struct article *ourArticle = getArticle(qs, article_id);

	if (ourArticle) {
		return ourArticle->title;
	} else {
		return NULL;
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
char* get_article_timestamp(long article_id, long revision_id, TAD_istruct qs) {
	struct article *ourArticle = getArticle(qs, article_id);

	if (ourArticle) {
		struct revision *ourRevision = getRevision(ourArticle->revisions, revision_id);
		if (ourRevision) {
			return ourRevision->timestamp;
		}
	}

	return NULL;
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
long* getTop20LargestArticles(TAD_istruct qs) {

	// Getting top 20 largest articles.

	void *iterator = getHashtableIterator(qs->articles);

	struct article *top20[20];

	// Inicializar array com shitty articles para começar.

	struct article nothing = {(long) 1, (long) 1, (long) 1, "", NULL};

	int i;
	for (i = 0; i < 20; i++) {
		top20[i] = &nothing;
	}

	void *key = NULL;
	struct article *curArticle = NULL;

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


/**
 * @brief Função que obtém o top dos N artigos com mais palavras.
 *
 * Cria um iterador da hashtable correspondente aos artigos.
 * Cria e inicializa um array de N artigos.
 * Itera pela hashtable e compara o número de palavras do artigo a ser iterado
 * com o top, começando pelo último lugar deste.
 * Caso seja maior ou igual que o último, continua a fazer comparações com os acima.
 * Caso o número de palavras dos artigos forem iguais, é feita a decisão pelo ID destes.
 * Caso entre no top, é feito o reajuste dos lugares abaixo.
 *
 * @see getHashtableIterator()
 * @see getNextFromIterator()
 *
 * @param n O tamanho do top.
 * @param qs A estrutura geral do programa.
 * @return Um array com os IDs dos N artigos com mais palavras.
 */
long* getTopNArticlesWithMoreWords(int n, TAD_istruct qs) {

	// Getting top n articles with more words.

	void *iterator = getHashtableIterator(qs->articles);

	struct article *topN[n];

	// Inicializar array com stupid articles para começar.

	struct article nothing = {(long) 0, (long) 0, (long) 0, "", NULL};

	int i;
	for (i = 0; i < n; i++) {
		topN[i] = &nothing;
	}

	void *key = NULL;
	struct article *curArticle = NULL;

	int index;

	// Iterating through users hash table.

	// Iterar pela hash table de artigos.
	while (getNextFromIterator(iterator, &key, &curArticle)) {

		index = n-1; // Rank onde o utilizador vai ser colocado.

		if (curArticle->nWords >= (topN[n-1])->nWords) {

			// A article's nWords >= topN[n-1]'s nWords.

			// Diminuir o indíce enquanto o utilizador encaixar num rank superior.
			while ((index >= 1) && curArticle->nWords >= (topN[index - 1])->nWords) {
				index--;
			}

			// This article will be compared to index.

			// Neste ponto sabemos que o score deste article >= top[index].

			if (curArticle->nWords == (topN[index])->nWords) {

				// The users nWords are equal.

				// Score deste article == top[index].
				// Comparar usernames alfabeticamente.

				if (curArticle->id > (topN[index])->id) {

					// The article id lost.

					index++; // Colocar este artigo uma posição abaixo no rank.

					if (index > n-1) {

						// User dropped to (n+1)st place.
						continue; // Utilizador desceu para (n+1)º, skipar.
					}

				} else {
					fprintf(stderr, "Erro ao comparar id dos artigos %ld e %ld\n", curArticle->id, (topN[index])->id);
				}

				// The article id won.
			}

			// Deslizar os outros artigos para baixo para termos espaço para o novo artigo no rank.

			int i;
			for (i = n-1; i > index; i--) {

				topN[i] = topN[i - 1];
			}

			// Colocar o utlizador na sua nova posição no rank.
			topN[index] = curArticle;
		}
	}

	// Importante - libertar a memória do iterador no final!
	freeIterator(iterator);

	// Obter array de ids e retorná-lo.

	long *topNIds = malloc(n * sizeof(long));

	int j;
	for (j = 0; j < n; j++) {
		topNIds[j] = (topN[j])->id;
	}

	return topNIds;
}


/**
 * @brief Função que obtém o top dos N artigos com mais palavras.
 *
 * Cria um iterador da hashtable correspondente aos artigos.
 * Itera pela hashtable e verifica através da função isTitlePrefix se aquele título tem aquele prefixo.
 * Caso tenha, aumenta o array e guarda o título.
 * No fim, é ordenado o array.
 *
 * @see isTitlePrefix()
 * @see getHashtableIterator()
 * @see getNextFromIterator()
 *
 * @param prefix O prefixo a ser pesquisado.
 * @param qs A estrutura geral do programa.
 * @return Um array com os títulos dos artigos com aquele prefixo, ou NULL, caso não hajam.
 */
char** getTitlesWithPrefix(char* prefix, TAD_istruct qs) {

	char** articlesWithPrefix = NULL;
	int numOfTitles = 0;

	void *iterator = getHashtableIterator(qs->articles);
	void *key = NULL;
	struct article *curArticle = NULL;

	// Iterar pela hash table de artigos
	while (getNextFromIterator(iterator, &key, &curArticle)) {

		if (isTitlePrefix(prefix, curArticle->title)) {

			numOfTitles++;
			articlesWithPrefix = realloc(articlesWithPrefix, numOfTitles * sizeof(char *));
			articlesWithPrefix[numOfTitles-1] = curArticle->title;

		}

	}

	// Ordena alfabeticamente
	char* tmp = NULL;
	int i, j;

	for (i = 1; i < numOfTitles; i++) {
 		for (j = 1; j < numOfTitles; j++) {
 			if (strcmp(articlesWithPrefix[j - 1], articlesWithPrefix[j]) > 0) {
 				tmp = articlesWithPrefix[j - 1];
 				articlesWithPrefix[j - 1] = articlesWithPrefix[j];
 				articlesWithPrefix[j] = tmp;
 			}
 		}
	}

	// Para a última posição retornar NULL.
	articlesWithPrefix = realloc(articlesWithPrefix, (numOfTitles + 1) * sizeof(char *));
	articlesWithPrefix[numOfTitles] = NULL;

	return articlesWithPrefix;
}
