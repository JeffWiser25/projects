// shapesorter-5.c
// name: Daryl Sweeney
// c22718249
//
// shapesorter.c - enhanced version for batch processing or single image processing
// Loads, analyses / modifies and saves multiple PGM files or a single PGM file
// Written by Daryl Sweeney - last modified 14-11-2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>

int p[400][640]; // Stores pixel values
int w, h, maxval; // Width, height, and maximum grayscale value of the image

// Function to check if a directory exists
int directory_exists(const char *path) {
    DWORD ftyp = GetFileAttributes(path);
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

// Function to create a directory if it doesn't exist
void create_directory(const char *path) {
    if (!directory_exists(path)) {
        mkdir(path);
    }
}

// Function to add a square to the center of the image
void add_center_square(int is_ring, int xtp, int ytp) {
    int square_size = 20;
    int color = is_ring ? 0 : maxval; // Black for ring, white for solid
    int adjusted_ytp = ytp + 120; // Adjusted position 115 pixels below the first black pixel

    // Ensure the adjusted position is within image bounds
    if (adjusted_ytp < 0 || adjusted_ytp >= h) {
        adjusted_ytp = h - square_size / 2; // Move the square within bounds if too close to edge
    }

    // Position the square centered around the adjusted point
    for (int y = adjusted_ytp - square_size / 2; y < adjusted_ytp + square_size / 2; y++) {
        for (int x = xtp - square_size / 2; x < xtp + square_size / 2; x++) {
            if (y >= 0 && y < h && x >= 0 && x < w) {
                p[y][x] = color;
            }
        }
    }
}

// Function to classify an image as a ring or solid
int classify_image(int *xtp, int *ytp) {
    *xtp = -1; 
    *ytp = -1; // Variables to store coordinates of the first dark pixel

    // Loop through each pixel in the image
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (p[y][x] < 128) { // Check if pixel is dark (below threshold)
                *xtp = x;
                *ytp = y;

                // Move down by 115 pixels
                int newY = y + 115;

                // Ensure the newY position is within the bounds of the image
                if (newY >= h) {
                    newY = h - 1; // Clamp the value to the bottom if out of bounds
                }

                // Check directly below, left, right, and below-left, below-right
                if ((newY < h && p[newY][x] >= 128) || // Check directly below for white pixel
                    (x - 40 >= 0 && p[newY][x - 40] >= 128) || // Check left (12 pixels to the left)
                    (x + 40 < w && p[newY][x + 40] >= 128)) { // Check right (12 pixels to the right)
                    return 1; // It's a ring if any white pixel is found
                }

                // If no white pixel detected, check further down (by an additional 12 pixels)
                newY = y + 150; // Move further down 12 pixels to check

                if (newY < h && p[newY][x] >= 128) { // Check below after 12 more pixels
                    return 1; // It's a ring if any white pixel is found further down
                }
                
                return 0; // Solid if no white pixel is found
            }
        }
    }
    return 0; // Default classification as solid
}

// Function to load the PGM image file
int load_image(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    char line[200];
    fscanf(f, "%[^\n]\n", line);
    fscanf(f, "%[^\n]\n", line);
    fscanf(f, "%d %d\n", &w, &h);
    fscanf(f, "%d\n", &maxval);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            fscanf(f, "%d", &p[y][x]);
        }
    }

    fclose(f);
    return 0;
}

// Function to save the modified image to a file
void save_image(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error: Could not save file %s\n", filename);
        return;
    }

    fprintf(f, "P2\n# Modified image\n%d %d\n%d\n", w, h, maxval);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            fprintf(f, "%d ", p[y][x]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

// Function to process an individual image
void process_image(const char *image_path, int *ring_count, int *solid_count) {
    if (load_image(image_path) != 0) {
        return;
    }

    int xtp, ytp;
    int is_ring = classify_image(&xtp, &ytp);

    char output_path[256];
    char modified_output_path[256];

    if (is_ring) {
        (*ring_count)++;
        snprintf(output_path, sizeof(output_path), "rings/ring_%d.pgm", *ring_count);
        snprintf(modified_output_path, sizeof(modified_output_path), "modified_rings/ring_%d_modified.pgm", *ring_count);
        printf("Classified %s as ring. Moving to 'rings'.\n", image_path);
    } else {
        (*solid_count)++;
        snprintf(output_path, sizeof(output_path), "solids/solid_%d.pgm", *solid_count);
        snprintf(modified_output_path, sizeof(modified_output_path), "modified_solids/solid_%d_modified.pgm", *solid_count);
        printf("Classified %s as solid. Moving to 'solids'.\n", image_path);
    }

    // Save the original classified image
    if (rename(image_path, output_path) != 0) {
        perror("Error moving file");
    }

    // Add square at detection point and save the modified image
    add_center_square(is_ring, xtp, ytp);
    save_image(modified_output_path);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_folder_or_image_file>\n", argv[0]);
        return 1;
    }

    char *input_path = argv[1];

    create_directory("rings");
    create_directory("solids");
    create_directory("modified_rings");
    create_directory("modified_solids");

    int ring_count = 0;
    int solid_count = 0;

    // Check if the input is a directory or a single file
    if (directory_exists(input_path)) {
        DIR *dir = opendir(input_path);
        if (dir == NULL) {
            printf("Error: Could not open directory %s\n", input_path);
            return 2;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, ".pgm") != NULL) {
                char filepath[256];
                snprintf(filepath, sizeof(filepath), "%s/%s", input_path, entry->d_name);
                process_image(filepath, &ring_count, &solid_count);
            }
        }
        closedir(dir);
    } else {
        process_image(input_path, &ring_count, &solid_count);
    }

    printf("Total rings sorted: %d\n", ring_count);
    printf("Total solids sorted: %d\n", solid_count);

    return 0;
}
