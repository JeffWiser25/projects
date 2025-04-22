//
// shapesorter.c - alternative version
// Loads, analyses / modifies and saves a PGM file
// Written by Ted Burke - last modified 20-11-2023
//
// To compile shapesorter:
//
//     gcc -o shapesorter.exe shapesorter.c
//
// Input and output filenames should be specified
// as command line arguments. For example,
//
//     shapesorter.exe image001.pgm modified.pgm
//
// If no output file is specified, the default "output.pgm".
//
 
#include <stdio.h>
  
// 2D pixel array, like a spreadsheet with 640 rows & 400 columns
int p[400][640];
  
// Other image properties - width, height, white value
int w, h, maxval;
  
int main(int argc, char *argv[])
{
    // Variables
    FILE *f;
    char line[200];
    int x, y;
      
    ///////////////////////////////////////////////
    /// LOAD PIXEL DATA FROM FILE INTO 2D ARRAY ///
    ///////////////////////////////////////////////
     
    // Check whether an input filename was provided
    // as a command line argument
    if (argc < 2)
    {
        printf("Error: No input file specified\n");
        return 1;
    }
     
    // Open input image file
    printf("Opening input file\n"); 
    f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Error: Invalid input file specified\n");
        return 2;
    }
      
    // Read 4 header lines.
    // The first two lines are simply ignored.
    // The next two lines contain the width,
    // height and maximum pixel value (white).
    printf("Reading file header info\n");
    fscanf(f, "%[^\n]\n", line);
    fscanf(f, "%[^\n]\n", line);
    fscanf(f, "%d %d\n", &w, &h);
    fscanf(f, "%d\n", &maxval);
      
    // Read pixel data into 2-D array
    printf("Reading pixel data from file\n");
    for (y = 0 ; y < h ; ++y)
    {
        for (x = 0 ; x < w ; ++x)
        {
            fscanf(f, "%d", &p[y][x]);
        }
    }
      
    // Close input file
    printf("Closing input file\n");
    fclose(f);
      
    //////////////////////////////////////
    /// MODIFY ALL PIXELS IN THE IMAGE ///
    //////////////////////////////////////
      
    // Variables for processing dark pixels and object detection
    int dark_pixel_count = 0;
    int xtp = -1, ytp = -1;
    
    printf("Analysing pixel data\n");
    
    // Loop through all pixels
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            // Look for the first dark pixel (black)
            if (p[y][x] < 128) {
                // Log the coordinates of the first dark pixel (xtp, ytp)
                xtp = x;
                ytp = y;
                printf("First dark pixel found at (%d, %d)\n", xtp, ytp);

                // Move 100 pixels downwards (center of circle)
                int newY = ytp + 115;
                
                // Make sure newY is within the bounds of the image
                if (newY < h) {
                    // Check the pixel at the new position (newY, xtp)
                    if (p[newY][xtp] < 128) {
                        printf("The object is a solid at(%d, %d)\n", xtp, newY);
                    } else {
                        printf("It's a ring at (%d, %d)\n", xtp, newY);
                    }
                } else {
                    printf("The pixel at (%d, %d) is too close to the bottom of the image.\n", xtp, ytp);
                }

                // After finding the first dark pixel and processing, break the loops
                break;
            }
        }
        if (xtp != -1) break; // Exit outer loop once the first dark pixel is found
    }

    // Output the count of dark pixels
    printf("%d dark pixels found\n", dark_pixel_count);
    //////////////////////////////
    /// OUTPUT IMAGE TO A FILE ///
    //////////////////////////////
      
    // open output file
    printf("Opening output file\n");
    if (argc > 2)
    {
        f = fopen(argv[2], "w");
    }
    else
    {
        f = fopen("output.pgm", "w");
    }
     
    if (f == NULL)
    {
        printf("Error: Failed to open output file\n");
        return 3;
    }
      
    // Print header info into file
    printf("Printing file header to file\n");
    fprintf(f, "P2\n# My PGM file\n%d %d\n%d\n", w, h, maxval);
 
    // Print pixel data into file
    printf("Printing pixel data to file\n");
    for (y = 0 ; y < h ; ++y)
    {
        for (x = 0 ; x < w ; ++x)
        {
            fprintf(f, "%3d ", p[y][x]);
        }
        fprintf(f, "\n");
    }
      
    // Close file
    printf("Closing output file\n");
    fclose(f);
      
    // exit normally
    return 0;
}