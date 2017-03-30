#ifndef hashtable_h
#define hashtable_h

typedef struct __user {
	long id, contributions;
	char username[255];
} user;

typedef struct __article {
	long id, size, words;
	char title[255];
} article;

void init();

void insertOrUpdateUser(long id, char *username, long contributions);

void insertOrUpdateArticle(long id, char *username, long contributions);

#endif
