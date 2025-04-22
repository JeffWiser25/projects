// Name: Daryl Sweeney
// Student ID: c22718249
// Course: Engineering Computing 2.1
// Project: ShapeSorter
// Filename: shapesorter-5.c
// Last Modified: 13-11-2024

// Program Description:
// This program processes images of shapes (rings or solids) in PGM format.
// It classifies each image as either a ring or a solid based on pixel brightness patterns.
// The program can process a single image or all images in a folder and save them into separate output folders (rings or solids).
// The program also marks the detected shape with a small square and saves the modified image.

#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For general utilities like memory allocation
#include <string.h>     // For string manipulation functions
#include <dirent.h>     // For directory operations
#include <windows.h>    // For Windows-specific file and folder handling

// Global Variables:
// 2D array to store image pixel data (maximum size: 400x640)
// `p` represents the pixel array, `w` and `h` are the image dimensions, and `maxval` is the maximum grayscale value.
int p[400][640]; 
int w, h, maxval;

// Function: Check if a directory exists
// Input: Path to the directory
// Output: Returns 1 if the directory exists, 0 otherwise
int directory_exists(const char *path) {
    DWORD ftyp = GetFileAttributes(path); // Retrieve file attributes
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY)); // Check if it's a directory
}

// Function: Create a directory if it does not exist
// Input: Path to the directory
void create_directory(const char *path) {
    if (!directory_exists(path)) {
        mkdir(path); // Create the directory
    }
}

// Function: Add a small square to mark the detected shape in the image
// Input: `is_ring` (1 if the shape is a ring, 0 if solid), `xtp` (x-coordinate), `ytp` (y-coordinate)
// Adds a square at the detected location with a color based on the classification.
void add_center_square(int is_ring, int xtp, int ytp) {
    int square_size = 20;               // Size of the square
    int color = is_ring ? 0 : maxval;   // Black square for a ring, white for a solid
    int adjusted_ytp = ytp + 120;       // Move square position slightly downward

    // Ensure the square stays within image bounds
    if (adjusted_ytp < 0 || adjusted_ytp >= h) {
        adjusted_ytp = h - square_size / 2; 
    }

    // Draw the square pixel by pixel
    for (int y = adjusted_ytp - square_size / 2; y < adjusted_ytp + square_size / 2; y++) {
        for (int x = xtp - square_size / 2; x < xtp + square_size / 2; x++) {
            if (y >= 0 && y < h && x >= 0 && x < w) { 
                p[y][x] = color; // Assign pixel value based on classification
            }
        }
    }
}

// Function: Classify the image as a ring or a solid shape
// Input: Pointers to `xtp` (x-coordinate) and `ytp` (y-coordinate) for the first detected dark pixel
// Output: Returns 1 if the shape is classified as a ring, 0 if solid
int classify_image(int *xtp, int *ytp) {
    *xtp = -1; 
    *ytp = -1; // Initialize coordinates to -1

    // Loop through each pixel in the image
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (p[y][x] < 128) { // Check if pixel is dark
                *xtp = x; 
                *ytp = y;

                // Check pixels 115 and 153 rows below the detected pixel, and 40 columns left/right
                int newY = y + 115;
                if (newY >= h) {
                    newY = h - 1; // Clamp newY within bounds
                }

                // Check for lighter pixels at specified offsets
                if ((newY < h && p[newY][x] >= 128) || 
                    (x - 40 >= 0 && p[newY][x - 40] >= 128) || 
                    (x + 40 < w && p[newY][x + 40] >= 128)) {
                    return 1; // Classified as ring
                }

                // Check further down
                newY += 38;
                if (newY >= h) {
                    newY = h - 1; // Clamp newY within bounds
                }

                // Check again at deeper offsets
                if ((newY < h && p[newY][x] >= 128) || 
                    (x - 40 >= 0 && p[newY][x - 40] >= 128) || 
                    (x + 40 < w && p[newY][x + 40] >= 128)) {
                    return 1; // Classified as ring
                }

                return 0; // Classified as solid
            }
        }
    }
    return 0; // Default to solid if no dark pixels are found
}

// Function: Load image data from a PGM file
// Input: Path to the PGM file
// Output: Returns 0 on success, -1 on failure
int load_image(const char *filename) {
    FILE *f = fopen(filename, "r"); // Open file for reading
    if (f == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    char line[200];
    fscanf(f, "%[^\n]\n", line); // Read and discard header
    fscanf(f, "%[^\n]\n", line); 
    fscanf(f, "%d %d\n", &w, &h); // Read image dimensions
    fscanf(f, "%d\n", &maxval);  // Read maximum brightness

    // Read pixel data into the 2D array
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            fscanf(f, "%d", &p[y][x]);
        }
    }

    fclose(f); // Close the file
    return 0;
}

// Function: Save image data to a PGM file
// Input: Path to the output file
void save_image(const char *filename) {
    FILE *f = fopen(filename, "w"); // Open file for writing
    if (f == NULL) {
        printf("Error: Could not save file %s\n", filename);
        return;
    }

    // Write image header and pixel data
    fprintf(f, "P2\n# Modified image\n%d %d\n%d\n", w, h, maxval);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            fprintf(f, "%d ", p[y][x]);
        }
        fprintf(f, "\n");
    }

    fclose(f); // Close the file
}

// Function: Process a single image and classify it
// Input: Image path, counts for rings and solids
void process_image(const char *image_path, int *ring_count, int *solid_count) {
    if (load_image(image_path) != 0) {
        return; // Exit if image loading fails
    }

    int xtp, ytp; // Coordinates of detected pixel
    int is_ring = classify_image(&xtp, &ytp); // Classify the image

    char output_path[256];
    char modified_output_path[256];

    // Set output paths and increment appropriate counters
    if (is_ring) {
        (*ring_count)++;
        snprintf(output_path, sizeof(output_path), "rings/ring_%d.pgm", *ring_count);
        snprintf(modified_output_path, sizeof(modified_output_path), "modified_rings/ring_%d_modified.pgm", *ring_count);
    } else {
        (*solid_count)++;
        snprintf(output_path, sizeof(output_path), "solids/solid_%d.pgm", *solid_count);
        snprintf(modified_output_path, sizeof(modified_output_path), "modified_solids/solid_%d_modified.pgm", *solid_count);
    }

    rename(image_path, output_path); // Move image to the corresponding folder
    add_center_square(is_ring, xtp, ytp); // Mark the detected shape
    save_image(modified_output_path); // Save the modified image
}

// Main Function: Entry point of the program
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_folder_or_image_file>\n", argv[0]);
        return 1;
    }

    char *input_path = argv[1];

    // Create necessary directories
    create_directory("rings");
    create_directory("solids");
    create_directory("modified_rings");
    create_directory("modified_solids");

    int ring_count = 0; 
    int solid_count = 0;

    // Check if input is a folder
    DIR *folder = opendir(input_path);
    if (folder != NULL) {
        struct dirent *entry;
        char image_path[256];

        // Loop through files in the folder
        while ((entry = readdir(folder)) != NULL) {
            if (strstr(entry->d_name, ".pgm")) { // Check for PGM files
                snprintf(image_path, sizeof(image_path), "%s/%s", input_path, entry->d_name);
                process_image(image_path, &ring_count, &solid_count);
            }
        }

        closedir(folder); // Close the folder
    } else {
        // Process single image file
        process_image(input_path, &ring_count, &solid_count);
    }

    // Print summary
    printf("Processing complete!\n");
    printf("Rings detected: %d\n", ring_count);
    printf("Solids detected: %d\n", solid_count);

    return 0;
}
