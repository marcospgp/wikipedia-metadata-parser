public class User {
    Long id, contributions;
    String username;


    public User() {
        this.id = null;
        this.contributions = 0L;
        this.username = null;
    }


    public Long getUserId() {
        return id;
    }
    public void setUserId(Long newId) {
        this.id = newId;
    }

    public Long getUserContributions() {
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