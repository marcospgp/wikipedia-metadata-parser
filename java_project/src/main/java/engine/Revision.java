public class Revision {
    long id, parentId;
    String timestamp;


    public Revision() {
        this.id = 0L;
        this.timestamp = "";
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
                " Timestamp=" + this.timestamp;
    }

}