#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "settings.h"

#include "users.h"

TAD_istruct onPageUsers(TAD_istruct qs, long id, char *username) {

/*
	-> no módulo, vai ter de chamar a função (da HASH) de procura por ID
		-> se encontrar, chama a função (da HASH) que aumenta o numOfContributions
		-> se não encontrar, chama a função (da HASH) que insere o ID+username+counter=1
*/

	printf("users.c - Received user data\n");

	qs = insertOrUpdateUser(qs, id, username);

	return qs;
}

char* getContributorName(TAD_istruct qs, long id) {

	struct user *ourUser = getUser(qs, id);

	return ourUser->username;
}

long* getTop10Contributors(TAD_istruct qs) {

	void *iterator = getHashtableIterator(qs->users);

	void *key;
	struct user curUser;

	struct user *top10[10];

	// Inicializar array com dummy users para começar
	int i;
	for (i = 0; i < 10; i++) {
		top10[i] = {(long) 0, (long) 0, ""};
	}

	int index = 9;

	// Iterar pela hash table de utilizadores
	while (getNextFromIterator(iterator, key, (void*) &curUser)) {

		if (curUser->contributions >= top10[9]) {

			// Diminuir o indíce enquanto o utilizador encaixar num rank superior
			while (curUser->contributions >= (top10[index - 1])->contributions) {
				index--;
			}

			// Neste ponto sabemos que o score deste user >= top[index]

			if (curUser->contributions == top10[index]) {

				// Comparar alfabeticamente

				int result = strcmp(curUser->username, )

				if (result > 0) {

				} else if (result < 0) {

				} else {
					fprintf(stderr, "Erro ao comparar contribuicoes dos utilizadores %s e %s\n", curUser->username, (top10[index])->username);
				}

			} else {
				// score deste user > top[index]
			}
		}
	}

	// Importante - libertar a memória do iterador no final!
	freeIterator(iterator);
}
