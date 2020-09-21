#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get user's name
    string name = get_string("What's your name?\n");
    //Print answer comined with name
    printf("Hello, %s\n", name);
    return 0;
}