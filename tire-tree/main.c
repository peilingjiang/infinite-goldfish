#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// trie node
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
    int count;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void) {
    struct TrieNode *pNode = NULL;
    pNode = (struct TrieNode *) malloc(sizeof(struct TrieNode));
    if (pNode) {
        int i;
        pNode->isEndOfWord = false;
        pNode->count = 0;
        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, char *key) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = true;
}

void insert_file(char *input_file_name, struct TrieNode *root) {
    FILE *fptr = fopen(input_file_name, "r");
    if (!fptr) assert("dictionary not valid");
    while (!feof(fptr)) {
        char ch = fgetc(fptr);
        char *ptr = (char *) malloc(sizeof(char));
        int i = 0;

        for (; !feof(fptr) && ch != '\n'; i++) {
            ptr = (char *) realloc(ptr, sizeof(char) * (i + 1));
            ptr[i] = ch;
            ch = fgetc(fptr);
        }
        insert(root, ptr);
        for (int j = 0; j <= i; j++) {
            ptr[j] = 0;
        }
        free(ptr);
    }
    fclose(fptr);
}

bool testTree(struct TrieNode *root, const char *key) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

void count(struct TrieNode *root, const char *key, int *count) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = CHAR_TO_INDEX(key[level]);
        pCrawl = pCrawl->children[index];
    }

    *count = pCrawl->count;
    pCrawl->count = 0;
}

void search(struct TrieNode *root, char *input_file_name, char *output_file_name, char *word_file_name) {
    FILE *ifptr = fopen(input_file_name, "r");
    FILE *ofptr = fopen(output_file_name, "w");
    FILE *wfptr = fopen(word_file_name, "w");
    char ch = fgetc(ifptr);
    int pLength = 0;
    char *buffer = (char *) malloc(sizeof(char) * 45);
    for (int i = 0; i < 45; i++) buffer[i] = 0;
    buffer[0] = ch;
    int index = 0;
    struct TrieNode *pCrawl;

    while (!feof(ifptr)) {
        index = CHAR_TO_INDEX(buffer[0]);
        int wordLength = 1;
        int length = 0;
        bool isWord = false;
        pCrawl = root;

        while (pCrawl->children[index]) {
            length++;
            pCrawl = pCrawl->children[index];
            if (pCrawl->isEndOfWord) {
                wordLength = length;
                isWord = true;
                (pCrawl->count)++;
            }
            if (length < pLength) {
                index = CHAR_TO_INDEX(buffer[length]);
            } else {
                ch = fgetc(ifptr);
                index = CHAR_TO_INDEX(ch);
                buffer[length] = ch;
            }
        }
        pLength = 0;
        if (isWord) {
            fprintf(ofptr, "%s", " **");
            for (int i = 0; i < wordLength; i++) {
                fprintf(ofptr, "%c", buffer[i]);
                fprintf(wfptr, "%c", buffer[i]);
            }
            fprintf(ofptr, "%s", "** ");
            fprintf(wfptr, "%c", ' ');
            for (int i = 0; i < 45 - wordLength; i++) {
                buffer[i] = buffer[i + wordLength];
                if (buffer[i]) pLength++;
            }
        } else {
            fprintf(ofptr, "%c", buffer[0]);
            for (int i = 0; i < 45; i++) {
                buffer[i] = buffer[i + 1];
                if (buffer[i] != NULL) pLength++;
            }
        }
    }
    fclose(ifptr);
    fclose(ofptr);
    fclose(wfptr);
}

void countWord(struct TrieNode *root, char *word_file_name, char *word_count) {
    FILE *ofptr = fopen(word_file_name, "r");
    FILE *fptr = fopen(word_count, "w");
    if (!ofptr) assert("word_appear file not valid");
    while (!feof(ofptr)) {
        char ch = fgetc(ofptr);
        char *ptr = (char *) malloc(sizeof(char));
        int i = 0;
        int *num = (int *) malloc(sizeof(int));

        for (; !feof(ofptr) && ch != ' '; i++) {
            ptr = (char *) realloc(ptr, sizeof(char) * (i + 1));
            ptr[i] = ch;
            ch = fgetc(ofptr);
        }
        count(root, ptr, num);
        if (*num != 0) fprintf(fptr, "%s %d\n", ptr, *num);
        for (int j = 0; j <= i; j++) {
            ptr[j] = 0;
        }
        free(ptr);
    }
    fclose(ofptr);
    fclose(fptr);
}

int main(int argc, char **argv) {
    struct TrieNode *root = getNode();
    // https://github.com/dolph/dictionary
    insert_file("/Users/wangkeru/Desktop/infinite-goldfish/tire-tree/dictionary_popular.txt", root);
//    char output[][32] = {"Not present in trie", "Present in trie"};
//    printf("%s --- %s\n", "pled", output[testTree(root, "pled")] );
    search(root, "/Users/wangkeru/Desktop/infinite-goldfish/tire-tree/input.txt", "output.txt", "word_appear.txt");
    countWord(root, "word_appear.txt", "word_count.txt");
    return 0;
}
