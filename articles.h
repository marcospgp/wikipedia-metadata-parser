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

long get_all_articles(TAD_istruct qs);

long get_unique_articles(TAD_istruct qs);

long get_all_revisions(TAD_istruct qs);

char* get_article_title(long article_id, TAD_istruct qs);

#endif
