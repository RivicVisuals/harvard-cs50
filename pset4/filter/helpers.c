#include "helpers.h"

#include <math.h>
#include <stdlib.h>

void createBuffer(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE buffer[height][width]);
void setPixels(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE buffer[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Calculate average of pixel at y/x
            average = (image[y][x].rgbtBlue + image[y][x].rgbtGreen + image[y][x].rgbtRed) / 3.0;

            average = round(average);

            // Set each Color value to the average to get a greyscale image
            image[y][x].rgbtBlue = (int) average;
            image[y][x].rgbtGreen = (int) average;
            image[y][x].rgbtRed = (int) average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width - x; x++)
        {
            // Swap n and width-n pixel in each row
            RGBTRIPLE tmp = image[y][x];
            image[y][x] = image[y][width - 1 - x];
            image[y][width - 1 - x] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*buffer)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    createBuffer(height, width, image, buffer);

    float avgBlue;
    float avgGreen;
    float avgRed;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // reset averages for this pixel
            avgBlue = 0;
            avgGreen = 0;
            avgRed = 0;
            int actualPixels = 0;

            // average the pixel at image[y][x] with each of the surrounding pixels:
            // image[y-1][x-1]
            // image[y-1][x]
            // image[y-1][x+1]
            // ----
            // image[y][x-1]
            // image[y][x]
            // image[y][x+1]
            // ----
            // image[y+1][x-1]
            // image[y+1][x]
            // image[y+1][x+1]
            for (int dy = -1; dy < 2; dy++)
            {
                for (int dx = -1; dx < 2; dx++)
                {
                    // don't try to calculate average of out of bounds pixels
                    if (y + dy < 0 || x + dx < 0 || y + dy >= height || x + dx >= width)
                    {
                        continue;
                    }

                    avgBlue += image[y + dy][x + dx].rgbtBlue;
                    avgGreen += image[y + dy][x + dx].rgbtGreen;
                    avgRed += image[y + dy][x + dx].rgbtRed;

                    actualPixels++;
                }
            }

            // calculate average by dividing by actual count of pixels
            avgBlue /= actualPixels;
            avgGreen /= actualPixels;
            avgRed /= actualPixels;

            avgBlue = round(avgBlue);
            avgGreen = round(avgGreen);
            avgRed = round(avgRed);

            // clamp at 255
            if (avgBlue > 255)
            {
                avgBlue = 255;
            }
            if (avgGreen > 255)
            {
                avgGreen = 255;
            }
            if (avgRed > 255)
            {
                avgRed = 255;
            }

            // set each of the color values to the calculated averages
            buffer[y][x].rgbtBlue = (int) avgBlue;
            buffer[y][x].rgbtGreen = (int) avgGreen;
            buffer[y][x].rgbtRed = (int) avgRed;
        }
    }

    // write buffer to original image
    setPixels(height, width, image, buffer);

    // clean up after ourselves
    free(buffer);
    return;
}

// Detect edges
// Gave up on separable 2D kernels. Done research with
// https://bartwronski.com/2020/02/03/separate-your-filters-svd-and-low-rank-approximation-of-image-filters/
// https://en.m.wikipedia.org/wiki/Sobel_operator
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*buffer)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    const int kernelGx[3][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    const int kernelGy[3][3] = {{-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int GxBlue = 0, GxGreen = 0, GxRed = 0;
            int GyBlue = 0, GyGreen = 0, GyRed = 0;
            float gBlue, gGreen, gRed;

            // Calculate Gx and Gy
            // look at each of the neighboring pixels
            for (int dy = -1; dy < 2; dy++)
            {
                for (int dx = -1; dx < 2; dx++)
                {
                    // assume out of bounds pixels are black
                    if (y + dy < 0 || x + dx < 0 || y + dy >= height || x + dx >= width)
                    {
                        continue;
                    }

                    GxBlue += image[y + dy][x + dx].rgbtBlue * kernelGx[dy + 1][dx + 1];
                    GxGreen += image[y + dy][x + dx].rgbtGreen * kernelGx[dy + 1][dx + 1];
                    GxRed += image[y + dy][x + dx].rgbtRed * kernelGx[dy + 1][dx + 1];

                    GyBlue += image[y + dy][x + dx].rgbtBlue * kernelGy[dy + 1][dx + 1];
                    GyGreen += image[y + dy][x + dx].rgbtGreen * kernelGy[dy + 1][dx + 1];
                    GyRed += image[y + dy][x + dx].rgbtRed * kernelGy[dy + 1][dx + 1];
                }
            }

            // Combine Gx and Gy
            gBlue = sqrt(GxBlue * GxBlue + GyBlue * GyBlue);
            gGreen = sqrt(GxGreen * GxGreen + GyGreen * GyGreen);
            gRed = sqrt(GxRed * GxRed + GyRed * GyRed);

            gBlue = round(gBlue);
            gGreen = round(gGreen);
            gRed = round(gRed);

            // clamp at 255
            if (gBlue > 255)
            {
                gBlue = 255;
            }
            if (gGreen > 255)
            {
                gGreen = 255;
            }
            if (gRed > 255)
            {
                gRed = 255;
            }

            // set each of the color values to the calculated averages
            buffer[y][x].rgbtBlue = gBlue;
            buffer[y][x].rgbtGreen = gGreen;
            buffer[y][x].rgbtRed = gRed;
        }
    }

    // write buffer to original image
    setPixels(height, width, image, buffer);

    // clean up after ourselves
    free(buffer);
    return;
}

// Make a copy of the image to work on
// Realisation that this is needed courtesy of
// https://www.reddit.com/r/cs50/comments/j2tk7t/issues_with_edges_pset4/
// and
// https://www.reddit.com/r/cs50/comments/j3rmum/pset4filterblur_with_4_for_loops/
void createBuffer(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE buffer[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            buffer[y][x] = image[y][x];
        }
    }
    return;
}

// Write modified pixels back to original image
void setPixels(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE buffer[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x] = buffer[y][x];
        }
    }
    return;
}