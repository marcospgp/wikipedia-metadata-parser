#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "parser.h"
#include "hashtable.h"

#include "interface.h"

TAD_istruct init() {

	struct TCD_istruct dataStructure = {NULL, NULL, 0, 0, 0};

	TAD_istruct qs = (TAD_istruct) &dataStructure;

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

/*

TAD_istruct clean(TAD_istruct qs) {}

long all_articles(TAD_istruct qs) {}

long unique_articles(TAD_istruct qs) {}

long all_revisions(TAD_istruct qs) {}

long* top_10_contributors(TAD_istruct qs) {}

char* contributor_name(long contributor_id, TAD_istruct qs) {}

long* top_20_largest_articles(TAD_istruct qs) {}

char* article_title(long article_id, TAD_istruct qs) {}

long* top_N_articles_with_more_words(int n, TAD_istruct qs) {}

char** titles_with_prefix(char* prefix, TAD_istruct qs) {}

char* article_timestamp(long article_id, long revision_id, TAD_istruct qs) {}

*/
