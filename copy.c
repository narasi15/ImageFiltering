#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"


/*
 * Main filter loop.
 * This function is responsible for doing the following:
 *   1. Read in pixels one at a time (because copy is a pixel-by-pixel transformation).
 *   2. Immediately write out each pixel.
 *
 * Note that this function should allocate space only for a single Pixel;
 * do *not* store more than one Pixel at a time, it isn't necessary here!
 */
void copy_filter(Bitmap *bmp) {
    // TODO: Complete this function.
    
    int width = bmp->width;
    int height = bmp->height;
    int n = 0;
    for(int i = 0; i < (width*height); i++) {
        Pixel *pixel = malloc(sizeof(Pixel));
        
        n = fread(pixel, 1, sizeof(Pixel), stdin);
        if (n == -1) {
            perror("read");
            exit(1);
        } else if(n == 0) {
            fprintf(stderr, "Error: could not read from stdin");
            exit(1);
        }
        
        fwrite(pixel, 1, sizeof(Pixel), stdout);
        free(pixel);
        
    }
    
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(copy_filter, 1);
    return 0;
}