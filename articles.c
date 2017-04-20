#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#include "articles.h"


// Testada e a funcionar ------- 14 apr
long wordCounter(char *revisionText, long *sizeBytes) {
	char *str = revisionText;
	long i;
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

	//return 1; // Comentei porque tava a dar erros ao compilar
}

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


/*
	-> chama a função de procura (da HASH) ID do artigo
		-> se não encontrar, chama a func insere (da HASH) articleID+title+
															sizeOfArticle(revisionText) (função do modulo ARTICLES)+
															numOfWords(revisionText) (funcao do modulo ARTICLES)+
															revisionId
		-> se encontrar, tem de fazer update do título
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

	//printf("articles.c - Received article revision data\n");

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


long get_all_articles(TAD_istruct qs) {
	struct TCD_istruct *ourTable = qs;

	return ourTable->allArticles;
}


long get_unique_articles(TAD_istruct qs) {
	struct TCD_istruct *ourTable = qs;

	return ourTable->uniqueArticles;
}


long get_all_revisions(TAD_istruct qs) {
	struct TCD_istruct *ourTable = qs;

	return ourTable->allRevisions;
}


char* get_article_title(long article_id, TAD_istruct qs) {
	struct article *ourArticle = getArticle(qs, article_id);

	if (ourArticle) {
		return ourArticle->title;
	} else {
		return NULL;
	}
}


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


long* getTop20LargestArticles(TAD_istruct qs) {

	//printf("Getting top 20 largest articles\n");

	void *iterator = getHashtableIterator(qs->articles);

	struct article *top20[20];

	// Inicializar array com shitty articles para começar

	struct article nothing = {(long) 1, (long) 1, (long) 1, "", NULL};

	int i;
	for (i = 0; i < 20; i++) {
		top20[i] = &nothing;
	}

	void *key = NULL;
	struct article *curArticle = NULL;

	int index;

	//printf("Iterating through users hash table\n");

	// Iterar pela hash table de artigos
	while (getNextFromIterator(iterator, &key, &curArticle)) {

		index = 19; // Rank onde o utilizador vai ser colocado

		if (curArticle->size >= (top20[19])->size) {

			//printf("A article's size >= top20[19]'s size\n");

			// Diminuir o indíce enquanto o artigo encaixar num rank superior
			while ((index >= 1) && (curArticle->size >= (top20[index - 1])->size)) {
				//printf("%d INDEX\n", index);
				//printf("%ld\n", curArticle->size);
				//printf("%ld\n", (top20[index - 1])->size);
				index--;
			}

			//printf("This article will be compared to index %d\n", index);

			// Neste ponto sabemos que o score deste article >= top[index]

			if (curArticle->size == (top20[index])->size) {

				//printf("The users size are equal\n");

				// Score deste article == top[index]
				// Comparar article id's

				if (curArticle->id > (top20[index])->id) {

					//printf("The article id lost\n");

					index++; // Colocar este artigo uma posição abaixo no rank

					if (index > 19) {

						//printf("User dropped to 21st place, skip\n");

						continue; // Utilizador desceu para 21º, skipar
					}

				} else {
					fprintf(stderr, "Erro ao comparar id dos artigos %ld e %ld\n", curArticle->id, (top20[index])->id);
				}

				//printf("The article id won\n");
			}

			// Deslizar os outros artigos para baixo para termos espaço para o novo artigo no rank

			int i;
			for (i = 19; i > index; i--) {

				top20[i] = top20[i - 1];
			}

			// Colocar o utlizador na sua nova posição no rank
			top20[index] = curArticle;
		}
	}

	// Importante - libertar a memória do iterador no final!
	freeIterator(iterator);

	// Obter array de ids e retorná-lo

	long *top20Ids = malloc(20 * sizeof(long));

	int j;
	for (j = 0; j < 20; j++) {
		top20Ids[j] = (top20[j])->id;
	}

	//printf("returning top20Ids...\n");

	return top20Ids;
}


long* getTopNArticlesWithMoreWords(int n, TAD_istruct qs) {

	//printf("Getting top n articles with more words\n");

	void *iterator = getHashtableIterator(qs->articles);

	struct article *topN[n];

	// Inicializar array com stupid articles para começar

	struct article nothing = {(long) 0, (long) 0, (long) 0, "", NULL};

	int i;
	for (i = 0; i < n; i++) {
		topN[i] = &nothing;
	}

	void *key = NULL;
	struct article *curArticle = NULL;

	int index;

	//printf("Iterating through users hash table\n");

	// Iterar pela hash table de artigos
	while (getNextFromIterator(iterator, &key, &curArticle)) {

		index = n-1; // Rank onde o utilizador vai ser colocado

		if (curArticle->nWords >= (topN[n-1])->nWords) {

			//printf("A article's nWords >= topN[n-1]'s nWords\n");

			// Diminuir o indíce enquanto o utilizador encaixar num rank superior
			while ((index >= 1) && curArticle->nWords >= (topN[index - 1])->nWords) {
				index--;
			}

			//printf("This article will be compared to index %d\n", index);

			// Neste ponto sabemos que o score deste article >= top[index]

			if (curArticle->nWords == (topN[index])->nWords) {

				//printf("The users nWords are equal\n");

				// Score deste article == top[index]
				// Comparar usernames alfabeticamente

				if (curArticle->id > (topN[index])->id) {

					//printf("The article id lost\n");

					index++; // Colocar este artigo uma posição abaixo no rank

					if (index > n-1) {

						//printf("User dropped to (n+1)st place, skip\n");

						continue; // Utilizador desceu para (n+1)º, skipar
					}

				} else {
					fprintf(stderr, "Erro ao comparar id dos artigos %ld e %ld\n", curArticle->id, (topN[index])->id);
				}

				//printf("The article id won\n");
			}

			// Deslizar os outros artigos para baixo para termos espaço para o novo artigo no rank

			int i;
			for (i = n-1; i > index; i--) {

				topN[i] = topN[i - 1];
			}

			// Colocar o utlizador na sua nova posição no rank
			topN[index] = curArticle;
		}
	}

	// Importante - libertar a memória do iterador no final!
	freeIterator(iterator);

	// Obter array de ids e retorná-lo

	long *topNIds = malloc(n * sizeof(long));

	int j;
	for (j = 0; j < n; j++) {
		topNIds[j] = (topN[j])->id;
	}

	return topNIds;
}


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

	//printf("ordenar...\n");
	// Ordena alfabeticamente
	char* tmp = NULL;
	int i, j;

	for (i = 1; i < numOfTitles; i++) {
 		for (j = 1; j < numOfTitles; j++) {
 			if (strcmp(articlesWithPrefix[j - 1], articlesWithPrefix[j]) > 0) {
 				//printf("aqui\n");
 				tmp = articlesWithPrefix[j - 1];
 				//printf("copia antes\n");
 				articlesWithPrefix[j - 1] = articlesWithPrefix[j];
 				//printf("dá o tmp\n");
 				articlesWithPrefix[j] = tmp;
 			}
 		}
	}

	//printf("finalizar...\n");
	// Para a última posição retornar NULL
	articlesWithPrefix = realloc(articlesWithPrefix, (numOfTitles + 1) * sizeof(char *));
	articlesWithPrefix[numOfTitles] = NULL;

	return articlesWithPrefix;
}
