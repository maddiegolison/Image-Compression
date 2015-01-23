/*
 *      compressedformat.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This reads, writes, and frees CompressedArrays, which are arrays of
 *      packed words.
 */

#include <stdlib.h>
#include "compressedformat.h"
#include "a2methods.h"
#include "assert.h"
#include "a2blocked.h"
#include "bitpack.h"
#include "sharedFunctions.h"

int const WORD_LEN = 32;
int const CHAR_LEN = 8;

/* Reads and returns next word (3 chars) in file in big-endian order */
Compressed readNextWord(FILE *file)
{
        Compressed word = 0;
        unsigned nextChar;
        
        for (int lsb = WORD_LEN - CHAR_LEN; lsb >= 0; lsb -= CHAR_LEN) {
                nextChar = fgetc(file);
                assert(nextChar != (unsigned)EOF);
                word = (Compressed)Bitpack_newu(word, CHAR_LEN, lsb, nextChar);
        }
        
        return word;
}

/* Reads compressed image file and stores and returns the array of words */
CompressedArray readCompressed(FILE *file)
{
        assert(file);
        unsigned height, width;
        int read = fscanf(file, "COMP40 Compressed image format 2\n%u %u",
                          &width, &height);
        assert(read == 2);
        int c = getc(file);
        assert(c == '\n');
        assert(width % MULTIPLE == 0 && height % MULTIPLE == 0);
        assert(width != 0 || height != 0);
        unsigned numberOfWords = (width * height) / (MULTIPLE * MULTIPLE);
        
        CompressedArray array = malloc(sizeof(struct CompressedArray));
        array->array = UArray_new(numberOfWords, sizeof(Compressed));
        array->originalWidth = width;
        array->originalHeight = height;
        
        for (unsigned i = 0; i < numberOfWords; i++) {
                *(Compressed *)UArray_at(array->array, i) = readNextWord(file);
        }
        
        return array;
}

/* Prints one word (3 chars) into stdout in big-endian order */
void printWord(Compressed word)
{
        for (int lsb = WORD_LEN - CHAR_LEN; lsb >= 0; lsb -= CHAR_LEN) {
                putchar((unsigned)Bitpack_getu(word, CHAR_LEN, lsb));
        }
}

/* Prints the compressed format from the array of words in row-major order */
void writeCompressed(CompressedArray array)
{
        assert(array);
        
        printf("COMP40 Compressed image format 2\n%u %u\n",
               array->originalWidth, array->originalHeight);
        
        UArray_T compresseds = array->array;
        
        for (int i = 0; i < UArray_length(compresseds); i++) {
                printWord(*(Compressed *)UArray_at(compresseds, i));
        }
        
        printf("\n");
}

void freeCompressed(CompressedArray *array)
{
        UArray_free(&((*array)->array));
        free(*array);
}
