#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode {
    struct TrieNode *children[26]; // Array to store links to children nodes
    int isEndOfWord; // Flag to indicate end of a word
    int count; // Counter to store the number of occurrences of the word
};

// Creates a new Trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->isEndOfWord = 0;
        newNode->count = 0;
        for (int i = 0; i < 26; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Inserts a word into the Trie
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *temp = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!temp->children[index]) {
            temp->children[index] = createTrieNode();
        }
        temp = temp->children[index];
    }
    temp->isEndOfWord = 1;
    temp->count++;
}

// Searches for a word in the Trie and returns its count if found
int numberOfOccurances(struct TrieNode *root, char *word) {
    struct TrieNode *temp = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!temp->children[index]) {
            return 0; // Word not found
        }
        temp = temp->children[index];
    }
    if (temp != NULL && temp->isEndOfWord) {
        return temp->count;
    }
    return 0; // Word not found
}

// Deallocates memory for the Trie
void deallocateTrie(struct TrieNode *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        if (root->children[i] != NULL) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads the dictionary from a file and stores the words in the array
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }
    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    // Create a Trie and insert words from the dictionary
    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }

    // Test the Trie with some words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(root, pWords[i]));
    }

    // Deallocate the Trie
    deallocateTrie(root);

    return 0;
}
