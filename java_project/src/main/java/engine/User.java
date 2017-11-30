package engine;

public class User {
    private long id, contributions;
    private String username;


    public User() {
        this.id = 0L;
        this.contributions = 0L;
        this.username = "";
    }


    public long getUserId() {
        return id;
    }
    public void setUserId(long newId) {
        this.id = newId;
    }

    public long getUserContributions() {
        return contributions;
    }
    public void addUserContribution() {
        this.contributions++;
    }

    public String getUsername() {
        return username;
    }
    public void setUsername(String newUsername) {
        this.username = newUsername;
    }


    @Override
    public String toString() {
        return "User::" +
                " ID=" + this.id +
                " Username=" + this.username +
                " Contributions=" + this.contributions;
    }

}