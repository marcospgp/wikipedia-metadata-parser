public class Article {
    Long id, size, nWords;
    String title;
    HashMap<Long, Revision> revisions;


    public Article() {
        this.id = null;
        this.size = null;
        this.nWords = null;
        this.title = null;
        this.revisions = new HashMap<Long, Revision>();
    }

    public Long getArticleId() {
        return id;
    }
    public void setArticleId(Long newId) {
        this.id = newId;
    }

    public Long getArticleSize() {
        return size;
    }
    public void setArticleSize(Long newSize) {
        this.size = newSize;
    }

    public Long getArticleNWords() {
        return nWords;
    }
    public void setArticleNWords(Long newNWords) {
        this.nWords = newNWords;
    }

    public String getArticleTitle() {
        return title;
    }
    public void setArticleTitle(String newTitle) {
        this.title = newTitle;
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