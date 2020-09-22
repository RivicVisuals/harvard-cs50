#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <cs50.h>

bool alphaComplete(string s);
void calculateDelta(string s, int deltaLUT[]);
string cipher(string input, int deltaLUT[]);

const char Alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int main(int argc, string argv[])
{
    string key = argv[1];

    if (!(argc == 2))
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    if (strlen(key) < 26 || strlen(key) > 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Loop over key to check each character of passed string for alphabetic characters and for completeness.
    for (int i = 0; i < 26; i++)
    {
        // Check for alphabetic characters
        if (!isalpha(key[i]))
        {
            // if key[i] is not a letter, throw an error.
            printf("Key must contain only characters A-Z.\n");
            return 1;
        }

        key[i] = toupper(key[i]); // Clean user input to all uppercase
    }


    if (!alphaComplete(key))
    {
        printf("Key must contain each individual character A-Z; no duplicates.\n");
        return 1;
    }

    /* CLI Argument passed requirements and is cleaned */

    int deltas[26]; // Create Array of 26 characters for delta calculation to work on

    calculateDelta(key, deltas); // Calculate delta of key against standard alphabet

    string input = get_string("plaintext: ");
    string output = cipher(input, deltas);

    printf("ciphertext: %s\n", output);

    return 0;
}

bool alphaComplete(string s)
{
    /* Checks wether passed string s contains each character of the alphabet exactly once.
     Params: string s
     Params: static char Alphabet[26]
     Returns: bool
    */

    int i, j; // initialize vars only once

    // 0: copy string as to not manipulate the original. because string names are pointers
    char t[26];
    for (i = 0; i < 26; i++)
    {
        t[i] = s[i];
    }

    // 1: sort string s; bubble sort help by http://www.program99.com/2012/10/bubble-sort-without-pointer.html
    char tmp;

    for (i = 0; i < 26; i++) // Pass over string s as many times as there are chars
    {
        for (j = 0; j < 25 - i; j++) // Don't touch rightmost items after they are sorted. stop before \0
        {
            if (t[j] > t[j + 1]) // current letter is bigger than the next one
            {
                // sort to the right
                tmp = t[j]; // hold current letter
                t[j] = t[j + 1]; // copy next letter to current letter
                t[j + 1] = tmp; // replace current letter at next position
            }
        }
    }

    // 2: compare string s to static char Alphabet[26]
    for (i = 0; i < 26; i++)
    {
        if (t[i] != Alphabet[i])
        {
            return false;
        }
    }

    return true;
}

void calculateDelta(string s, int deltaLUT[])
{
    /* Calculates delta of passed string s against standard alphabet and stores it in passed array.
     Params: string s
     Params: int deltaLUT[]
     */

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        deltaLUT[i] = (int) s[i] - (int) Alphabet[i];
    }
}

string cipher(string s, int deltaLUT[])
{
    /* Applies deltas from LUT to each character of the passed string input.
     Keeps interpunctuation and case.
     Params: string input
     Params: int deltaLUT[]
     Returns: string cipherText
     */

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        char currentLetter = toupper(s[i]);
        switch (currentLetter)
        {
            case 'A':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[0] : tolower(s[i] + deltaLUT[0]);
                break;
            case 'B':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[1] : tolower(s[i] + deltaLUT[1]);
                break;
            case 'C':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[2] : tolower(s[i] + deltaLUT[2]);
                break;
            case 'D':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[3] : tolower(s[i] + deltaLUT[3]);
                break;
            case 'E':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[4] : tolower(s[i] + deltaLUT[4]);
                break;
            case 'F':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[5] : tolower(s[i] + deltaLUT[5]);
                break;
            case 'G':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[6] : tolower(s[i] + deltaLUT[6]);
                break;
            case 'H':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[7] : tolower(s[i] + deltaLUT[7]);
                break;
            case 'I':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[8] : tolower(s[i] + deltaLUT[8]);
                break;
            case 'J':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[9] : tolower(s[i] + deltaLUT[9]);
                break;
            case 'K':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[10] : tolower(s[i] + deltaLUT[10]);
                break;
            case 'L':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[11] : tolower(s[i] + deltaLUT[11]);
                break;
            case 'M':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[12] : tolower(s[i] + deltaLUT[12]);
                break;
            case 'N':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[13] : tolower(s[i] + deltaLUT[13]);
                break;
            case 'O':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[14] : tolower(s[i] + deltaLUT[14]);
                break;
            case 'P':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[15] : tolower(s[i] + deltaLUT[15]);
                break;
            case 'Q':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[16] : tolower(s[i] + deltaLUT[16]);
                break;
            case 'R':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[17] : tolower(s[i] + deltaLUT[17]);
                break;
            case 'S':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[18] : tolower(s[i] + deltaLUT[18]);
                break;
            case 'T':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[19] : tolower(s[i] + deltaLUT[19]);
                break;
            case 'U':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[20] : tolower(s[i] + deltaLUT[20]);
                break;
            case 'V':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[21] : tolower(s[i] + deltaLUT[21]);
                break;
            case 'W':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[22] : tolower(s[i] + deltaLUT[22]);
                break;
            case 'X':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[23] : tolower(s[i] + deltaLUT[23]);
                break;
            case 'Y':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[24] : tolower(s[i] + deltaLUT[24]);
                break;
            case 'Z':
                s[i] = isupper(s[i]) ? s[i] + deltaLUT[25] : tolower(s[i] + deltaLUT[25]);
                break;
            default:
                break;
        }
    }
    return s;
}