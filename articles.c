#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#include "articles.h"

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


	qs = onRevision(articleId, title, revisionId, revisionTimestamp, long sizeBytes, long nWords);

}
