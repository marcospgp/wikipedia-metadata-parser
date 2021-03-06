#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "parser.h"
#include "hashtable.h"
#include "users.h"
#include "articles.h"

#include "interface.h"

TAD_istruct init() {

	TAD_istruct qs = malloc(sizeof(struct TCD_istruct));

	qs->users = NULL;
	qs->articles = NULL;
	qs->allArticles = 0;
	qs->uniqueArticles = 0;
	qs->allRevisions = 0;

	// Inicializar hash tables
	qs = initHashtables(qs);

	return qs;
}


TAD_istruct load(TAD_istruct qs, int nsnaps, char* snaps_paths[]) {

	int i;
	for (i = 0; i < nsnaps; i++) {

		qs = parseWikiData(qs, snaps_paths[i]);

		if (qs == NULL) {
			fprintf(stderr, "Erro ao processar dados do caminho %d\n", (i + 1));
		}
	}

	return qs;
}


TAD_istruct clean(TAD_istruct qs) {
	return clean_everything(qs);
}


long all_articles(TAD_istruct qs) {
	return get_all_articles(qs);
}


long unique_articles(TAD_istruct qs) {
	return get_unique_articles(qs);
}


long all_revisions(TAD_istruct qs) {
	return get_all_revisions(qs);
}



long* top_10_contributors(TAD_istruct qs) {
	return getTop10Contributors(qs);
}


char* contributor_name(long contributor_id, TAD_istruct qs) {
	return getContributorName(qs, contributor_id);
}


long* top_20_largest_articles(TAD_istruct qs) {
	return getTop20LargestArticles(qs);
}


char* article_title(long article_id, TAD_istruct qs) {
	return get_article_title(article_id, qs);
}


long* top_N_articles_with_more_words(int n, TAD_istruct qs) {
	return getTopNArticlesWithMoreWords(n, qs);
}


char** titles_with_prefix(char* prefix, TAD_istruct qs) {
	return getTitlesWithPrefix(prefix, qs);
}


char* article_timestamp(long article_id, long revision_id, TAD_istruct qs) {
	return get_article_timestamp(article_id, revision_id, qs);
}
