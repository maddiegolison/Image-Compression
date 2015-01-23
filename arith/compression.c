/*
 *      compression.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.18
 *
 *      This includes all the main functions dealing with both compressing and
 *      decompressing images. Functions are broken down into each algorithmic
 *      task, such as discrete cosine transform, and its inverse.
 */

#include "compression.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "assert.h"
#include "arith40.h"
#include "ppm.h"
#include <math.h>
#include <stdlib.h>

/* For when packing cosine coefficients */
unsigned const BCD_BITS = 5;
float const BCD_LO = -0.3;
float const BCD_HI = 0.3;

/* Used when converting to PPM */
unsigned const DENOMINATOR = 255;

/* We only use the block implementation of A2Methods */
#define a2methods uarray2_methods_blocked

/* Used in mapping functions */
typedef struct {
        A2Methods_UArray2 array;
        unsigned denominator;
        A2Methods_T const methods;
} Closure;

/* Returns true if array dimensions are multiples of MULTIPLE, now 2 */
bool isTrimmed(A2Methods_UArray2 array)
{
        return (a2methods->width(array) % MULTIPLE == 0 &&
                a2methods->height(array) % MULTIPLE == 0);
}

/* Returns a pointer to a ComponentVideo inside a block, for convenience */
ComponentVideo *CV(Block block, int i)
{
        return (ComponentVideo *)(UArray_at(block->componentVideos, i));
}

/* Changes values in block */
void discreteCosineTransform(Block block)
{
        assert(block);
        
        float y1 = CV(block, 0)->y;
        float y2 = CV(block, 1)->y;
        float y3 = CV(block, 2)->y;
        float y4 = CV(block, 3)->y;
        
        float a = (y4 + y3 + y2 + y1) / 4.0;
        float b = (y4 + y3 - y2 - y1) / 4.0;
        float c = (y4 - y3 + y2 - y1) / 4.0;
        float d = (y4 - y3 - y2 + y1) / 4.0;
        
        block->a = roundf(a * 511.0);
        block->b = quantizeFloatSigned(b, BCD_BITS, BCD_LO, BCD_HI);
        block->c = quantizeFloatSigned(c, BCD_BITS, BCD_LO, BCD_HI);
        block->d = quantizeFloatSigned(d, BCD_BITS, BCD_LO, BCD_HI);
}

/* Changes values in block */
void discreteCosineTransformInverse(Block block)
{
        assert(block);
        
        float a = block->a / 511.0;
        float b = unquantizeFloatSigned(block->b, BCD_BITS, BCD_LO, BCD_HI);
        float c = unquantizeFloatSigned(block->c, BCD_BITS, BCD_LO, BCD_HI);
        float d = unquantizeFloatSigned(block->d, BCD_BITS, BCD_LO, BCD_HI);
        
        CV(block, 0)->y = a - b - c + d;
        CV(block, 1)->y = a - b + c - d;
        CV(block, 2)->y = a + b - c - d;
        CV(block, 3)->y = a + b + c + d;
}

/* Calculates ComponentVideos from other values in block */
void calculateComponentVideos(Block block)
{
        assert(block);
        assert(block->componentVideos);
        
        discreteCosineTransformInverse(block);
        
        for (int i = 0; i < 4; i++) {
                CV(block, i)->pb = Arith40_chroma_of_index(block->pb);
                CV(block, i)->pr = Arith40_chroma_of_index(block->pr);
        }
}

/* Calculates Pb and Pr from other values in block */
void calculatePbPr(Block block)
{
        int numberOfPixels = MULTIPLE * MULTIPLE;
        float sumPb = 0;
        float sumPr = 0;
        
        for (int i = 0; i < numberOfPixels; i++) {
                sumPb += ((ComponentVideo *)
                          UArray_at(block->componentVideos, i))->pb;
                sumPr += ((ComponentVideo *)
                          UArray_at(block->componentVideos, i))->pr;
        }
        
        float avgPb = sumPb / (float)numberOfPixels;
        float avgPr = sumPr / (float)numberOfPixels;
        
        assert(avgPb >= -0.5 && avgPb <= 0.5 && avgPr >= -0.5 && avgPr <= 0.5);
        
        avgPb = forceIntoRange(avgPb, -0.3, 0.3);
        avgPr = forceIntoRange(avgPr, -0.3, 0.3);
        
        block->pb = Arith40_index_of_chroma(avgPb);
        block->pr = Arith40_index_of_chroma(avgPr);
}

