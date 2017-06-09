package engine;

import java.util.*;

public class Article {
	private long id, size, nWords;
	private String title;
	private HashMap<Long, Revision> revisions;

	public Article() {
		this.id = 0L;
		this.size = 0L;
		this.nWords = 0L;
		this.title = "";
		this.revisions = new HashMap<Long, Revision>();
	}

	public Article(long id, long size, long nWords, String title) {
		this.id = id;
		this.size = size;
		this.nWords = nWords;
		this.title = title;
		this.revisions = new HashMap<Long, Revision>();
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

	public HashMap<Long, Revision> getRevisionsHash() {
		return this.revisions;
	}

	public void addRevision(long id, Revision rev) {
		this.revisions.put(id, rev);
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
