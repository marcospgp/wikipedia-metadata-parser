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

}
