#include "interface.h"

int main(int argc, char const *argv[]) {

TAD_istruct estrutura;

estrutura = init();
load(estrutura, int nsnaps, char* snaps_paths[]);

all_articles(TAD_istruct estrutura);
unique_articles(TAD_istruct estrutura);
all_revisions(TAD_istruct estrutura);
top_10_contributors(TAD_istruct estrutura);
contributor_name(long contributor_id, TAD_istruct estrutura);
top_20_largest_articles(TAD_istruct estrutura);
article_title(long article_id, TAD_istruct estrutura);
top_N_articles_with_more_words(int n, TAD_istruct estrutura);
titles_with_prefix(char* prefix, TAD_istruct estrutura);
article_timestamp(long article_id, long revision_id, TAD_istruct estrutura);

clean(estrutura);

	return 1;
}