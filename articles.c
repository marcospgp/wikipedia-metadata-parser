#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#include "articles.h"

// TENHO DE TESTAR!!

long wordCounter(char *revisionText) {
	char *str;
	int i;
	long count = 0;
	int foundLetter = False;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == ' ')
			foundLetter = False;
		else {
      		if (foundLetter == False)
        		count++;
    	foundLetter = True;
		}
	}

	return count;
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

// TODO - fazer a função que conta o número de palavras e a função que conta o número de caracteres

	long sizeBytes = strlen(revisionText)
	long nWords = wordCounter(revisionText);


/* Esta função deve ser chamada quando é encontrada uma revisão de um artigo.
 * A função cria ou atualiza um artigo já existence na tabela. Caso seja feita uma
 * atualização, o tamanho do artigo e número de palavras só é atualizado se for
 * maior que o anterior. Os valores restantes são sempre atualizados.
*/
	qs = onRevision(articleId, title, revisionId, revisionTimestamp, sizeBytes, nWords);

}
