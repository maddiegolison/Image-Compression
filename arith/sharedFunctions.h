/*
 *      sharedFunctions.h
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      These are shared functions and constants across all modules, including
 *      common math algorithms.
 */

#ifndef __HW4__sharedFunctions__
#define __HW4__sharedFunctions__

#include <stdio.h>
#include <stdint.h>

typedef enum {
        COMPRESS, DECOMPRESS
} CompressionOption;

unsigned const MULTIPLE;

/*
void SFLog(char *message, const char *file, int line);
#define LOG(message) SFLog(message, __FILE__, __LINE__)
 */

/* Forces a number into given range so it doesn't overflow */
float forceIntoRange(float number, float low, float high);

/* Converts a float with given range to an int whose scale is determined by
 * number of bits, and its inverse
 */
int quantizeFloatSigned(float f, unsigned bits, float low, float high);
float unquantizeFloatSigned(int i, unsigned bits, float low, float high);

/* Returns 2 to the power of n */
uint64_t raise2(unsigned n);

#endif /* defined(__HW4__sharedFunctions__) */
