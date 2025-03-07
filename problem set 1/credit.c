#include <stdio.h>
#include <cs50.h>
#include <math.h>

string getCardType();
int getCardSum();


int main(void)
{
    // Request card number from user
    printf("Number: ");
    long long cardNum = get_long_long();

    // Calculate the sum based on the card number
    int cardSum = getCardSum(cardNum);

    // The mod10 of the sum of a valid card number is always 0
    if (cardSum % 10 == 0)
        printf("%s\n", getCardType(cardNum));
    else
        printf("INVALID\n");

    return 0;
}

// Returns a card company name based on card number
string getCardType(long long cardNum)
{
    if (cardNum >= 5100000000000000 && cardNum < 5600000000000000)
        return "MASTERCARD\n";

    else if ((cardNum >= 4000000000000 && cardNum < 5000000000000) || (cardNum >= 4000000000000000 && cardNum < 5000000000000000))
        return "VISA\n";

    else if ((cardNum >= 340000000000000 && cardNum < 350000000000000) || (cardNum >= 370000000000000 && cardNum < 380000000000000))
        return "AMEX\n";
    else
        return "INVALID\n";
}

// Returns a processed total based on the card number
int getCardSum(long long cardNum)
{
    int sumOdd = 0;
    int sumEven = 0;
    int x1, x2;

    // Odd positions summed up in sumOdd
    for (int i = 1; i <= 16; i = i + 2)
    {
        long long cardNumNew = cardNum / pow(10,16-i);
        long long cardNumNew1 = cardNumNew * 10;
        long long cardNumNew2 = cardNum / pow(10,15-i);
        long long y = cardNumNew2 - cardNumNew1;

        sumOdd = sumOdd + y;
    }

    // Even positions summed up according to Luhn algorithm
    for (int i = 0; i <= 15; i = i + 2)
    {
        // Even positions first multiplied by 2
        long long cardNumNew = cardNum / pow(10,16-i);
        long long cardNumNew1 = cardNumNew * 10;
        long long cardNumNew2 = cardNum / pow(10,15-i);
        long long x = 2 * (cardNumNew2 - cardNumNew1);

        // Any resulting double digits are split into two digitsseparated into their two constituent digits
        if (x >= 10)
        {
            x1 = x / 10;
            x2 = x - 10;
            x = x1 + x2;
        }

        sumEven = sumEven + x;
    }

    return sumOdd + sumEven;
}
