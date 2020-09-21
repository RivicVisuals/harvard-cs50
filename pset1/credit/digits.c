#include <stdio.h>
#include <cs50.h>

int countDigits(long n);
int extractDigit(long ccNumber, int place);
long expo(long base, int exponent);
int checksum(long number, int digits);

long number;


int main(void) {
    number = get_long("Number: ");
    int digits = countDigits(number);

    printf("%i digits in number %li\n", digits, number);
    printf("The first two digits are: %i%i\n", extractDigit(number, digits), extractDigit(number, digits - 1));
    printf("Checksum: %i\n", checksum(number, digits));
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

int extractDigit(long ccNumber, int place)
{
    long exponent = 1;

    for (int i = 0; i < place; i++)
    {
        exponent *= 10;
    }
    return (ccNumber % exponent - ccNumber % (exponent / 10)) / (exponent / 10);
}

int checksum(long n, int digits)
{
    int sum = 0;

    for (int i = 1; i <= digits; i++) // deliberately start at 1 to expressively show even / odd digits
    {
        int currentDigit = extractDigit(n, i);

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
            } else
            {
                // add product to running sum
                sum += currentDigit;
            }
        } else
        {
            // add odd digit to running sum
            sum += currentDigit;
        }
    }

    return sum;
}