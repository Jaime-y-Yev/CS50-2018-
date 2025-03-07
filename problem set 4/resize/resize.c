// Resizes a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // parse command line
    float factor;
    sscanf(argv[1], "%f", &factor);
    printf("factor = %.2f \n", factor);

    if (factor == 0)
    {
        fprintf(stderr, "Usage: f must be a number\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Store original dimensions
    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Calculate new dimensiones
    if (factor > 0.0 && factor < 1.0)                       // minimize image
    {
        long tempFactor = 10*factor;

        bi.biWidth = 10*bi.biWidth;
        bi.biWidth = bi.biWidth * tempFactor / 100;

        bi.biHeight = 10*bi.biHeight;
        bi.biHeight = bi.biHeight * tempFactor / 100;
    }
    else                                                    // amplify image
    {
       bi.biWidth = bi.biWidth * factor;
       bi.biHeight = bi.biHeight * factor;
    }

    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = (sizeof(RGBTRIPLE) * bi.biWidth + newPadding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        for (int s = 0; s < factor; s++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < oldWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                //printf("read pixel (B,G,R) = (%d,%d,%d) \n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);

                int pixelCounter;
                for (pixelCounter = 0; pixelCounter < factor; pixelCounter++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    //printf("wrote pixel (B,G,R) = (%d,%d,%d) \n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);
                }
            }

            // skip over padding, if any
            fseek(inptr, oldPadding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }

            fseek(inptr, -(oldWidth*sizeof(RGBTRIPLE) + oldPadding), SEEK_CUR);
        }

        fseek(inptr, oldWidth*sizeof(RGBTRIPLE) + oldPadding, SEEK_CUR);
    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
