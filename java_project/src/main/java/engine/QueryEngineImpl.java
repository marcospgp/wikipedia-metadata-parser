package engine;

import li3.Interface;

import java.util.ArrayList;

public class QueryEngineImpl implements Interface {


    public void init() {

    	HashMap<Long, Article> articles = new HashMap<Long, Article>();
    	HashMap<Long, User> users = new HashMap<Long, User>();
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

        return new ArrayList<Long>();
    }

    public String contributor_name(long contributor_id) {

        return Users.getContributorName(users, contributor_id);
    }

    public ArrayList<Long> top_20_largest_articles() {

        return new ArrayList<Long>();
    }

    public String article_title(long article_id) {

        return Articles.get_article_title(article_id, articles);
    }

    public ArrayList<Long> top_N_articles_with_more_words(int n) {

        return new ArrayList<Long>();
    }

    public ArrayList<String> titles_with_prefix(String prefix) {

        return new ArrayList<String>();
    }

    public String article_timestamp(long article_id, long revision_id) {

        return Articles.get_article_timestamp(article_id, revision_id, articles);
    }

    public void clean() {

    	articles.clear();
    	users.clear();
    }
}
