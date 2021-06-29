#include "helpers.h"
#include <math.h>



// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Getting average of blue, green and red
            float color = (round(image[i][j].rgbtBlue) + round(image[i][j].rgbtGreen) + round(image[i][j].rgbtRed)) / 3;

            //Outputting new image
            image[i][j].rgbtBlue = round(color);
            image[i][j].rgbtGreen = round(color);
            image[i][j].rgbtRed = round(color);
        }
    }
}



// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Getting new blue
            float blue = (.131 * image[i][j].rgbtBlue) + (.534 * image[i][j].rgbtGreen) + (.272 * image[i][j].rgbtRed);
            {
                if (blue > 255)
                {
                    blue = 255;
                }
            }
            //Getting new green
            float green = (.168 * image[i][j].rgbtBlue) + (.686 * image[i][j].rgbtGreen) + (.349 * image[i][j].rgbtRed);
            {
                if (green > 255)
                {
                    green = 255;
                }
            }
            //Getting new red
            float red = (.189 * image[i][j].rgbtBlue) + (.769 * image[i][j].rgbtGreen) + (.393 * image[i][j].rgbtRed);
            {
                if (red > 255)
                {
                    red = 255;
                }
            }

            //Outputting new image
            image[i][j].rgbtBlue = round(blue);
            image[i][j].rgbtGreen = round(green);
            image[i][j].rgbtRed = round(red);
        }
    }
}



// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            //Storing old image and reflecting
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}



// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Making copy of image
    RGBTRIPLE blur_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blur_image[i][j] = image[i][j];
        }
    }


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float counter = 0;
            float blue = 0;
            float green = 0;
            float red = 0;

            //Current image pixels
            blue += blur_image[i][j].rgbtBlue;
            green += blur_image[i][j].rgbtGreen;
            red += blur_image[i][j].rgbtRed;

            counter++;

            // Check if pixel above exists
            if (i - 1 >= 0 && j >= 0)
            {
                blue += blur_image[i - 1][j].rgbtBlue;
                green += blur_image[i - 1][j].rgbtGreen;
                red += blur_image[i - 1][j].rgbtRed;

                counter++;
            }
            // Above and left
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                blue += blur_image[i - 1][j - 1].rgbtBlue;
                green += blur_image[i - 1][j - 1].rgbtGreen;
                red += blur_image[i - 1][j - 1].rgbtRed;

                counter++;
            }
            // Above and right
            if (i - 1 >= 0 && j + 1 < width)
            {
                blue += blur_image[i - 1][j + 1].rgbtBlue;
                green += blur_image[i - 1][j + 1].rgbtGreen;
                red += blur_image[i - 1][j + 1].rgbtRed;

                counter++;
            }
            // Left
            if (j - 1 >= 0)
            {
                blue += blur_image[i][j - 1].rgbtBlue;
                green += blur_image[i][j - 1].rgbtGreen;
                red += blur_image[i][j - 1].rgbtRed;

                counter++;
            }
            // Right
            if (j + 1 < width)
            {
                blue += blur_image[i][j + 1].rgbtBlue;
                green += blur_image[i][j + 1].rgbtGreen;
                red += blur_image[i][j + 1].rgbtRed;

                counter++;
            }
            // Below
            if (i + 1 < height && j >= 0)
            {
                blue += blur_image[i + 1][j].rgbtBlue;
                green += blur_image[i + 1][j].rgbtGreen;
                red += blur_image[i + 1][j].rgbtRed;

                counter++;
            }
            // Below and left
            if (i + 1 < height && j - 1 >= 0)
            {
                blue += blur_image[i + 1][j - 1].rgbtBlue;
                green += blur_image[i + 1][j - 1].rgbtGreen;
                red += blur_image[i + 1][j - 1].rgbtRed;

                counter++;
            }
            // Below and right
            if (i + 1 < height && j + 1 < width)
            {
                blue += blur_image[i + 1][j + 1].rgbtBlue;
                green += blur_image[i + 1][j + 1].rgbtGreen;
                red += blur_image[i + 1][j + 1].rgbtRed;

                counter++;
            }

            //Getting new image :D
            image[i][j].rgbtBlue = round(blue / counter);
            image[i][j].rgbtGreen = round(green / counter);
            image[i][j].rgbtRed = round(red / counter);

        }
    }
}



