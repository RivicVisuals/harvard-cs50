// Implements a dictionary's functionality
// using Tries
// Trie implementation courtesy of https://www.techiedelight.com/trie-implementation-insert-search-delete/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

#define CHARS 27

// Represents a node in a hash table
typedef struct node
{
    bool end;
    struct node *next[CHARS];
}
node;

// Prototypes
void storeNode(char *word);
void destroy(node *list);

// keeps track of the word count in the loaded dictionary
unsigned int wordCount = 0;

// Hash table
node *table;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *tmp = table;

    while (*word)
    {
        tmp = tmp->next[tolower(*word) - 'a'];
        if (tmp == NULL)
        {
            return false;
        }
        word++;
    }

    if (tmp->end)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    return 0;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // initialise new node
    node *n = calloc(1, sizeof(node));

    // insert node at the top
    table = n;

    // Try to open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char currentWord[LENGTH + 1];

    while (feof(dict) == 0)
    {
        // read word wise, check for end of file
        fscanf(dict, "%s", currentWord);
        if (feof(dict))
        {
            break;
        }

        storeNode(currentWord);

        // increment wordCount
        wordCount++;
    }

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
    // recursive optimisation for unload
    destroy(table);
    return true;
}

// Stores given node in trie linked list
void storeNode(char* word)
{
    node *tmp = table;

    while (*word)
    {
        int index = tolower(*word) - 'a';
        // Path does not exist yet, create a new node for the current char
        if (tmp->next[index] == NULL)
        {
            // initialise new node
            node *n = calloc(1, sizeof(node));

            // insert node at the correct next pointer
            tmp->next[index] = n;
        }

        // go to next character
        tmp = tmp->next[index];
        word++;
    }

    // mark whole word
    tmp->end = true;
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
        for (int i = 0; i < CHARS; i++)
        {
            // use a traversal pointer
            node *tmp = list->next[i];
            destroy(tmp);
        }
    }
    // we're the last node
    free(list);
    return;
}