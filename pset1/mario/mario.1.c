#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    do
    {
        //Ask user how tall the pyramid shall be
        height = get_int("How tall: ");
    }
    while (height < 1 || height > 8); //Check if answer is 1 to 8, if answer is not 1 to 8, ask again

    //answer is valid, draw pyramid

    //Loop over the height to print as many lines
    for (int line = 0; line < height; line++)
    {
        //Loop over the columns to print as many spaces or hashes
        //The Pyramid has columns double its height + 2 more for the spacers
        for (int width = 1; width <= (height * 2 + 2); width++)
        {

            //before we print hashes, we need to print (height - current line) spaces, as well as in between pyramids
            if (width < height - line || width == height + 1 || width == height + 2)
            {
                printf(" ");
            }

            //draw the right half up to the correct width
            else if (width > height + line + 3)
            {
                break;
            }
            else
            {
                printf("#");
            }
        }

        printf("\n");
    }
}
