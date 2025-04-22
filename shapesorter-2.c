//engineering computing 2.1
//shapesorter-2.c
//name: Daryl Sweeney
//c22718249
//
// 11-11-2024
//
//

// shapesorter.c - enhanced version for batch processing
// Loads, analyses / modifies and saves multiple PGM files
// Written by Daryl Sweeney - last modified 12-11-2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>

// 2D pixel array, with 640 rows & 400 columns
int p[400][640]; // Stores pixel values

// Other image properties - width, height, white value
int w, h, maxval; // Width, height, and maximum grayscale value of the image

// Function to check if a directory exists on Windows
int directory_exists(const char *path) {
    DWORD ftyp = GetFileAttributes(path); // Get file attributes of the path
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
    // Return true if path exists and is a directory, false otherwise
}

// Function to create a directory (Windows specific)
void create_directory(const char *path) {
    if (!directory_exists(path)) { // Check if directory exists
        mkdir(path);               // Create directory if it does not exist
    }
}

// Function to classify an image as ring or solid based on pixel analysis
int classify_image() {
    int xtp = -1, ytp = -1; // Variables to store coordinates of first dark pixel

    // Loop through each pixel in the image
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (p[y][x] < 128) {   // Check if pixel is dark (below threshold)
                xtp = x;           // Store x-coordinate of first dark pixel
                ytp = y;           // Store y-coordinate of first dark pixel
                int newY = ytp + 115; // Calculate position 115 pixels below this pixel

                if (newY < h && p[newY][xtp] < 128) { // Check if pixel 115 pixels below is also dark
                    return 0;      // If yes, classify as solid
                } else {
                    return 1;      // Otherwise, classify as ring
                }
            }
        }
    }
    return 0;                      // Default classification is solid
}

// Function to load a PGM file into the pixel array
int load_image(const char *filename) {
    FILE *f = fopen(filename, "r"); // Open file in read mode
    if (f == NULL) {                // Check if file opened successfully
        printf("Error: Could not open file %s\n", filename);
        return -1; // Return error if file cannot be opened
    }

    char line[200];
    fscanf(f, "%[^\n]\n", line);  // Skip P2 line (PGM format identifier)
    fscanf(f, "%[^\n]\n", line);  // Skip comment line
    fscanf(f, "%d %d\n", &w, &h); // Read width and height of the image
    fscanf(f, "%d\n", &maxval);   // Read maximum grayscale value

    // Loop through each pixel position and load value into 2D array
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            fscanf(f, "%d", &p[y][x]); // Read pixel value and store in array
        }
    }

    fclose(f); // Close file after reading
    return 0;  // Return success
}

// Main function to process images in a folder
int main(int argc, char *argv[]) {
    if (argc < 2) { // Check if input folder argument is provided
        printf("Usage: %s <input_folder>\n", argv[0]);
        return 1; // Exit if no folder specified
    }

    char *input_folder = argv[1]; // Store folder path from argument
    char rings_dir[] = "rings";   // Directory for ring images
    char solids_dir[] = "solids"; // Directory for solid images

    create_directory(rings_dir);  // Create "rings" directory if it doesn't exist
    create_directory(solids_dir); // Create "solids" directory if it doesn't exist

    DIR *dir = opendir(input_folder); // Open input folder
    if (dir == NULL) {                // Check if folder was opened successfully
        printf("Error: Could not open directory %s\n", input_folder);
        return 2; // Exit if directory cannot be opened
    }

    struct dirent *entry; // Struct to store each entry in directory
    while ((entry = readdir(dir)) != NULL) { // Loop through all files in folder
        // Process only files with ".pgm" extension
        if (strstr(entry->d_name, ".pgm") != NULL) {
            char filepath[256]; // Buffer for full file path
            snprintf(filepath, sizeof(filepath), "%s/%s", input_folder, entry->d_name);
            // Construct full path to image file

            if (load_image(filepath) != 0) { // Load image into memory
                continue; // Skip this file if loading failed
            }

            int is_ring = classify_image(); // Classify image as ring or solid

            // Determine output path based on classification
            char output_path[256];
            if (is_ring) { // If classified as ring
                snprintf(output_path, sizeof(output_path), "%s/%s", rings_dir, entry->d_name);
                printf("Classified %s as ring. Moving to '%s'.\n", entry->d_name, rings_dir);
            } else {       // If classified as solid
                snprintf(output_path, sizeof(output_path), "%s/%s", solids_dir, entry->d_name);
                printf("Classified %s as solid. Moving to '%s'.\n", entry->d_name, solids_dir);
            }

            // Move the file to the appropriate directory
            if (rename(filepath, output_path) != 0) { // Rename (move) file to new directory
                perror("Error moving file"); // Print error if file move fails
            }
        }
    }

    closedir(dir); // Close input folder
    return 0;      // Program completed successfully
}
