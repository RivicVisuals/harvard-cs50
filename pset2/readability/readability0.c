#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main (void)
{
    string s = get_string("Text: ");

    int n = strlen(s);

    // 0: repeat after me
    //printf("%s\n", s);

    // 1: Letters
    int lettersCount = 0;
    for (int i = 0; i < n; i++)
    {
        // if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')) // check for alphabetic signs
        if (isalpha(s[i])) // be smart about checkig for alphabetic signs.
        {
            lettersCount++;
        }
        else
        {
            continue;
        }
    }
    printf("%i letter(s)\n", lettersCount);

    // 2: Words
    int wordsCount = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '-' || s[i] == '\'') // ignore hyphenation and appostrophes for purposes of this problem set
        {
            continue;
        }
        if ((isspace(s[i]) || ispunct(s[i])) && isalpha(s[i - 1])) // an alphabetic character before a space or punctuation counts as a word.
        {
            wordsCount++;
        }
        else
        {
            continue;
        }
    }

    printf("%i word(s)\n", wordsCount);

    // 3: sentences
    int sentenceCount = 0;
    for (int i = 0; i < n; i++)
    {
        if ((s[i] == '.' && s[i - 1] != '.') || s[i] == '?' || s[i] == '!') // assume every sentence ends with . or ? or ! and count as sentence. Ignore Elipsis (...) though.
        {
            sentenceCount++;
        }
        else
        {
            continue;
        }
    }

    printf("%i sentence(s)\n", sentenceCount);

    // 4: grade level
    //float gradeLevel = 0.0588 * averageLettersPer100Words - 0.296 * averageSentencesPer100Words - 15.8;
}