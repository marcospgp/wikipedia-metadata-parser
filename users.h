#ifndef users_h
#define users_h

typedef struct TCD_istruct* TAD_istruct;

// Função chamada em parser.c que nos passa dados
TAD_istruct onPageUsers(TAD_istruct qs, long id, char *username);

#endif
