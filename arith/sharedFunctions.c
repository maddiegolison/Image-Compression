/*
 *      sharedFunctions.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.18
 *
 *      These are shared functions and constants across all modules, including
 *      common math algorithms.
 */

#include "sharedFunctions.h"
#include <math.h>
#include "assert.h"

unsigned const MULTIPLE = 2;

/*
static int logNumber = 0;

void SFLog(char *message, const char *file, int line)
{
        logNumber++;
        fprintf(stderr, "Log #%d from %s line %d: %s\n", logNumber, file, line,
                message);
}
 */

/* Forces a number into given range so it doesn't overflow */
float forceIntoRange(float number, float low, float high)
{
        float forced = number;
        
        if (number < low) {
                forced = low;
        } else if (number > high) {
                forced = high;
        }
        
        return forced;
}

/* Converts a float with given range to an int whose scale is determined by
 * number of bits
 */
int quantizeFloatSigned(float f, unsigned bits, float low, float high)
{
        f = forceIntoRange(f, low, high);
        
        float offset = (high + low) / 2.0;
        float fWithOffset = f - offset;
        
        double scale = (raise2(bits) - 2.0) / (high - low);
        int rounded = round(fWithOffset * scale);
        
        rounded = forceIntoRange(rounded, (int)(-raise2(bits - 1) + 1.0),
                                 (int)(raise2(bits - 1) - 1.0));
        
        assert(rounded > (int)(-raise2(bits - 1)) &&
               rounded < (int)(raise2(bits - 1)));
        
        return rounded;
}

float unquantizeFloatSigned(int i, unsigned bits, float low, float high)
{
        float offset = (high + low) / 2.0;
        double scale = (raise2(bits) - 2.0) / (high - low);
        
        float f = (i / scale) + offset;
        return f;
}

/* Returns 2 to the power of n */
uint64_t raise2(unsigned n)
{
        return ((uint64_t)1 << n);
}
