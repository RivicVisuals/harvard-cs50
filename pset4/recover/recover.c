#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BLOCK_SIZE 512
typedef uint8_t byte;

// typedef uint8_t byte;

int checkSignature(FILE *diskImage, long *start);
void copyBytes(FILE *output, FILE *diskImage, long *start, long offset);

int main(int argc, char *argv[])
{
    // ensure correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open the image file passed in by argv[1] for reading, twice for a read ahead
    FILE *img = fopen(argv[1], "r");
    FILE *imgReadAhead = fopen(argv[1], "r");

    // error reading the file
    if (!img)
    {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }

    // Business Logic of the analytics tool
    int counter = 0;
    char fileName[8];

    long startByte = 0;
    long offsetBlocks = 0;

    do
    {
        // Read ahead and extract start + offset of valid jpg
        offsetBlocks = checkSignature(imgReadAhead, &startByte);

        if (offsetBlocks == 0)
        {
            break;
        }

        // only prepare files if we find traces of jpg

        // compile the file name for each jpg
        sprintf(fileName, "%03i.jpg", counter);

        // create a new file and fill it with bytes from img
        FILE *file = fopen((const char *)fileName, "w");
        copyBytes(file, img, &startByte, offsetBlocks);

        // prepare for next file
        counter++;
        startByte += offsetBlocks * BLOCK_SIZE;

        // clean up after ourselves
        fclose(file);
    }
    while (!feof(imgReadAhead)); // Halt at end of readahead

    // clean up after ourselves
    fclose(img);
    fclose(imgReadAhead);
}

int checkSignature(FILE *diskImage, long *pstart)
{
    /*
    * Checks for JPEG signature at the first four bytes of a block
    * until it either finds another signature or the end of the disk image
    */
    long offset = 0;
    long start = *pstart;
    long end = 0;

    byte tmp[BLOCK_SIZE];

    do
    {
        fread(tmp, BLOCK_SIZE, 1, diskImage);

        // JPG Magic Bytes
        if (tmp[0] == 0xff && tmp[1] == 0xd8 && tmp[2] == 0xff && (tmp[3] >= 0xe0 && tmp[3] <= 0xef))
        {
            if (start == 0) // Find first start of jpg
            {
                start = ftell(diskImage) - BLOCK_SIZE; // we've read over the current block, set start back 1 block to include the magic bytes
                *pstart = start;
            }
            else
            {
                end = ftell(diskImage) - BLOCK_SIZE;
            }
        }

        if (feof(diskImage)) // Halt at end of diskImage
        {
            if (start == 0) // no jpg found
            {
                return 0;
            }
            end = ftell(diskImage);
            break;
        }
    }
    while (start >= end); // read bytes until we find another jpg header after our starting point

    offset = (end - start) / BLOCK_SIZE;
    return offset;
}

void copyBytes(FILE *output, FILE *diskImage, long *pstart, long offset)
{
    /*
    * Write offset bytes from *pstart in FILE* output
    * Reads and writes in chunks to be more efficient
    */

    char *tmp;
    tmp = malloc(offset * BLOCK_SIZE);

    fseek(diskImage, *pstart, SEEK_SET);
    fread(tmp, offset * BLOCK_SIZE, 1, diskImage);

    fwrite(tmp, BLOCK_SIZE, offset, output);

    // clean up after ourselves
    free(tmp);
    return;
}
