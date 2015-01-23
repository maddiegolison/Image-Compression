/*
 *      compression.h
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This includes all the main functions dealing with both compressing and
 *      decompressing images. Functions are broken down into each algorithmic
 *      task, such as discrete cosine transform, and its inverse.
 */

#ifndef __HW4__compression__
#define __HW4__compression__

#include <stdio.h>
#include "a2methods.h"
#include "ppm.h"
#include "rgbcomponentvideo.h"
#include "bitpack.h"
#include "uarray.h"
#include "sharedFunctions.h"
#include "compressedformat.h"

typedef UArray_T BlocksArray;
typedef A2Methods_UArray2 ComponentsArray;    /* 2D array of ComponentVideos */
typedef UArray_T pixelArray;                  /* For ComponentVideos in block */
typedef struct Block {                        /* Each 2 by 2 block of pixels */
        pixelArray componentVideos;
        unsigned a;
        int b;
        int c;
        int d;
        unsigned pr;
        unsigned pb;
} *Block;

/* Returns true if array dimensions are multiples of MULTIPLE, now 2 */
bool isTrimmed(A2Methods_UArray2 array);

ComponentsArray PixmapToComponents(Pixmap pixmap);
Pixmap ComponentsToPixmap(ComponentsArray components);

BlocksArray ComponentsToBlocks(ComponentsArray components);
ComponentsArray BlocksToComponents(BlocksArray blocks, unsigned width,
                                   unsigned height);

CompressedArray BlocksToCompressedArray(BlocksArray blocks, int width,
                                        int height);
BlocksArray CompressedArrayToBlocks(CompressedArray cArray);

Compressed packBlock(Block block);
Block unpackWord(Compressed word);

#endif /* defined(__HW4__compression__) */
