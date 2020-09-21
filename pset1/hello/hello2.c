#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get the user's name, and include it in an answer
    printf("Hello, %s\n", get_string("What's your name?\n"));
}