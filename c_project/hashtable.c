#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "hashtable.h"

/**
 * @brief Inicia a estrutura geral do programa.
 *
 * @param qs A estrutura geral do programa que é nula ainda.
 * @return A estrutura iniciada.
 */
TAD_istruct initHashtables(TAD_istruct qs) {

	GHashTable *users = g_hash_table_new_full(g_int64_hash, g_int64_equal, free, free);
	GHashTable *articles = g_hash_table_new_full(g_int64_hash, g_int64_equal, free, free);

	qs->users = users;
	qs->articles = articles;

	return qs;
}

/**
 * @brief Insere ou atualiza um utilizador na hashtable de utilizadores.
 *
 * Procurar pelo utilizador na hash table e, caso o utilizador não foi encontrado,
 * criar e inserir novo utilizador.
 * Caso tenha sido encontrado, apenas aumentar uma contribuição ao utilizador.
 *
 * @param qs A estrutura geral do programa.
 * @param id A ID do contribuidor.
 * @param username O username do contribuidor.
 * @param userWasFound Passado para fazer controlo.
 * @return A estrutura atualizada.
 */
TAD_istruct insertOrUpdateUser(TAD_istruct qs, long id, char *username, int *userWasFound) {

	//printf("hashtable.c - Received user data\n");

	// Procurar pelo utilizador na hash table
	struct user *userData = (struct user*) g_hash_table_lookup(qs->users, &id);

	if (userData == NULL) {

		//printf("Creating new user with id %ld\n", id);

		// Informar que o utilizador não foi encontrado
		*userWasFound = 0;

		// Criar novo utilizador

		long *idCopyPtr = malloc(sizeof(long)); // Copiar id porque a hash table só guarda um pointer

		*idCopyPtr = id;

		char *usernameCopy = g_strdup(username); // Copiar string porque o parser pode apagá-la

		struct user *newUser = malloc(sizeof(struct user));

		newUser->id = id;
		newUser->contributions = 1;
		newUser->username = usernameCopy;

		// Inserir novo utilizador na hash table
		g_hash_table_insert(qs->users, idCopyPtr, newUser);

	} else {

		//printf("Updating user with id %ld\n", id);

		// Informar que o utilizador foi encontrado
		*userWasFound = 1;

		// Contar uma contribuição do utilizador
		userData->contributions = (userData->contributions) + 1;

		// Nota - não é preciso inserir de novo na hashtable porque o que
		// está lá é o endereço para a estrutura que alteramos
	}

	return qs;
}

/**
 * @brief Insere ou atualiza um artigo na hashtable de utilizadores.
 *
 * Copiar strings porque o parser pode apagá-las e a hash table só guarda um pointer.
 * Copiar longs usadas como key também, porque keys são usadas a partir dos seus pointers.
 * Criar nova revisão (necessária caso o artigo já exista ou não).
 * Informar se o artigo foi encontrado ou não através de articleFound e articleUpdated.
 * Criar ou atualizar o artigo dependendo se este foi encontrado ou não.
 *
 * @param qs A estrutura geral do programa.
 * @param id A ID do artigo.
 * @param title O título do artigo.
 * @param revisionId O ID da revisão.
 * @param revisionTimestamp O timestamp da revisão.
 * @param sizeBytes O tamanho do artigo.
 * @param nWords O número de palavras do artigo.
 * @param articleFound Passado para fazer controlo nos contadores de articles.
 * @param articleUpdated Passado para fazer controlo nos contadores de articles.
 * @return A estrutura atualizada.
 */
