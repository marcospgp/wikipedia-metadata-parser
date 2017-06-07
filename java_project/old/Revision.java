public class Revision {
    long id;
    String timestamp;


    public Revision() {
        this.id = null;
        this.timestamp = null;
    }


    public long getRevisionId() {
        return id;
    }
    public void setRevisionId(long newId) {
        this.id = newId;
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