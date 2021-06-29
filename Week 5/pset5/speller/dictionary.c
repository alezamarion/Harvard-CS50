// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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

// Number of buckets in hash table
const unsigned int N = 143093;

// Hash table
node *table[N];



// Hashes word to a number
unsigned int hash(const char *word)
{
    //Source: Shrey Tripathi, on https://cs50.stackexchange.com/questions
    unsigned int hash_value = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash_value = (hash_value << 2) ^ word[i];
    }
    return hash_value % 143093;
}



//initializes counter
unsigned int word_count = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    //Opens dictionary
    FILE *file = fopen(dictionary, "r");
    
    if (file == NULL)
    {
        return false;
    }
    
    //Scans dictionary word by word populating hash table
    char word[LENGTH + 1];
 
    while (fscanf(file, "%s", word) != EOF)
    {
        //*Mallocs a node for each new word in load
        node *new_node = malloc(sizeof(node));

        //Checks if malloc succeeded
        if (new_node == NULL)
        {
            unload();
            return false;
        }
       
        //Calculating index
        unsigned int h = hash(word);

        //Copies word into node
        strcpy(new_node->word, word);

        //Inserts word into list
        new_node->next = table[h];

        // If bucket is empty, insert first node
        if (table[h] == NULL)
        {
            table[h] = new_node;
        }
        else
        {
            new_node->next = table[h];
            table[h] = new_node;
        }
        word_count++;
    }
    /*
    int buckets = 0;

    for (int i = 0; i < 143093; i++)
    {
        if (table[i] != NULL)
        {
            buckets++;
        }
    }
    printf("Buckets: %i\n", buckets);
    printf("Palavras carregadas: %u\n", word_count);
    exit(0);
    */
    fclose(file);
    return true;
}



// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}



// Returns true if word is in dictionary else false
bool check(const char *word)
{
    
    // Creates lowercase copy to hash it
    int n = strlen(word);
    char word_copy[LENGTH + 1];
    
    for (int i = 0; i < n; i++)
    {
        word_copy[i] = tolower(word[i]);
    }
    word_copy[n] = '\0';
    
    //hashing word again to find bucket
    unsigned int h = hash(word_copy);
    
    if (table[h] == NULL)
    {
        return false;
    }
    else if (table[h] != NULL)
    {
        node *cursor = table[h];
        
        while (cursor != NULL)
        {
            if (strcasecmp(cursor->word, word) == 0)
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }
    }
    return false;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //iterate throught hashtable
    for (int i = 0; i < 143093; i++)
    {
        //seting pointer
        node *cursor = table[i];
        
        //traversing list
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        free(cursor);
    }
    return true;
}  
