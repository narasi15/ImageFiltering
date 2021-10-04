#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"


/*
 * The Gaussian Blur filter
 */
void gaussian_blur_filter(Bitmap *bmp) {
    
    int width = bmp->width;
    int height = bmp->height;
    int n = 0;
    
    
    Pixel *before = malloc(sizeof(Pixel)*width);
    Pixel *current = malloc(sizeof(Pixel)*width);
    
    
    
    // read the first row
    for(int i = 0; i < width; i++) {
        Pixel *pixel = malloc(sizeof(Pixel));
        n = fread(pixel, 1, sizeof(Pixel), stdin);
        if (n == -1) {
            perror("read");
            exit(1);
        } else if(n == 0) {
            fprintf(stderr, "Error: could not read from stdin");
            exit(1);
        }
        before[i] = *pixel;
        
        free(pixel);
        
    }
    
    // read the second row
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
        current[j] = *pixel;
        
        free(pixel);
        
    }
    
    // we already read row0 and row1, now read the rows from 2 - 199
    //these are all the rows that will be stored in after
    for(int i = 2; i < height; i++) {
        // we want to store 3 pixel rows at a time
        
        Pixel *after = malloc(sizeof(Pixel)*width);
        
        //read all the pixels in the ith row and store them in after
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
            after[j] = *pixel;
            
            free(pixel);
        }
        
        Pixel *blurred_row = malloc(sizeof(Pixel)*width);
        
        //read out the first 3 pixels of before, current and after to get the 3x3 matrix
        for(int k = 0; k <= width - 3; k++) {
            Pixel *first = malloc(sizeof(Pixel) * 3);
            Pixel *middle = malloc(sizeof(Pixel) * 3);
            Pixel *last = malloc(sizeof(Pixel) * 3);
            
            
            //read the first 3 pixels from before, current and after
            for(int a = 0; a < 3; a++) {
                first[a] = before[k + a];
                middle[a] = current[k + a];
                last[a] = after[k + a];
            }

            //apply the gaussian filter and get result
            Pixel result = apply_gaussian_kernel(first, middle, last);
        
            //get result and change that in the middle row
            memcpy(&blurred_row[k+1], &result, sizeof(Pixel));
            
            
            if(k == 0) {
                memcpy(&blurred_row[k], &result, sizeof(Pixel));
            }
            else if(k == width - 3) {
                memcpy(&blurred_row[k+2], &result, sizeof(Pixel));
            }
            
            // free everything
            free(first);
            free(middle);
            free(last);
            
        
        }
        
        
        // now 3 rows stored have been modified, write out the first row (that is, before)
        if(i == 2 || i == height - 1) {
            for(int r = 0; r < 2; r++) {
                for(int y = 0; y < width; y++) {
                    fwrite(&(blurred_row[y]), 1, sizeof(Pixel), stdout);
                }
            }
        }
        else {
            for(int y = 0; y < width; y++) {
                fwrite(&(blurred_row[y]), 1, sizeof(Pixel), stdout);
            }
        }
        free(blurred_row);
        
        //update the next before and current using memcpy
        memcpy(before, current, sizeof(Pixel)*width);
        memcpy(current, after, sizeof(Pixel)*width);
        
        free(after);

    }
    free(before);
    free(current);

    
    
    
}


int main() {
    run_filter(gaussian_blur_filter, 1);
    return 0;
}
