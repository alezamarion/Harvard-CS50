#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{

    typedef uint8_t BYTE;
    BYTE buffer[512];
    char image[8];
    FILE *jpg = NULL;
    int count = 0;

    //checking for valid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    char *card = argv[1];

    //opening memory card
    FILE *file = fopen(card, "r");

    if (file == NULL)
    {
        printf("Could not open %s\n", card);
        return 1;
    }

    //iterating through memory card untill no blocks left
    while (fread(buffer, 512, 1, file) == 1)
    {
        //checking for valid jpg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (count > 0)
            {
                //found start of a new pic, so close current picture
                fclose(jpg);
            }

            //set jpg filename
            sprintf(image, "%03i.jpg", count);

            //open new file
            jpg = fopen(image, "w");

            //write new block to output file
            fwrite(buffer, 512, 1, jpg);
            count++;
            
        }
        //check if still writing jpg
        else if (count > 0)
        {
            fwrite(buffer, 512, 1, jpg);
        }

    }
    return 0;
    
    //free memory
    free(buffer);
    //close output file
    fclose(jpg);
    //close input file
    fclose(file);
    
}






