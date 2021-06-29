
#include <stdio.h>
#include <cs50.h>
#include <math.h>


int main(void)

{
    float i;                                        // declaring variables
    int change;
    int quarters;
    int remainder_quarters;
    int dimes;
    int remainder_dimes;
    int nickels;
    int remainder_nickels;
    int pennies;
    int coins;

    do
    {
        i = get_float("Change owed: ");             // getting change
    }
    while (i <= 0);


    change = round(i * 100);                        // rounding to cents

    quarters = change / 25;                         // finding number of quarters
    remainder_quarters = change % 25;
    dimes = remainder_quarters / 10;                // finding number of dimes
    remainder_dimes = remainder_quarters % 10;
    nickels = remainder_dimes / 5;                  // finding number of nickels
    remainder_nickels = remainder_dimes % 5;
    pennies = remainder_nickels / 1;                // finding number of pennies
    
    coins = quarters + dimes + nickels + pennies;
    
    printf("%i\n", coins);

}
