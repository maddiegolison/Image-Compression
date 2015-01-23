/*
 *      ppm.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This creates, reads, trims, writes, and frees PPM files.
 */

#include "ppm.h"
#include "a2methods.h"
#include "a2blocked.h"
#include <stdlib.h>

Pixmap newPPM(unsigned width, unsigned height, unsigned denominator,
              int blockSize)
{
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm image = malloc(sizeof(struct Pnm_ppm));
        image->width = width;
        image->height = height;
        image->denominator = denominator;
        image->pixels = methods->new_with_blocksize(width, height,
                                                    sizeof(struct Pnm_rgb),
                                                    blockSize);
        image->methods = methods;
        return image;
}

Pixmap readPPM(FILE *file)
{
        return Pnm_ppmread(file, uarray2_methods_blocked);
}

/* Used in map function */
void applyTrim(int col, int row, A2Methods_UArray2 array2,
               A2Methods_Object *object, void *closure)
{
        (void)array2;
        A2Methods_UArray2 toTrim = (A2Methods_UArray2)closure;
        *((Pnm_rgb)object) = *((Pnm_rgb)uarray2_methods_blocked->at(toTrim,
                                                                    col, row));
}

/* Trims a pixmap to dimensions that are multiples of multiple */
Pixmap trimPPM(Pixmap pixmap, unsigned multiple)
{
        unsigned widthRemainder = pixmap->width % multiple;
        unsigned heightRemainder = pixmap->height % multiple;
        
        if (widthRemainder == 0 && heightRemainder % multiple == 0) {
                return pixmap;
        }
        
        Pixmap newPixmap = newPPM(pixmap->width - widthRemainder,
                                  pixmap->height - heightRemainder,
                                  pixmap->denominator, multiple);
        
        uarray2_methods_blocked->map_block_major(newPixmap->pixels, applyTrim,
                                                 pixmap->pixels);
        freePPM(&pixmap);
        return newPixmap;
}

void writePPM(FILE *file, Pixmap pixmap)
{
        Pnm_ppmwrite(file, pixmap);
        printf("\n");
}

void freePPM(Pixmap *pixmap)
{
        Pnm_ppmfree(pixmap);
}
