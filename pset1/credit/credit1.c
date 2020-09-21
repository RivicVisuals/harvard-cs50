#include <stdio.h>
#include <cs50.h>

int countDigits(long n);
int extractDigit(long number, int place);
int checksum(long number, int digits);
int checkCC(long number);

int main(void)
{
    long number = get_long("Number: ");

    checkCC(number);
    return 0;
}

int countDigits(long n)
{
    int counter = 0;

    do
    {
        n /= 10;
        counter++;
    }
    while (n != 0);

    return counter;
}

int extractDigit(long number, int place)
{
    long exponent = 1;

    for (int i = 0; i < place; i++)
    {
        exponent *= 10;
    }
    return (number % exponent - number % (exponent / 10)) / (exponent / 10);
}

int checksum(long number, int digits)
{
    int sum = 0;

    for (int i = 1; i <= digits; i++) // deliberately start at 1 to expressively show even / odd digits
    {
        int currentDigit = extractDigit(number, i);

        // check wether we have an even or odd digit
        if (i % 2 == 0)
        {
            // multiply even digits by 2
            currentDigit *= 2;

            if (currentDigit > 9)
            {
                // add values of digits to running sum
                for (int j = 1; j <= 2; j++)
                {
                    sum += extractDigit(currentDigit, j);
                }
            }
            else
            {
                // add product to running sum
                sum += currentDigit;
            }
        }
        else
        {
            // add odd digit to running sum
            sum += currentDigit;
        }
    }

    return sum;
}

int checkCC(long number)
{
    int digits = countDigits(number);
    int firstDigit = extractDigit(number, digits);
    int secondDigit = extractDigit(number, digits - 1);

    if (digits < 13 || digits > 16) // entered number is too short or too long to be CC number.
    {
        printf("INVALID\n");
        return 0;
    }
    if (!(firstDigit == 3 || firstDigit == 4 || firstDigit == 5)) // first digit is none of the reserved start digits for CC numbers.
    {
        printf("INVALID\n");
        return 0;
    }
    if (checksum(number, digits) % 10 != 0) // Entered number is possibly a CC number, calculate checksum.
    {
        printf("INVALID\n");
        return 0;
    }
    if (firstDigit == 3 && (secondDigit == 4 || secondDigit == 7)) // check for first two digits, if 34 or 37 pass
    {
        printf("AMEX\n");
    }
    else if (firstDigit == 4)
    {
        printf("VISA\n");
    }
    else if (firstDigit == 5 && (secondDigit >= 1 && secondDigit <= 5)) // check for first two digits, if 51, 52, 53, 54, 55 pass
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n"); // if some number does fall through assume it is invalid.
    }
    return 0;
}