/*
 *      ppm.h
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This creates, reads, trims, writes, and frees PPM files.
 */

#ifndef __HW4__ppm__
#define __HW4__ppm__

#include <stdio.h>
#include "pnm.h"

typedef Pnm_ppm Pixmap;

Pixmap newPPM(unsigned width, unsigned height, unsigned denominator,
              int blockSize);
Pixmap readPPM(FILE *file);

/* Trims a pixmap to dimensions that are multiples of multiple */
Pixmap trimPPM(Pixmap pixmap, unsigned multiple);

void writePPM(FILE *file, Pixmap pixmap);
void freePPM(Pixmap *pixmap);

#endif /* defined(__HW4__ppm__) */
