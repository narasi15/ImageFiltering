#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"


/*
 * Read in bitmap header data from stdin, and return a pointer to
 * a new Bitmap struct containing the important metadata for the image file.
 *
 * TODO: complete this function.
 *
 * Notes:
 *   1. Store header data in an array of unsigned char (essentially
 *      an array of bytes). Examples:
 *      on the stack,
 *          unsigned char data[10];
 *      on the heap,
 *          unsigned char *data = malloc(10);
 *
 *   2. Don't make any assumptions about the header size. You should read in
 *      BMP_HEADER_SIZE_OFFSET bytes first, and then the header size,
 *      and then use this to allocate enough space for the actual header.
 *
 *   3. You can use memcpy to transfer bytes to and from the Bitmap "header" field.
 *      You can even write these bytes to memory allocated for variables of other types!
 *      For example:
 *          unsigned char bytes[4];
 *          int x = 10;
 *          int y;
 *          memcpy(bytes, &x, 4);  // Copy the int x into bytes.
 *          memcpy(&y, bytes, 4);  // Copy the contents of bytes into y.
 *
 *   4. You can use either fread/fwrite or read/write to perform I/O operations here.
 *
 *   5. Make good use of the provided macros in bitmap.h to index into the "header" array.
 */
Bitmap *read_header() {
    int n = 0;
    int width, height, header_size = 0;  // information to collect
    unsigned char first[BMP_HEADER_SIZE_OFFSET];    //10 elements
    unsigned char headerSize[4];
    unsigned char second[BMP_WIDTH_OFFSET - 1 - (BMP_HEADER_SIZE_OFFSET + 3)];
    unsigned char w[4];
    unsigned char h[4];
    
    
    // read up to the header size offset(0-9)
    n = fread(first, BMP_HEADER_SIZE_OFFSET, 1, stdin);
    if (n == -1) {
            perror("read");
            exit(1);
        } else if(n == 0) {
            fprintf(stderr, "Error: could not read from stdin");
            exit(1);
        }
    

    
    // now we are at offset of header size, read 4 bytes to get the header size
    n = fread(headerSize, sizeof(int), 1, stdin);
    if (n == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }
    memcpy(&header_size, headerSize, sizeof(int));  //store int value of header size
    

    // read up until we get to width offset (14-17)
    n = fread(second, BMP_WIDTH_OFFSET - 1 - (BMP_HEADER_SIZE_OFFSET + 3), 1, stdin);
    if (n == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }
    

    // get width information (18-21)
    n = fread(w, sizeof(int), 1, stdin);
    if (n == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }

    memcpy(&width, w, sizeof(int));      //store int value of width

    // get height information
    n = fread(h, sizeof(int), 1, stdin);
    if (n == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }
    memcpy(&height, h, sizeof(int));      //store int value of height
    
    
    
    unsigned char rest[header_size - 1 -(BMP_HEIGHT_OFFSET + 3)];
    
    // read the rest of the stuff in header
    n = fread(rest, header_size - 1 - (BMP_HEIGHT_OFFSET + 3), 1, stdin);
    if (n == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }
    
    
    
    // create a new array of full header size and store all the header info
    unsigned char *fullHeader = malloc(header_size + 1);
    
    memcpy(fullHeader, first, sizeof(first));
    memcpy(fullHeader + sizeof(first), headerSize, sizeof(headerSize));
    memcpy(fullHeader + sizeof(first) + sizeof(headerSize), second, sizeof(second));
    
    memcpy(fullHeader + sizeof(first) + sizeof(headerSize) + sizeof(second), w, sizeof(w));
    memcpy(fullHeader + sizeof(first) + sizeof(headerSize) + sizeof(second) + sizeof(w), h, sizeof(h));
    memcpy(fullHeader + sizeof(first) + sizeof(headerSize) + sizeof(second) + sizeof(w) + sizeof(h), rest, sizeof(rest));
    
    
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    bitmap->headerSize = header_size;
    bitmap->header = fullHeader;
    bitmap->width = width;
    bitmap->height = height;
    
    return bitmap;
}

/*
 * Write out bitmap metadata to stdout.
 * You may add extra fprintf calls to *stderr* here for debugging purposes.
 */
void write_header(const Bitmap *bmp) {
    fwrite(bmp->header, bmp->headerSize, 1, stdout);
}

/*
 * Free the given Bitmap struct.
 */
void free_bitmap(Bitmap *bmp) {
    free(bmp->header);
    free(bmp);
}

