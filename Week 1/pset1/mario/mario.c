#include <stdio.h>
#include <cs50.h>

int main(void)
{

    int n;

    do
    {
        n = get_int("Height: ");            //get number
    }
    while (n < 1 || n > 8);                 // numbers between 1 and 8


    for (int g = 0; g < n; g++)             // first loop: line
    {
        for (int h = n - 1; h > g; h--)     //second loop: spaces
        {
            printf(" ");
        }
        for (int i = 0; i <= g; i++)        //third loop: hashes
        {
            printf("#");
        }
        printf("\n");
    }

}


