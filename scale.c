#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

int scale_factor = 0;

/*
 * The scale filter
 */
void scale_filter(Bitmap *bmp) {
    
    //get the original picture's dimensions
    int width = bmp->width/scale_factor;
    int height = bmp->height/scale_factor;
    
    int n = 0;
    

    for(int i = 0; i < height; i++) { //iterate through the rows
        // store a pixel row
        Pixel *arr = malloc(width * sizeof(*arr));
        
        for(int j = 0; j < width; j++) {
            Pixel *pixel = malloc(sizeof(Pixel));
            n = fread(pixel, 1, sizeof(Pixel), stdin);
            if (n == -1) {
                perror("read");
                exit(1);
            } else if(n == 0) {
                fprintf(stderr, "Error: could not read from stdin");
                exit(1);
            }
            arr[j] = *pixel;
            
            // write out each pixel <scale factor> times
            for(int k = 0; k < scale_factor; k++) {
                fwrite(pixel, 1, sizeof(Pixel), stdout);
            }
            free(pixel);
            
        }
        
        //write out the rows again <scale factor> times
        for(int y = 0; y < scale_factor - 1; y++) { //
            for(int l = 0; l < width; l++) {
                for(int m = 0; m < scale_factor; m++) {
                    fwrite(&arr[l], 1, sizeof(Pixel), stdout);
                }
            }
        }
        
        free(arr);
        
        
    }
}

int main(int argc, char *argv[]) {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    scale_factor = strtol(argv[1], NULL, 0);
    
    run_filter(scale_filter, scale_factor);
    return 0;
}