/* Changes values in block */
void calculateBlockValues(Block block)
{
        discreteCosineTransform(block);
        calculatePbPr(block);
}

/* Used in mapping function */
void applyRGBToComponent(int col, int row, A2Methods_UArray2 array,
                         A2Methods_Object *object, void *closure)
{
        (void)array;
        Closure cl = *(Closure *)closure;
        RGB rgb = *((RGB *)cl.methods->at(cl.array, col, row));
        RGBFloat rgbFloat = RGBToRGBFloat(rgb, cl.denominator);
        *(ComponentVideo *)object = RGBFloatToComponent(rgbFloat);
}

ComponentsArray PixmapToComponents(Pixmap pixmap)
{
        Pixmap trimmed = trimPPM(pixmap, MULTIPLE);
        ComponentsArray componentVideos =
        a2methods->new_with_blocksize(trimmed->width,
                                      trimmed->height,
                                      sizeof(ComponentVideo),
                                      MULTIPLE);
        Closure closure = {
                trimmed->pixels,
                trimmed->denominator,
                a2methods
        };
        
        a2methods->map_block_major(componentVideos, applyRGBToComponent,
                                   &closure);
        freePPM(&trimmed);
        return componentVideos;
}

/* Used in map function */
void applyComponentToRGB(int col, int row, A2Methods_UArray2 array,
                         A2Methods_Object *object, void *closure)
{
        (void)array;
        Closure cl = *(Closure *)closure;
        ComponentVideo component = *((ComponentVideo *)
                                     cl.methods->at(cl.array, col, row));
        RGBFloat rgbFloat = ComponentToRGBFloat(component);
        *(RGB *)object = RGBFloatToRGB(rgbFloat, cl.denominator);
}

Pixmap ComponentsToPixmap(ComponentsArray components)
{
        int width = a2methods->width(components);
        int height = a2methods->height(components);
        
        Closure closure = {
                components,
                DENOMINATOR,
                a2methods
        };
        
        Pixmap pixmap = newPPM(width, height, DENOMINATOR, MULTIPLE);
        a2methods->map_block_major(pixmap->pixels, applyComponentToRGB,
                                   &closure);
        return pixmap;
}

BlocksArray ComponentsToBlocks(ComponentsArray components)
{
        assert(isTrimmed(components));
        unsigned componentsWidth = a2methods->width(components);
        unsigned componentsHeight = a2methods->height(components);
        unsigned blockLength = (componentsWidth / MULTIPLE) *
        (componentsHeight / MULTIPLE);
        
        BlocksArray blocks = UArray_new(blockLength, sizeof(struct Block));
        
        unsigned blockIndex = 0;
        unsigned componentIndex = 0;
        
        for (unsigned rOut = 0; rOut < componentsHeight; rOut += MULTIPLE) {
                for (unsigned cOut = 0; cOut < componentsWidth;
                     cOut += MULTIPLE) {
                        /* For every block */
                        Block thisBlock = (Block)UArray_at(blocks, blockIndex);
                        thisBlock->componentVideos =
                        UArray_new(MULTIPLE * MULTIPLE,
                                   sizeof(ComponentVideo));
                        componentIndex = 0;
                        for (unsigned rIn = rOut; rIn < rOut + MULTIPLE;
                             rIn++) {
                                for (unsigned cIn = cOut; cIn < cOut + MULTIPLE;
                                     cIn++) {
                                        *((ComponentVideo *)
                                          UArray_at(thisBlock->componentVideos,
                                                    componentIndex)) =
                                        *((ComponentVideo *)
                                          (a2methods->at(components,
                                                         cIn, rIn)));
                                        componentIndex++;
                                }
                        }
                        blockIndex++;
                }
        }
        return blocks;
}

