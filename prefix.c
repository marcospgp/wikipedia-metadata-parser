#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int titles_with_prefix(char* prefix, char* title) {

	char* prefixo = prefix;
	char* string = title;
	int i;

	for (i = 0; (prefixo[i] != '\0'); i++) {
		if (string[i] != '\0') {
			if (prefixo[i] == string[i]) continue;
			else {
				printf("0\n");
				return 0;
			}
		}
		// caso o prefix seja maior que o titulo
		else {
			printf("0\n");
			return 0;
		}
	}

	printf("1\n");
	return 1;

}


int main () {

	char** articlesWithPrefix = NULL;
	int numOfTitles = 0;

	char* prefixoInput = "canecase";
	char* tituloInput = "canecasde";
	char* prefixoInput2 = "chouri";
	char* tituloInput2 = "chour";

	// quando aquele titulo tem o prefixo
	if (titles_with_prefix(prefixoInput, tituloInput)) {
		numOfTitles++;
		articlesWithPrefix = realloc(articlesWithPrefix, numOfTitles * sizeof(char *));
		articlesWithPrefix[numOfTitles-1] = tituloInput;
	}

	if (titles_with_prefix(prefixoInput2, tituloInput2)) {
		numOfTitles++;
		articlesWithPrefix = realloc(articlesWithPrefix, numOfTitles * sizeof(char *));
		articlesWithPrefix[numOfTitles-1] = tituloInput2;
	}


	articlesWithPrefix = realloc(articlesWithPrefix, (numOfTitles + 1) * sizeof(char *));
	articlesWithPrefix[numOfTitles] = NULL;

	// imprime a lista de titulos
	int i;
	for (i = 0; i <= numOfTitles; i++) {
		printf("%s\n", articlesWithPrefix[i]);
	}

	return 1;
}