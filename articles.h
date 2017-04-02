#ifndef articles_h
#define articles_h

#include "hashtable.h"

// Função chamada em parser.c que nos passa dados
TAD_istruct onPageArticles(
    TAD_istruct qs,
    long articleId,
    char *title,
    char *revisionText,
    long revisionId,
    long revisionParentId,
    char *revisionTimestamp
);

#endif
