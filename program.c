#include <stdio.h>
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

	printf("Calling init()\n");

	TAD_istruct qs = init();

	printf("Calling load()\n");

	char *paths[] = {"datateste"};

	qs = load(qs, 1, paths);

	// TODO - Adicionar chamadas de teste!

	return 0;
}
