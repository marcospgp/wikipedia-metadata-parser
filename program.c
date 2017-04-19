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

teste para long all_articles(TAD_istruct qs);
printf("Testing all_articles...\n");
long allArticlesResult = all_articles(qs);
if (allArticlesResult == NUM_ALL_ARTICLES) {printf("CORRETO!\n"); corretos++;}
else printf("Valor esperado: %ld\n Valor obtido: %ld", NUM_ALL_ARTICLES, allArticlesResult);

teste para long unique_articles(TAD_istruct qs);
printf("Testing unique_articles...\n");
long uniqueArticlesResult = unique_articles(qs);
if (uniqueArticlesResult == NUM_UNIQUE_ARTICLES) {printf("CORRETO!\n"); corretos++;}
else printf("Valor esperado: %d\n Valor obtido: %d", NUM_UNIQUE_ARTICLES, uniqueArticlesResult);

teste para long all_revisions(TAD_istruct qs);
printf("Testing all_revisions...\n");
long allRevisionsResult = all_revisions(qs);
if (allRevisionsResult == NUM_ALL_REVISIONS) {printf("CORRETO!\n"); corretos++;}
else printf("Valor esperado: %d\n Valor obtido: %d", NUM_ALL_REVISIONS, allRevisionsResult);

teste/print dos long* top_10_contributors(TAD_istruct qs);
for(int i = 0;contributor_rank < 11;i++){
	printf("contributor com rank %d: %s \n",i,contributor_name(i));
}

teste para char* contributor_name(long contributor_id, TAD_istruct qs);
printf("Testing contributor_name...\n");
printf("Is user 9092818 Paine Ellsworth?\n");
query = contributor_name((long) 9092818, qs);
printf("Answer: %s\n\n", query);
printf("Is user 17585538 Telfordbuck?\n");
query = contributor_name((long) 17585538, qs);
printf("Answer: %s\n\n", query);
printf("Is user 7328338 Yobot?\n");
query = contributor_name((long) 7328338, qs);
printf("Answer: %s\n\n", query);
printf("Is user 9635580 Vitor?\n");
query = contributor_name((long) 9635580, qs);
printf("Answer: %s\n\n", query);
printf("Is user 9635580 Marcos?\n");
query = contributor_name((long) 9635580, qs);
printf("Answer: %s\n\n", query);
printf("Is user 9635580 Sergio?\n");
query = contributor_name((long) 9635580, qs);
printf("Answer: %s\n\n", query);

teste/print dos long* top_20_largest_articles(TAD_istruct qs);
for(int i = 0;article_rank < 21;i++){
	printf("article com rank %d: %s \n",i,article_name(i));
}

teste para char* article_title(long article_id, TAD_istruct qs);
printf("Testing article_title...\n");
printf("Is article 9092818 Paine Ellsworth?\n");
query = article_name((long) 9092818, qs);
printf("Answer: %s\n\n", query);
printf("Is article 17585538 Telfordbuck?\n");
query = article_name((long) 17585538, qs);
printf("Answer: %s\n\n", query);
printf("Is article 7328338 Yobot?\n");
query = article_name((long) 7328338, qs);
printf("Answer: %s\n\n", query);
printf("Is article 9635580 Vitor?\n");
query = article_name((long) 9635580, qs);
printf("Answer: %s\n\n", query);
printf("Is article 9635580 Marcos?\n");
query = article_name((long) 9635580, qs);
printf("Answer: %s\n\n", query);
printf("Is article 9635580 Sergio?\n");
query = article_name((long) 9635580, qs);
printf("Answer: %s\n\n", query);

