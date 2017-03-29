#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// tamanho de articleTitle é 255 a Wiki só permite até 85 caract no nome do user


struct hash *hashTable = NULL;
int eleCount = 0;

struct node {
    int articleId, articleSize;
    char articleTitle[255];
    struct node *next;
};

struct hash {
    struct node *head;
    int count;
};

struct node * createNode(int articleId, char *articleTitle, int articleSize) {
    struct node *newnode;
    newnode = (struct node *) malloc(sizeof(struct node));
    newnode->articleId = articleId;
    newnode->articleSize = articleSize;
    strcpy(newnode->articleTitle, articleTitle);
    newnode->next = NULL;
    return newnode;
}

void insertToHash(int articleId, char *articleTitle, int articleSize) {
    int hashIndex = articleId % eleCount;
    struct node *newnode = createNode(articleId, articleTitle, articleSize);
    /* head of list for the bucket with index "hashIndex" */
    if (!hashTable[hashIndex].head) {
        hashTable[hashIndex].head = newnode;
        hashTable[hashIndex].count = 1;
        return;
    }
    /* adding new node to the list */
    newnode->next = (hashTable[hashIndex].head);
    /*
     * update the head of the list and no of
     * nodes in the current bucket
     */
    hashTable[hashIndex].head = newnode;
    hashTable[hashIndex].count++;
    return;
}

void deleteFromHash(int articleId) {
    /* find the bucket using hash index */
    int hashIndex = articleId % eleCount, flag = 0;
    struct node *temp, *myNode;
    /* get the list head from current bucket */
    myNode = hashTable[hashIndex].head;
    if (!myNode) {
        printf("Given data is not present in hash Table!!\n");
        return;
    }
    temp = myNode;
    while (myNode != NULL) {
        /* delete the node with given articleId */
        if (myNode->articleId == articleId) {
            flag = 1;
            if (myNode == hashTable[hashIndex].head)
                hashTable[hashIndex].head = myNode->next;
            else
                temp->next = myNode->next;

            hashTable[hashIndex].count--;
            free(myNode);
            break;
        }
        temp = myNode;
        myNode = myNode->next;
    }
    if (flag)
        printf("Data deleted successfully from Hash Table\n");
    else
        printf("Given data is not present in hash Table!!!!\n");
    return;
}

void searchInHash(int articleId) {
    int hashIndex = articleId % eleCount, flag = 0;
    struct node *myNode;
    myNode = hashTable[hashIndex].head;
    if (!myNode) {
        printf("Search element unavailable in hash table\n");
        return;
    }
    while (myNode != NULL) {
        if (myNode->articleId == articleId) {
            printf("UserID  : %d\n", myNode->articleId);
            printf("articleTitle     : %s\n", myNode->articleTitle);
            printf("articleSize      : %d\n", myNode->articleSize);
            flag = 1;
            break;
        }
        myNode = myNode->next;
    }
    if (!flag)
        printf("Search element unavailable in hash table\n");
    return;
}

void display() {
    struct node *myNode;
    int i;
    for (i = 0; i < eleCount; i++) {
        if (hashTable[i].count == 0)
            continue;
        myNode = hashTable[i].head;
        if (!myNode)
            continue;
        printf("\nData at index %d in Hash Table:\n", i);
        printf("ArticleID      Title                         Size \n");
        printf("---------------------------------------------------\n");
        while (myNode != NULL) {
            printf("%-15d", myNode->articleId);
            printf("%-30s", myNode->articleTitle);
            printf("%d\n", myNode->articleSize);
            myNode = myNode->next;
        }
    }
    return;
}

int main() {
    int n, ch, articleId, articleSize;
    char articleTitle[255];
    printf("Enter the number of elements:");
    scanf("%d", &n);
    eleCount = n;
    /* create hash table with "n" no of buckets */
    hashTable = (struct hash *) calloc(n, sizeof(struct hash));
    while (1) {
        printf("\n1. Insertion\t2. Deletion\n");
        printf("3. Searching\t4. Display\n5. Exit\n");
        printf("Enter your choice:");
        scanf("%d", &ch);
        switch (ch) {
        case 1:
            printf("Enter the articleId value:");
            scanf("%d", &articleId);
            getchar();
            printf("articleTitle:");
            fgets(articleTitle, 255, stdin);
            articleTitle[strlen(articleTitle) - 1] = '\0';
            printf("articleSize:");
            scanf("%d", &articleSize);
            /*inserting new node to hash table */
            insertToHash(articleId, articleTitle, articleSize);
            break;

        case 2:
            printf("Enter the articleId to perform deletion:");
            scanf("%d", &articleId);
            /* delete node with "articleId" from hash table */
            deleteFromHash(articleId);
            break;

        case 3:
            printf("Enter the articleId to search:");
            scanf("%d", &articleId);
            searchInHash(articleId);
            break;
        case 4:
            display();
            break;
        case 5:
            exit(0);
        default:
            printf("U have entered wrong option!!\n");
            break;
        }
    }
    return 0;
}
