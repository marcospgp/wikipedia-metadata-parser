public class Article {
    long id, size, nWords;
    String title;
    HashMap<long, Revision> revisions;


    public Article() {
        this.id = null;
        this.size = null;
        this.nWords = null;
        this.title = null;
        this.revisions = new HashMap<long, Revision>();
    }

    public long getArticleId() {
        return id;
    }
    public void setArticleId(long newId) {
        this.id = newId;
    }

    public long getArticleSize() {
        return size;
    }
    public void setArticleSize(long newSize) {
        this.size = newSize;
    }

    public long getArticleNWords() {
        return nWords;
    }
    public void setArticleNWords(long newNWords) {
        this.nWords = newNWords;
    }

    public String getArticleTitle() {
        return title;
    }
    public void setArticleTitle(String newTitle) {
        this.title = newTitle;
    }

    public HashMap<long, Revision> getRevisionsHash() {
        return this.revisions;
    }


    @Override
    public String toString() {
        return "Article::" +
                " ID=" + this.id +
                " Title=" + this.title +
                " Size=" + this.size +
                " NWords=" + this.nWords +
                "\nRevisions:" + this.revisions.toString();
    }

}