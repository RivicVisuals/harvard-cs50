// Implements a dictionary's functionality
// using a hash table

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Prototypes
void storeNode(node *node, unsigned int hash);
void destroy(node *list);

// Number of buckets in hash table
const unsigned int N = 26 * 25 * 24; // for every combination of the first two letters

// keeps track of the word count in the loaded dictionary
unsigned int wordCount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // get hash of the search query
    unsigned int index = hash(word);

    // do a linear search on the nodes at the resulting index
    node *tmp = table[index];

    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // self made algorithm: bible.txt spends ~ 0.60 sec in check
    int len = strlen(word);
    switch (len)
    {
        case 1:
            return tolower(word[0]) % N;
        case 2:
            return (tolower(word[0]) * len + tolower(word[1]) * len) % N;
        default:
            return ((tolower(word[0]) * len + tolower(word[1]) * (len - 1) + tolower(word[2]) * (len - 2)) -
                    (tolower(word[0]) * tolower(word[1]) * tolower(word[2]))) % N;
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Try to open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char currentWord[LENGTH + 1];
    int i = 0;

    do
    {
        int c = fgetc(dict);

        // parse word wise
        if (isalpha(c) || c == '\'')
        {

            currentWord[i] = c;
            i++;

        }
        else if (c == '\n')
        {
            // terminate currentWord and reset character index
            currentWord[i] = '\0';
            i = 0;

            // create nodes for the word
            node *n = calloc(1, sizeof(node)); // calloc gives clean node, shuts valgrind up
            if (n == NULL)
            {
                return false;
            }

            // parse word
            strcpy(n->word, currentWord);

            // get hash value
            unsigned int h = hash(currentWord);

            // store in hashtable
            storeNode(n, h);

            // increment wordCount
            wordCount++;
        }
    }
    while (feof(dict) == 0);

    // close dictionary file
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // check each bucket in the hash table
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL) // Conditional jump or move depends on uninitialised value(s): (file: dictionary.c, line: 134)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;

    // recursive optimisation (?) for unload
    // for (int i = 0; i < N; i++)
    // {
    //     destroy(table[i]);
    // }
    // return true;
}

// Stores given node in hashtable at the appropriate hash index
void storeNode(node *node, unsigned int hash)
{
    // go to table[hash] and point to the same first member, if there is one, then assign table[hash] to point at the new node
    // effectively storing the node at the top
    if (table[hash] == NULL)
    {
        table[hash] = node;
    }
    else
    {
        node->next = table[hash];
        table[hash] = node;
    }
    return;
}

// Destroys given linked list recursively
void destroy(node *list)
{
    // base case
    if (list == NULL)
    {
        return;
    }
    // recursive case
    else
    {
        // use a traversal pointer
        node *tmp = list->next;
        destroy(tmp);
    }
    // we're the last node
    free(list);
    return;
}