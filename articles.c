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
