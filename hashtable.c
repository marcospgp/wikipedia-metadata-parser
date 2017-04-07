#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "settings.h"

#include "hashtable.h"

TAD_istruct initHashtables(TAD_istruct qs) {

	GHashTable *users = g_hash_table_new_full(g_int64_hash, g_int64_equal, free, free);
	GHashTable *articles = g_hash_table_new_full(g_int64_hash, g_int64_equal, free, free);

	qs->users = users;
	qs->articles = articles;

	return qs;
}

TAD_istruct insertOrUpdateUser(TAD_istruct qs, long id, char *username) {

	printf("hashtable.c - Received user data\n");

	// Procurar pelo utilizador na hash table
	struct user *userData = (struct user*) g_hash_table_lookup(qs->users, &id);

	if (userData == NULL) {

		printf("Creating new user with id %ld\n", id);

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

		printf("Updating user with id %ld\n", id);

		userData->contributions = (userData->contributions) + 1;

		// Nota - não é preciso inserir de novo na hashtable porque o que
		// está lá é o endereço para a estrutura que alteramos
	}

	return qs;
}

TAD_istruct insertOrUpdateArticle(TAD_istruct qs, long id, char *title, long revisionId, char *revisionTimestamp, long sizeBytes, long nWords) {

	printf("hashtable.c - Received article revision data\n");

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

		printf("Creating new article and adding revision...\n");

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
		g_hash_table_insert(qs->users, idCopyPtr, newArticle);

	} else {

		printf("Updating article...\n");

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

		printf("Adding revision...\n");

		GHashTable *revisions = articleData->revisions;

		g_hash_table_insert(revisions, revisionIdCopyPtr, newRevision);
	}

	return qs;
}

struct user * getUser(TAD_istruct qs, long id) {

	return (struct user*) g_hash_table_lookup(qs->users, &id);
}

void * getContributorsIterator(TAD_istruct qs) {

	GHashTableIter *iter = malloc(sizeof(GHashTableIter));

	g_hash_table_iter_init(iter, qs->users);

	return (void*) iter;
}

void * getArticlesIterator(TAD_istruct qs) {

	GHashTableIter *iter = malloc(sizeof(GHashTableIter));

	g_hash_table_iter_init(iter, qs->articles);

	return (void*) iter;
}

void getNextFromIterator(void *iterator, void *key, void *value) {

	g_hash_table_iter_next(iterator, key, value);
}

void freeIterator(void *iterator) {
	free(iterator);
}
