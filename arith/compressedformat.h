/*
 *      compressedformat.h
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This reads, writes, and frees CompressedArrays, which are arrays of
 *      packed words.
 */

#ifndef __HW4__compressedformat__
#define __HW4__compressedformat__

#include <stdio.h>
#include <inttypes.h>
#include "uarray.h"

typedef uint32_t Compressed;            /* For a word */
typedef struct CompressedArray {        /* The array of words */
        UArray_T array;
        unsigned originalWidth;
        unsigned originalHeight;
} *CompressedArray;

/* Reads compressed image file and stores and returns the array of words */
CompressedArray readCompressed(FILE *file);

/* Prints the compressed format from the array of words in row-major order */
void writeCompressed(CompressedArray array);

void freeCompressed(CompressedArray *array);

#endif /* defined(__HW4__compressedformat__) */
