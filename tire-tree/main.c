#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// trie node
struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];

    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode* getNode(void)
{
    struct TrieNode* pNode = NULL;
    pNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (pNode)
    {
        int i;
        pNode->isEndOfWord = false;
        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode* root, const char* key)
{

    int level;
    int length = strlen(key);
    int index;
    struct TrieNode* pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}

void insert_file(char* input_file_name, struct TrieNode* root)
{
    FILE* fptr = fopen(input_file_name, "r");
    while (!feof(fptr))
    {
        int size = 1;
        char ch = fgetc(fptr);
        char* ptr = (char*)malloc(sizeof(char) * size);
        for (; ch != '\n'; )
        {
            ptr[size] = ch;
            size++;
            ptr = (char*)realloc(ptr, sizeof(char) * size);
            ch = fgetc(fptr);
        }
        insert(root, ptr);
        free(ptr);
    }
    fclose(fptr);
}

// Returns true if key presents in trie, else false
void search(struct TrieNode* root, char* input_file_name, char* output_file_name )
{
    FILE* ifptr = fopen(input_file_name, "r");
    FILE* ofptr = fopen(output_file_name, "w");
    char ch = fgetc(ifptr);
    int wordLength = 0;
    int pLength = 0;
    char* buffer = (char*)malloc(sizeof(char) * 45);
    int index = CHAR_TO_INDEX(ch);
    while (!feof(ifptr))
    {
        int length = 0;
        bool isWord = false;
        struct TrieNode* pCrawl = root;
        if (!pCrawl->children[index])
        {
            buffer[length] = ch;
            if (pCrawl->isEndOfWord)
            {
                isWord = true;
                wordLength = length;
            }
            length++;
            pCrawl = pCrawl->children[index];
            if (pLength >= length) index = CHAR_TO_INDEX(buffer[length]);
            else ch = fgetc(ifptr);
        }
        else
        {
            if (isWord)
            {
                fprintf(ofptr, "%s", "**");
                for (int i = 0; i < wordLength; i++)
                {
                    fprintf(ofptr, "%c", buffer[i]);
                }
                fprintf(ofptr, "%s", "**");
                for (int i = 0; i < (length - wordLength); i++) {
                    buffer[i] = buffer[wordLength + i];
                }
                pLength = length;
            }
            else
            {
                for (int i = 0; i < length; i++)
                {
                    fprintf(ofptr, "%c", buffer[i]);
                    buffer[i] = 0;
                }
                pLength = 0;
            }
        }
    }
    fclose(ifptr);
    fclose(ofptr);
}

// Driver
int main()
{
    // Input keys (use only 'a' through 'z' and lower case)
    char keys[][8] = { "the", "a", "there", "answer", "any",
                       "by", "bye", "their" };

    char output[][32] = { "Not present in trie", "Present in trie" };


    struct TrieNode* root = getNode();

    // Construct trie
    int i;
    for (i = 0; i < ARRAY_SIZE(keys); i++)
        insert(root, keys[i]);

    // Search for different keys
    //printf("%s --- %s\n", "the", output[search(root, "the")]);
    //printf("%s --- %s\n", "these", output[search(root, "these")]);
    //printf("%s --- %s\n", "their", output[search(root, "their")]);
    //printf("%s --- %s\n", "thaw", output[search(root, "thaw")]);

    return 0;
}