ComponentsArray BlocksToComponents(BlocksArray blocks, unsigned width,
                                   unsigned height)
{
        ComponentsArray components =
        a2methods->new_with_blocksize(width, height, sizeof(ComponentVideo),
                                      MULTIPLE);
        unsigned blockIndex = 0;
        unsigned componentIndex = 0;
        
        for (unsigned rOut = 0; rOut < height; rOut += MULTIPLE) {
                for (unsigned cOut = 0; cOut < width; cOut += MULTIPLE) {
                        /* For every block */
                        Block thisBlock = (Block)UArray_at(blocks, blockIndex);
                        componentIndex = 0;
                        for (unsigned rIn = rOut; rIn < rOut + MULTIPLE;
                             rIn++) {
                                for (unsigned cIn = cOut; cIn < cOut + MULTIPLE;
                                     cIn++) {
                                        *((ComponentVideo *)
                                          (a2methods->at(components,
                                                         cIn, rIn))) =
                                        *((ComponentVideo *)
                                          UArray_at(thisBlock->componentVideos,
                                                    componentIndex));
                                        componentIndex++;
                                }
                        }
                        UArray_free(&(thisBlock->componentVideos));
                        blockIndex++;
                }
        }
        return components;
}

CompressedArray BlocksToCompressedArray(BlocksArray blocks, int width,
                                        int height)
{
        CompressedArray compressedArray = malloc(sizeof(struct
                                                        CompressedArray));
        compressedArray->originalWidth = width;
        compressedArray->originalHeight = height;
        compressedArray->array = UArray_new(UArray_length(blocks),
                                            sizeof(Compressed));
        Block thisBlock;
        
        for (int i = 0; i < UArray_length(blocks); i++) {
                thisBlock = (Block)UArray_at(blocks, i);
                calculateBlockValues(thisBlock);
                *(Compressed *)UArray_at(compressedArray->array, i) =
                        packBlock(thisBlock);
                UArray_free(&(thisBlock->componentVideos));
        }
        
        return compressedArray;
}

BlocksArray CompressedArrayToBlocks(CompressedArray cArray)
{
        int compressedsLength = UArray_length(cArray->array);
        BlocksArray blocks = UArray_new(compressedsLength,
                                        sizeof(struct Block));
        
        Block unpackedWord;
        for (int i = 0; i < compressedsLength; i++) {
                unpackedWord = unpackWord(*(Compressed *)
                                          UArray_at(cArray->array, i));
                *(Block)UArray_at(blocks, i) = *unpackedWord;
                free(unpackedWord);
        }
        
        return blocks;
}

Compressed packBlock(Block block)
{
        Compressed word = 0;
        word = (Compressed)Bitpack_newu(word, 4, 0, block->pr);
        word = (Compressed)Bitpack_newu(word, 4, 4, block->pb);
        word = (Compressed)Bitpack_news(word, 5, 8, block->d);
        word = (Compressed)Bitpack_news(word, 5, 13, block->c);
        word = (Compressed)Bitpack_news(word, 5, 18, block->b);
        word = (Compressed)Bitpack_newu(word, 9, 23, block->a);
        return  word;
}

Block unpackWord(Compressed word)
{
        Block block = malloc(sizeof(struct Block));
        block->a = (unsigned)Bitpack_getu(word, 9, 23);
        block->b = (int)Bitpack_gets(word, 5, 18);
        block->c = (int)Bitpack_gets(word, 5, 13);
        block->d = (int)Bitpack_gets(word, 5, 8);
        block->pb = (unsigned)Bitpack_getu(word, 4, 4);
        block->pr = (unsigned)Bitpack_getu(word, 4, 0);
        block->componentVideos = UArray_new(MULTIPLE * MULTIPLE,
                                            sizeof(ComponentVideo));
        calculateComponentVideos(block);
        return block;
}
