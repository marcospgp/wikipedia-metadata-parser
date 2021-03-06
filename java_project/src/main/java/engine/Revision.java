package engine;

public class Revision {
    private long id, parentId;
    private String timestamp;

    public Revision() {
        this.id = 0L;
        this.parentId = 0L;
        this.timestamp = "";
    }

    public Revision(long id, long parentId, String timestamp) {
        this.id = id;
        this.parentId = parentId;
        this.timestamp = timestamp;
    }

    public long getRevisionId() {
        return id;
    }
    public void setRevisionId(long newId) {
        this.id = newId;
    }

    public long getRevisionParentId() {
        return parentId;
    }
    public void setRevisionParentId(long newParentId) {
        this.parentId = newParentId;
    }

    public String getTimestamp() {
        return timestamp;
    }
    public void setTimestamp(String newTimestamp) {
        this.timestamp = newTimestamp;
    }


    @Override
    public String toString() {
        return "Revision::" +
                " ID=" + this.id +
                " Timestamp=" + this.timestamp +
                " Parent ID=" + this.parentId;
    }

}