TAD_istruct insertOrUpdateArticle(TAD_istruct qs, long id, char *title, long revisionId, char *revisionTimestamp, long sizeBytes, long nWords, int *articleFound, int *articleUpdated) {

	//printf("hashtable.c - Received article revision data\n");

	// Copiar strings porque o parser pode apagá-las
	// E a hash table só guarda um pointer
	// Copiar longs usadas como key também, porque keys
	// são usadas a partir dos seus pointers

	char *titleCopy = g_strdup(title);
	char *timestampCopy = g_strdup(revisionTimestamp);

	long *idCopyPtr = malloc(sizeof(long));
	long *revisionIdCopyPtr = malloc(sizeof(long));

	*idCopyPtr = id;
	*revisionIdCopyPtr = revisionId;

	// Criar nova revisão (necessária caso o artigo já exista ou não)
	struct revision *newRevision = malloc(sizeof(struct revision));

	newRevision->id = revisionId;
	newRevision->timestamp = timestampCopy;

	// Verificar se este artigo já existe na hash table

	struct article *articleData = (struct article*) g_hash_table_lookup(qs->articles, idCopyPtr);

	if (articleData == NULL) {

		//printf("Creating new article and adding revision...\n");

		// Informar que o artigo não foi encontrado
		*articleFound = 0;
		*articleUpdated = 1;

		// Criar hashtable de revisões
		GHashTable *revisions = g_hash_table_new_full(g_int64_hash, g_int64_equal, free, free);

		// Adicionar revisão à hashtable de revisões
		g_hash_table_insert(revisions, revisionIdCopyPtr, newRevision);

		// Criar novo artigo

		struct article *newArticle = malloc(sizeof(struct article));

		newArticle->id = id;
		newArticle->size = sizeBytes;
		newArticle->nWords = nWords;
		newArticle->title = titleCopy;
		newArticle->revisions = revisions;

		// Adicionar novo artigo à hashtable
		g_hash_table_insert(qs->articles, idCopyPtr, newArticle);

	} else {

		//printf("Updating article...\n");

		// Informar que o artigo foi encontrado
		*articleFound = 1;

		// Atualizar artigo já existente
		// Apenas atualizar tamanho e nº de palavras do artigo se for maior que o anterior

		long currentSizeBytes = articleData->size;

		long currentNWords = articleData->nWords;

		if (sizeBytes > currentSizeBytes) {
			articleData->size = sizeBytes;
		}

		if (nWords > currentNWords) {
			articleData->nWords = nWords;
		}

		// Atualizar título

		g_free(articleData->title);

		articleData->title = titleCopy;

		// Adicionar revisão

		GHashTable *revisions = articleData->revisions;

		if (g_hash_table_lookup(revisions, &revisionId)) {

			//printf("Duplicated revision. Skipping...\n");
			*articleUpdated = 0;

		} else {

			//printf("Adding revision...\n");
			g_hash_table_insert(revisions, revisionIdCopyPtr, newRevision);
			*articleUpdated = 1;
		}
	}

	return qs;
}

/**
 * @brief Limpa a estrutura geral do programa.
 *
 * @param qs A estrutura geral do programa.
 * @return A estrutura vazia.
 */
TAD_istruct clean_everything(TAD_istruct qs) {

	// Remover todas as hash tables de revisões

	void *iterator = getHashtableIterator(qs->articles);

	int *key = NULL;
	struct article *art = NULL;

	while (getNextFromIterator(iterator, &key, &art)) {

		g_hash_table_remove_all(art->revisions);
	}

	// Remover hash tables de users e articles

	g_hash_table_remove_all(qs->users);

	g_hash_table_remove_all(qs->articles);

	return qs;
}

/**
 * @brief Obtém um utilizador de acordo com a ID.
 *
 * @param qs A hashtable.
 * @param id O ID do utilizador a obter.
 * @return O utilizador.
 */
struct user * getUser(TAD_istruct qs, long id) {

	return (struct user*) g_hash_table_lookup(qs->users, &id);
}

/**
 * @brief Obtém um artigo de acordo com a ID.
 *
 * @param qs A hashtable.
 * @param id O ID do artigo a obter.
 * @return O artigo.
 */
struct article * getArticle(TAD_istruct qs, long id) {

	return (struct article*) g_hash_table_lookup(qs->articles, &id);
}

/**
 * @brief Obtém uma revisão de acordo com a ID.
 *
 * @param revisionsHashTable A hashtable de revisões.
 * @param revisionId O ID da revisão a obter.
 * @return A revisão.
 */
struct revision * getRevision(void *revisionsHashTable, long revisionId) {

	return (struct revision*) g_hash_table_lookup(revisionsHashTable, &revisionId);
}

/**
 * @brief Obtém um iterador.
 */
void * getHashtableIterator(GHashTable *hashtable) {

	GHashTableIter *iter = malloc(sizeof(GHashTableIter));

	g_hash_table_iter_init(iter, hashtable);

	return (void*) iter;
}

/**
 * @brief Obtém o próximo elemento a partir de um iterador.
 */
int getNextFromIterator(void *iterator, void *key, void *value) {

	int result = (int) g_hash_table_iter_next(iterator, (gpointer) key, (gpointer) value);

	return result;
}

/**
 * @brief Liberta a memória de um iterador.
 */
void freeIterator(void *iterator) {
	free(iterator);
}
