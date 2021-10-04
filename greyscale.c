#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"


/*
 * The greyscale function
 */
void greyscale_filter(Bitmap *bmp) {
    
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
        unsigned char new = ((pixel->blue + pixel->green + pixel->red)/3);
        
        pixel->blue = new;
        pixel->green = new;
        pixel->red = new;

        fwrite(pixel, 1, sizeof(Pixel), stdout);
        free(pixel);
        
    }
    
}

int main() {
    run_filter(greyscale_filter, 1);
    return 0;
}