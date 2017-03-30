#ifndef articles_h
#define articles_h

// Função chamada em parser.c que passa dados para articles.c
TAD_istruct onPageArticles(TAD_istruct qs, long articleId, char *title, char *revisionText, long revisionId, long revisionParentId);

#endif