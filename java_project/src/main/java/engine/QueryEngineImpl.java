package engine;

import li3.Interface;

import java.util.*;

public class QueryEngineImpl implements Interface {

	private HashMap<Long, Article> articles;
    private HashMap<Long, User> users;


    public void init() {

    	articles = new HashMap<Long, Article>();
    	users = new HashMap<Long, User>();
    }

    public void load(int nsnaps, ArrayList<String> snaps_paths) {

    	Parser.parseWikiData(articles, users, nsnaps, snaps_paths);
    }

    public long all_articles() {

        return Articles.get_all_articles();
    }

    public long unique_articles() {

        return Articles.get_unique_articles();
    }

    public long all_revisions() {

        return Articles.get_all_revisions();
    }

    public ArrayList<Long> top_10_contributors() {

        return Users.getTop_10_contributors(users);
    }

    public String contributor_name(long contributor_id) {

        return Users.getContributorName(users, contributor_id);
    }

    public ArrayList<Long> top_20_largest_articles() {

        return Articles.getTop_20_largest_articles(articles);
    }

    public String article_title(long article_id) {

        return Articles.get_article_title(article_id, articles);
    }

    public ArrayList<Long> top_N_articles_with_more_words(int n) {

        return Articles.getTop_N_largest_articles(articles,n);
    }

    public ArrayList<String> titles_with_prefix(String prefix) {

        return Articles.getTitlesWithPrefix(articles,prefix);
    }

    public String article_timestamp(long article_id, long revision_id) {

        return Articles.get_article_timestamp(article_id, revision_id, articles);
    }

    public void clean() {

    	articles.clear();
    	users.clear();
    }
}