teste/print para long* top_N_articles_with_more_words(int n, TAD_istruct qs);
for(int i = 0;article_top < n+1;i++){
	printf("article com rank %d: %s \n",i,article(i));
}

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

	qs = load(qs, 3, paths);

	// TODO - Adicionar chamadas de teste!

	// Todas estas queries são em relação ao dataset dec16 - Marcos

	char *query;

	printf("Is user 9092818 Paine Ellsworth?\n");
	query = contributor_name((long) 9092818, qs);
	printf("Answer: %s\n", query);

	printf("Is user 17585538 Telfordbuck?\n");
	query = contributor_name((long) 17585538, qs);
	printf("Answer: %s\n", query);

	printf("Is user 7328338 Yobot?\n");
	query = contributor_name((long) 7328338, qs);
	printf("Answer: %s\n", query);

	printf("Is user 9635580 Simple Bob?\n");
	query = contributor_name((long) 9635580, qs);
	printf("Answer: %s\n", query);

	// Testar top 10 contributors ------------

	printf("\nTop 10 contributors:\n");

	long *top10Contributors = top_10_contributors(qs);

	int i;
	for (i = 0; i < 10; i++) {
		printf("%d: %ld (%s)\n", (i + 1), top10Contributors[i], contributor_name(top10Contributors[i], qs));
	}

	free(top10Contributors);

	// ---------------------------------------


	// Testar allArticles, uniqueArticles, allRevisions ------------

	long allArticlesNum = all_articles(qs);
	printf("\nallArticles: %ld\n", allArticlesNum);

	long uniqueArticlesNum = unique_articles(qs);
	printf("uniqueArticles: %ld\n", uniqueArticlesNum);

	long allRevisionsNum = all_revisions(qs);
	printf("allRevisions:  %ld\n", allRevisionsNum);

	// -------------------------------------------------------------

	// Testar article_title ------------

	char *title_query;

	printf("\nIs article 10 named AccessibleComputing?\n");
	title_query = article_title((long) 10, qs);
	printf("Answer: %s\n", title_query);

	printf("Is article 12 named Anarchism?\n");
	title_query = article_title((long) 12, qs);
	printf("Answer: %s\n", title_query);


	// -------------------------------------------------------------

	/*
	// Testar contadores de palavras e bytes ------------

	long nWords;
	long size;

	nWords = get_article_nWords((long) 10, qs);
	size = get_article_size((long) 10, qs);
	printf("\nNum of Words of article 10 is: %ld\n", nWords);
	printf("Size of article 10 is: %ld\n", size);

	nWords = get_article_nWords((long) 12, qs);
	size = get_article_size((long) 12, qs);
	printf("\nNum of Words of article 10 is: %ld\n", nWords);
	printf("Size of article 10 is: %ld\n", size);


	// -------------------------------------------------------------
	*/


	// Testar article_timestamp ------------

	char *time_query;

	printf("\nIs article 10, revision 631144794, timestamped 2014-10-26T04:50:23Z ?\n");
	time_query = article_timestamp((long) 10, (long) 631144794, qs);
	printf("Answer: %s\n", time_query);

	printf("Is article 12, revision 751573634, timestamped 2016-11-26T15:18:43Z ?\n");
	time_query = article_timestamp((long) 12, (long) 751573634, qs);
	printf("Answer: %s\n", time_query);


	// -------------------------------------------------------------

	// Testar top 20 articles ------------

	printf("\nTop 20 articles:\n");

	long *top20Articles = top_20_largest_articles(qs);

	int index;
	for (index = 0; index < 20; index++) {
		printf("%d: %ld (%s)\n", (index + 1), top20Articles[index], article_title(top20Articles[index], qs));
	}

	free(top20Articles);

	// ---------------------------------------

	// Testar top N more words ------------ IS IT WORKING- GOTTA 64 BITS THAT

	int numTop = 30;
	printf("\nTop %d articles with more words:\n", numTop);
	long *topNArticles = top_N_articles_with_more_words(numTop, qs);

	int ind;
	for (ind = 0; ind < numTop; ind++) {
		printf("%d: %ld (%s)\n", (ind + 1), topNArticles[ind], article_title(topNArticles[ind], qs));
	}

	free(topNArticles);

	// ---------------------------------------


	printf("\nCalling clean()\n");

	qs = clean(qs);

	// Terminar timer

	clock_t execEnd = clock();
	float timeElapsed = (float)(execEnd - execStart) / CLOCKS_PER_SEC;

	printf("\nAll done. Time elapsed: %f seconds\n", timeElapsed);

	return 0;
}