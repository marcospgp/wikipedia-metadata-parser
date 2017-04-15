#ifndef users_h
#define users_h

// Função chamada em parser.c que nos passa dados
TAD_istruct onPageUsers(TAD_istruct qs, long id, char *username, long articleId, long revisionId);

char* getContributorName(TAD_istruct qs, long id);

long* getTop10Contributors(TAD_istruct qs);

#endif