/*
 * Update the bitmap header to record a resizing of the image.
 *
 * TODO: complete this function when working on the "scale" filter.
 *
 * Notes:
 *   1. As with read_header, use memcpy and the provided macros in bitmap.h.
 *
 *   2. bmp->header *must* be updated, as this is what's written out
 *      in write_header.
 *
 *   3. You may choose whether or not to also update bmp->width and bmp->height.
 *      This choice may depend on how you implement the scale filter.
 */
void scale(Bitmap *bmp, int scale_factor) {
    //update the new width and height
    int old_width = bmp->width;
    int old_height = bmp->height;
    
    
    bmp->width = (bmp->width)*scale_factor;
    bmp->height = (bmp->height)*scale_factor;
    
    unsigned char updated_width[4];
    unsigned char updated_height[4];
    
    memcpy(updated_width, &bmp->width, sizeof(int));
    memcpy(updated_height, &bmp->height, sizeof(int));
    
    //update the width and height in header
    memcpy(bmp->header + BMP_WIDTH_OFFSET, updated_width, sizeof(int));
    memcpy(bmp->header + BMP_HEIGHT_OFFSET, updated_height, sizeof(int));
    
    //update the file size
    //int old_file_size = 0;
    //memcpy(&old_file_size, bmp->header + BMP_FILE_SIZE_OFFSET, sizeof(int));
    unsigned char updated_filesize[4];
    int new_file_size = bmp->headerSize + (scale_factor*old_height*scale_factor*old_width*3);

    memcpy(updated_filesize, &new_file_size, sizeof(int));
    
    //update bmp->header file size
    memcpy(bmp->header + BMP_FILE_SIZE_OFFSET, updated_filesize, sizeof(int));
    
}



/*
 * The "main" function.
 *
 * Run a given filter function, and apply a scale factor if necessary.
 * You don't need to modify this function to make it work with any of
 * the filters for this assignment.
 */
void run_filter(void (*filter)(Bitmap *), int scale_factor) {
    
    Bitmap *bmp = read_header();

    
    if (scale_factor > 1) {
        scale(bmp, scale_factor);
    }

    write_header(bmp);

    // Note: here is where we call the filter function.
    filter(bmp);

    free_bitmap(bmp);
}


/******************************************************************************
 * The gaussian blur and edge detection filters.
 * You should NOT modify any of the code below.
 *****************************************************************************/
const int gaussian_kernel[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}
};

const int kernel_dx[3][3] = {
    {1, 0, -1},
    {2, 0, -2},
    {1, 0, -1}
};

const int kernel_dy[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
};

const int gaussian_normalizing_factor = 16;


Pixel apply_gaussian_kernel(Pixel *row0, Pixel *row1, Pixel *row2) {
    int b = 0, g = 0, r = 0;
    Pixel *rows[3] = {row0, row1, row2};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b += rows[i][j].blue * gaussian_kernel[i][j];
            g += rows[i][j].green * gaussian_kernel[i][j];
            r += rows[i][j].red * gaussian_kernel[i][j];
        }
    }

    b /= gaussian_normalizing_factor;
    g /= gaussian_normalizing_factor;
    r /= gaussian_normalizing_factor;

    Pixel new = {
        .blue = b,
        .green = g,
        .red = r
    };

    return new;
}


Pixel apply_edge_detection_kernel(Pixel *row0, Pixel *row1, Pixel *row2) {
    int b_dx = 0, b_dy = 0;
    int g_dx = 0, g_dy = 0;
    int r_dx = 0, r_dy = 0;
    Pixel *rows[3] = {row0, row1, row2};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b_dx += rows[i][j].blue * kernel_dx[i][j];
            b_dy += rows[i][j].blue * kernel_dy[i][j];
            g_dx += rows[i][j].green * kernel_dx[i][j];
            g_dy += rows[i][j].green * kernel_dy[i][j];
            r_dx += rows[i][j].red * kernel_dx[i][j];
            r_dy += rows[i][j].red * kernel_dy[i][j];
        }
    }
    int b = floor(sqrt(square(b_dx) + square(b_dy)));
    int g = floor(sqrt(square(g_dx) + square(g_dy)));
    int r = floor(sqrt(square(r_dx) + square(r_dy)));

    int edge_val = max(r, max(g, b));
    Pixel new = {
        .blue = edge_val,
        .green = edge_val,
        .red = edge_val
    };

    return new;
}
