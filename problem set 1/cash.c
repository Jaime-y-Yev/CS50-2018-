#include <stdio.h>
#include <cs50.h>
#include <math.h>


int main(void)
{
    // Keeps requesting a positive dollar amount (X.XX or X.X)
    printf("Input a dollar amount eg, X.XX or X.X: ");
    float dollarCents = get_float();

    while (dollarCents < 0)
    {
        printf("Input a positive value: ");
        dollarCents = get_float();
    }

    int cents = round(dollarCents * 100);       // convert to pure cents


    // Find number of quarters, dimes, and nickels (in that order)
    int numCoins = 0;
    int remain = cents;
    int totalCoins = 0;

    // Each loop extracts amount of quarters, dimes, and nickels (pennies processed after the loop)
    for (int i = 0; i < 3; i++)
    {
        int weirdCalculation = (25 - i * (20 - i * 5));

        numCoins = remain / weirdCalculation;

        remain = remain - numCoins * weirdCalculation;   // subtracts amount and updates remaining amount

        totalCoins = totalCoins + numCoins;                     // keeps count of number of coins extracted
    }

    // adds final remainder (pennies) to coin counter and prints final total coin count
    totalCoins = totalCoins + remain;
    printf("%i\n", totalCoins);
}