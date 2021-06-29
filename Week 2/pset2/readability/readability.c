
#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


int i;
int n;
float letters;
float words;
float sentences;
float L;
float S;
float index;


int main(void)

{

    // input string from user
    string str = get_string("Text: ");

    words = 1;


    // finding numbers of letters, words and sentences
    for (i = 0, n = strlen(str); i < n; i++)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
        {
            letters++;
        }
        else if (str[i] == 32)
        {
            words++;
        }
        else if ((str[i] == 33) || (str[i] == 46) || (str[i] == 63))
        {
            sentences++;
        }
    }


    // calculating index
    L = (letters * 100) / words;

    S = (sentences * 100) / words;

    index = roundf(0.0588 * L - 0.296 * S - 15.8);


    // printing results
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %1.f\n", index);
    }


}