#ifndef hashtable_h
#define hashtable_h

typedef struct TCD_istruct* TAD_istruct;

typedef struct __article {
	long id, size, words;
	char title[255]; // Limite de tamanho de título da wikipédia
} article;

typedef struct __user {
	long id, contributions;
	char username[255];
} user;

typedef struct __revision {
	long id;
	char timestamp[255];
} revision;

/* Substitui os apontadores NULL da estrutura com apontadores para as respetivas hash tables
 */
TAD_istruct initHashtables(TAD_istruct qs);

/* Esta função deve ser chamada quando é encontrada uma contribuição de um certo utilizador.
 * O módulo deve manter internamente o cálculo do número de contribuições de cada utilizador.
 */
TAD_istruct onUserContribution(TAD_istruct qs, long id, char *username);

/* Esta função deve ser chamada quando é encontrada uma revisão de um artigo.
 * A função cria ou atualiza um artigo já existence na tabela. Caso seja feita uma
 * atualização, o tamanho do artigo e número de palavras só é atualizado se for
 * maior que o anterior. Os valores restantes são sempre atualizados.
*/
TAD_istruct onRevision(TAD_istruct qs, long id, char *title, long revisionId, char *revisionTimestamp, long sizeBytes, long nWords);

// TODO - função que permita fazer loop pelas hash tables

#endif
