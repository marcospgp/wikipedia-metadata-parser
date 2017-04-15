#ifndef hashtable_h
#define hashtable_h

#include <glib.h>

typedef struct TCD_istruct* TAD_istruct;

struct TCD_istruct {
	GHashTable *users;
	GHashTable *articles;
	long allArticles;
	long uniqueArticles;
	long allRevisions;
};

struct article {
	long id, size, nWords;
	char *title;
	GHashTable *revisions; // Hashtable que guarda todas as revisoes do artigo, incluindo timestamp
};

struct user {
	long id, contributions;
	char *username;
};

struct revision {
	long id;
	char *timestamp;
};

/* Substitui os apontadores NULL da estrutura com apontadores para as respetivas hash tables
 */
TAD_istruct initHashtables(TAD_istruct qs);

/* Esta função deve ser chamada quando é encontrada uma contribuição de um certo utilizador.
 * O módulo deve manter internamente o cálculo do número de contribuições de cada utilizador.
 *
 * @param userWasFound - Usado para retornar 1 se o utlizador foi atualizado, 0 se foi criado um novo
 */
TAD_istruct insertOrUpdateUser(TAD_istruct qs, long id, char *username, int *userWasFound);

/* Esta função deve ser chamada quando é encontrada uma revisão de um artigo.
 * A função cria ou atualiza um artigo já existente na tabela. Caso seja feita uma
 * atualização, o tamanho do artigo e número de palavras só é atualizado se for
 * maior que o anterior. Os valores restantes são sempre atualizados.
 *
 * @param articleFound - Usado para retornar 1 se o artigo foi encontrado
 * @param articleFound - Usado para retornar 1 se o artigo foi atualizado, 0 se a revisão foi ignorada porque era duplicada
 */
TAD_istruct insertOrUpdateArticle(TAD_istruct qs, long id, char *title, long revisionId, char *revisionTimestamp, long sizeBytes, long nWords, int *articleFound, int *articleUpdated);

/* Retorna um utilizador a partir da sua id */
struct user * getUser(TAD_istruct qs, long id);

/* Retorna um artigo a partir da sua id */
struct article * getArticle(TAD_istruct qs, long id);

/* Retorna uma revisão a partir da hash table de revisões e da id */
struct revision * getRevision(void *revisionsHashTable, long revisionId);

/* Retorna um iterador para uma hash table */
void * getHashtableIterator(GHashTable *hashtable);

/* Obter o próximo elemento de um iterador de hash table */
int getNextFromIterator(void *iterator, void *key, void *value);

/* Liberta a memória ocupada por um iterador */
void freeIterator(void *iterator);

#endif
