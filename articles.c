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
			if (str[i] == ' ')
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


// NAO TESTADO - 12 apr
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

// Para ver se o wordCounter está a funcionar
long get_article_size(long article_id, TAD_istruct qs) {
	struct article *ourArticle = getArticle(qs, article_id);

	if (ourArticle) {
		return ourArticle->size;
	} else {
		return (long) 0;
	}
}

long get_article_nWords(long article_id, TAD_istruct qs) {
	struct article *ourArticle = getArticle(qs, article_id);

	if (ourArticle) {
		return ourArticle->nWords;
	} else {
		return (long) 0;
	}
}

long* getTop20LargestArticles(TAD_istruct qs) {

	//printf("Getting top 20 largest articles\n");

	void *iterator = getHashtableIterator(qs->articles);

	struct article *top20[20];

	// Inicializar array com shitty articles para começar

	struct article nothing = {(long) 0, (long) 0, (long) 0, "", NULL};

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
			while (curArticle->size >= (top20[index - 1])->size) {
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

	return top20Ids;
}