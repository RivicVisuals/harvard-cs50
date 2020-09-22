#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <cs50.h>

int countLetters(char currentLetter);
int countWords(char currentLetter, char prevLetter);
int countSentences(char currentLetter, char prevLetter);

int letterCount = 0;
int wordCount = 0;
int sentenceCount = 0;

int main(void)
{
    string s = get_string("Text: ");

    int n = strlen(s);

    // iterate once over the whole string and check each character and increment the counters accordingly.
    for (int i = 0; i < n; i++)
    {
        // Letters
        countLetters(s[i]);
        // Words
        countWords(s[i], s[i - 1]);
        // Sentences
        countSentences(s[i], s[i - 1]);
    }

    // calculate readability grade level
    float avgLetters = (float) letterCount * 100 / (float) wordCount;
    float avgSentences = (float) sentenceCount * 100 / (float) wordCount;
    float indexReadability = 0.0588 * avgLetters - 0.296 * avgSentences - 15.8;

    int gradeLevel = (int) round(indexReadability);  // round grade index to the nearest int

    if (gradeLevel < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (gradeLevel >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", gradeLevel);
    }
}

int countLetters(char currentLetter)
{
    /*
     For use inside of a loop iterating over string s
     Works on global variable lettersCount.

     Params: currentLetter of string s
    */

    if (isalpha(currentLetter)) // be smart about checkig for alphabetic signs.
    {
        letterCount++;
    }

    return 0;
}

int countWords(char currentLetter, char prevLetter)
{
    /*
     For use inside of a loop iterating over string s
     Works on global variable wordsCount.

     Params: currentLetter of string s
     Params: prevLetter of string s
    */

    if (currentLetter == '-' || currentLetter == '\'') // ignore hyphenation and appostrophes for purposes of this problem set
    {
        return 0;
    }
    if ((isspace(currentLetter) || ispunct(currentLetter))
        && isalpha(prevLetter)) // an alphabetic character before a space or punctuation counts as a word.
    {
        wordCount++;
    }
    return 0;
}

int countSentences(char currentLetter, char prevLetter)
{
    /*
     For use inside of a loop iterating over string s
     Works on global variable sentenceCount.

     Params: currentLetter of string s
     Params: prevLetter of string s
    */

    // assume every sentence ends with . or ? or ! and count as sentence. Ignore Elipsis (...) though.
    if ((currentLetter == '.' && prevLetter != '.')
        || currentLetter == '?'
        || currentLetter == '!')
    {
        sentenceCount++;
    }
    return 0;
}