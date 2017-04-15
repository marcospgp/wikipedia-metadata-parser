#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#include "users.h"

TAD_istruct onPageUsers(TAD_istruct qs, long id, char *username, long articleId, long revisionId) {

	//printf("users.c - Received user data\n");

	// Não fazer nada se isto for uma revisão duplicada

	struct article *articlePtr = getArticle(qs, articleId);

	if (articlePtr && getRevision(articlePtr->revisions, revisionId)) {

		return qs;
	}

	int userWasFound;

	qs = insertOrUpdateUser(qs, id, username, &userWasFound);

	return qs;
}

char* getContributorName(TAD_istruct qs, long id) {

	struct user *ourUser = getUser(qs, id);

	if (ourUser) {
		return ourUser->username;
	} else {
		return NULL;
	}
}

long* getTop10Contributors(TAD_istruct qs) {

	//printf("Getting top 10 contributors\n");

	void *iterator = getHashtableIterator(qs->users);

	struct user *top10[10];

	// Inicializar array com dummy users para começar

	struct user nobody = {(long) 0, (long) 0, ""};

	int i;
	for (i = 0; i < 10; i++) {
		top10[i] = &nobody;
	}

	void *key = NULL;
	struct user *curUser = NULL;

	int index;

	//printf("Iterating through users hash table\n");

	// Iterar pela hash table de utilizadores
	while (getNextFromIterator(iterator, &key, &curUser)) {

		index = 9; // Rank onde o utilizador vai ser colocado

		if (curUser->contributions >= (top10[9])->contributions) {

			//printf("A user's contributions >= top10[9]'s contributions\n");

			// Diminuir o indíce enquanto o utilizador encaixar num rank superior
			while (curUser->contributions >= (top10[index - 1])->contributions) {
				index--;
			}

			//printf("This user will be compared to index %d\n", index);

			// Neste ponto sabemos que o score deste user >= top[index]

			if (curUser->contributions == (top10[index])->contributions) {

				//printf("The users contributions are equal\n");

				// Score deste user == top[index]
				// Comparar usernames alfabeticamente

				int result = strcmp(curUser->username, (top10[index])->username);

				if (result < 0) {

					//printf("The user alphabetically lost\n");

					index++; // Colocar este utilizador uma posição abaixo no rank

					if (index > 9) {

						//printf("User dropped to 11th place, skip\n");

						continue; // Utilizador desceu para 11º, skipar
					}

				} else if (result == 0) {
					fprintf(stderr, "Erro ao comparar contribuicoes dos utilizadores %s e %s\n", curUser->username, (top10[index])->username);
				}

				//printf("The user alphabetically won\n");
			}

			// Deslizar os outros utilizadores para baixo para termos espaço para o novo utilizador no rank

			int i;
			for (i = 9; i > index; i--) {

				top10[i] = top10[i - 1];
			}

			// Colocar o utlizador na sua nova posição no rank
			top10[index] = curUser;
		}
	}

	// Importante - libertar a memória do iterador no final!
	freeIterator(iterator);

	// Obter array de ids e retorná-lo

	long *top10Ids = malloc(10 * sizeof(long));

	int j;
	for (j = 0; j < 10; j++) {
		top10Ids[j] = (top10[j])->id;
	}

	return top10Ids;
}
