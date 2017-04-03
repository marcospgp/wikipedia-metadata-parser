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

	qs = onUserContribution(qs, id, username);

	return qs;
}
