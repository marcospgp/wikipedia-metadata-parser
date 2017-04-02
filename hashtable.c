#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "hashtable.h"

// TODO - guardar char *title ao receber antes de ser apagado

TAD_istruct initHashtables(TAD_istruct qs) {

	GHashTable *users = g_hash_table_new(NULL, NULL);
	GHashTable *articles = g_hash_table_new(NULL, NULL);

	qs->users = users;
	qs->articles = articles;

	return qs;
}

TAD_istruct onUserContribution(TAD_istruct qs, long id, char *username) {

	// Procurar pelo utilizador na hash table
	struct user *userData = (struct user*) g_hash_table_lookup(qs->users, &id);

	if (userData == NULL) {

		// Criar novo utilizador

		char *usernameCopy = g_strdup(username); // Copiar string porque o parser pode apagá-la

		struct user newUser = {id, 1, usernameCopy};

		userData = &newUser;

	} else {
		userData->contributions = (userData->contributions) + 1;
	}

	// Inserir utilizador na hash table
	g_hash_table_insert(qs->users, &id, &userData);

	return qs;
}

TAD_istruct onRevision(TAD_istruct qs, long id, char *title, long revisionId, char *revisionTimestamp, long sizeBytes, long nWords) {

	// Copiar strings porque o parser pode apagá-las
	char *newTitle = g_strdup(title);
	char *newTimestamp = g_strdup(revisionTimestamp);

	// Criar nova revisão (necessária caso o artigo já exista ou não)
	struct revision newRevision = {revisionId, newTimestamp};

	// Verificar se este artigo já existe na hash table

	struct article *articleData = (struct article*) g_hash_table_lookup(qs->articles, &id);

	if (articleData == NULL) {

		// Criar hashtable de revisões
		GHashTable *revisions = g_hash_table_new(NULL, NULL);

		// Adicionar revisão à hashtable de revisões
		g_hash_table_insert(revisions, &revisionId, &newRevision);

		// Criar novo artigo
		struct article newArticle = {id, sizeBytes, nWords, newTitle, revisions};

		// Adicionar novo artigo à hashtable
		g_hash_table_insert(qs->users, &id, &newArticle);

	} else {

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

		articleData->title = newTitle;

		// Adicionar revisão

		GHashTable *revisions = articleData->revisions;

		g_hash_table_insert(revisions, &revisionId, &newRevision);
	}

	return qs;
}
