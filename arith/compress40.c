/*
 *      compress40.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This calls functions from different modules to compress or decompress
 *      an image from a file.
 */

#include <stdio.h>
#include "compress40.h"
#include "ppm.h"
#include "compression.h"
#include "compressedformat.h"
#include "a2blocked.h"

/* We only use the blocked implemention of A2Methods */
#define a2methods uarray2_methods_blocked

extern void compress40(FILE *input)
{
        Pixmap pixmap = readPPM(input);
        
        ComponentsArray components = PixmapToComponents(pixmap);
        /* Pixmap trimmed and both freed */
        
        int width = a2methods->width(components);
        int height = a2methods->height(components);
        
        BlocksArray blocks = ComponentsToBlocks(components);
        a2methods->free(&components);
        
        CompressedArray cArray = BlocksToCompressedArray(blocks, width, height);
        UArray_free(&blocks);
        
        writeCompressed(cArray);
        freeCompressed(&cArray);
}

extern void decompress40(FILE *input)
{
        CompressedArray cArray = readCompressed(input);
        
        unsigned componentsWidth = cArray->originalWidth;
        unsigned componentsHeight = cArray->originalHeight;
        
        BlocksArray blocks = CompressedArrayToBlocks(cArray);
        ComponentsArray components = BlocksToComponents(blocks, componentsWidth,
                                                        componentsHeight);
        Pixmap pixmap = ComponentsToPixmap(components);
        writePPM(stdout, pixmap);
        
        a2methods->free(&components);
        freeCompressed(&cArray);
        UArray_free(&blocks);
        freePPM(&pixmap);
}
