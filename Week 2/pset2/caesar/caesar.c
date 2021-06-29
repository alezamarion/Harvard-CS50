
#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int i;
int n;
string key;
int num_key;


int main(int argc, string argv[])
{
    // checking single argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    // getting key 
    key = argv[1];

    // checking if key is valid
    for (i = 0, n = strlen(key); i < n; i++)
    {
        if ((key[i] < 48) || (key[i] > 57))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    for (i = 0, n = strlen(key); i < n; i++)
    {
        if ((key[i] > 47) || (key[i] < 58))
        {
            num_key = atoi(key);                        // converting valid key to integer
        }
    }

    // getting plaintext
    string plaintext = get_string("plaintext: ");
    
    // starting printing answer
    printf("ciphertext: ");

    // changing plaintext to ciphertext
    for (i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                printf("%c", (((plaintext[i] - 65) + num_key) % 26) + 65);
            }
            if (islower(plaintext[i]))
            {
                printf("%c", (((plaintext[i] - 97) + num_key) % 26) + 97);
            }
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}
