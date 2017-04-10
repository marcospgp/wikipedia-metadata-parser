#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "interface.h"

#define NUM_ALL_ARTICLES 1987654
#define NUM_UNIQUE_ARTICLES 1234567
#define NUM_ALL_REVISIONS 1347356


/*
	Este ficheiro vai ser substituído pelos professores.
	Aqui devem ser executados testes usando as funções definidas em interface.h e os datasets de teste
*/

//qs = init();
//load(qs, int nsnaps, char* snaps_paths[]);

/*
int corretos = 0;

printf("Testing all_articles...\n");
long allArticlesResult = all_articles(qs);
if (allArticlesResult == NUM_ALL_ARTICLES) {printf("CORRETO!\n"); corretos++;}
else printf("Valor esperado: %ld\n Valor obtido: %ld", NUM_ALL_ARTICLES, allArticlesResult);

printf("Testing unique_articles...\n");
long uniqueArticlesResult = unique_articles(qs);
if (uniqueArticlesResult == NUM_UNIQUE_ARTICLES) {printf("CORRETO!\n"); corretos++;}
else printf("Valor esperado: %d\n Valor obtido: %d", NUM_UNIQUE_ARTICLES, uniqueArticlesResult);

printf("Testing all_revisions...\n");
long allRevisionsResult = all_revisions(qs);
if (allRevisionsResult == NUM_ALL_REVISIONS) {printf("CORRETO!\n"); corretos++;}
else printf("Valor esperado: %d\n Valor obtido: %d", NUM_ALL_REVISIONS, allRevisionsResult);


long* top_10_contributors(TAD_istruct qs);
char* contributor_name(long contributor_id, TAD_istruct qs);
long* top_20_largest_articles(TAD_istruct qs);
char* article_title(long article_id, TAD_istruct qs);
long* top_N_articles_with_more_words(int n, TAD_istruct qs);
char** titles_with_prefix(char* prefix, TAD_istruct qs);
char* article_timestamp(long article_id, long revision_id, TAD_istruct qs);
//TAD_istruct clean(TAD_istruct qs);

printf("Acertaste em %d de 10 testes.\n", corretos);

*/

int main(int argc, char const *argv[]) {

	printf("Starting timer\n");

	clock_t execStart = clock();

	printf("Calling init()\n");

	TAD_istruct qs = init();

	printf("Calling load()\n\n");

	char *paths[] = {"datasets/dec16", "datasets/jan17", "datasets/fev17"};

	qs = load(qs, 1, paths);

	// TODO - Adicionar chamadas de teste!

	// Todas estas queries são em relação ao dataset dec16 - Marcos

	char *query;

	printf("Is user 9092818 Paine Ellsworth?\n");
	query = contributor_name((long) 9092818, qs);
	printf("Answer: %s\n\n", query);

	printf("Is user 17585538 Telfordbuck?\n");
	query = contributor_name((long) 17585538, qs);
	printf("Answer: %s\n\n", query);

	printf("Is user 7328338 Yobot?\n");
	query = contributor_name((long) 7328338, qs);
	printf("Answer: %s\n\n", query);

	printf("Is user 9635580 Simple Bob?\n");
	query = contributor_name((long) 9635580, qs);
	printf("Answer: %s\n\n", query);

	// Testar top 10 contributors ------------

	printf("Top 10 contributors:\n");

	long *top10Contributors = top_10_contributors(qs);

	int i;
	for (i = 0; i < 10; i++) {
		printf("%d: %ld (%s)\n", (i + 1), top10Contributors[i], contributor_name(top10Contributors[i], qs));
	}

	free(top10Contributors);

	// ---------------------------------------

	// Terminar timer

	clock_t execEnd = clock();
	float timeElapsed = (float)(execEnd - execStart) / CLOCKS_PER_SEC;

	printf("\nAll done. Time elapsed: %f seconds\n", timeElapsed);

	return 0;
}
