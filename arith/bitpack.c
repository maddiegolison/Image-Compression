/*
 *      Here is an example of my code from an assignment this semester. We were 
 *      provided an interface and had to implement given bitpacking functions.
 *      We utilized this module for a larger project that compresses and
 *      decompresses images.  The compressed image was four times smaller than its
 *      original size.
 *      Please let me know if you would like to see any of my other code samples
 *
 *      bitpack.c
 *      for Comp 40 Homework 4
 *      Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      Bitpack packs and unpacks bits into and from words. Functions include
 *      checking whether an int fits into n bits, packing one value into a word
 *      when given the width and lowest significant bit, its inverse, etc.
 */

#include <stdio.h>
#include <math.h>
#include "bitpack.h"
#include "assert.h"

unsigned const WORD_SIZE = 64;
Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* Returns 2 to the nth power */
static inline uint64_t raise2(unsigned n)
{
        return ((uint64_t)1 << n);
}

static inline uint64_t shiftLeft(uint64_t word, unsigned shift)
{
        return word << (uint64_t)shift;
}

static inline uint64_t shiftRight(uint64_t word, unsigned shift)
{
        return word >> (uint64_t)shift;
}

/* Returns whether n can be represented by max width bits */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width > 0 && width <= WORD_SIZE);
        if (width == 64) {
                /* Should always return true, but just in case */
                return (n <= (uint64_t)0 - (uint64_t)1);
        } else {
                return (n < raise2(width));
        }
}

/* Returns whether n can be represented by max width bits */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width > 0 && width <= WORD_SIZE);
        if (width == 64) {
                /* Should always return true, but just in case */
                return (n >= (int64_t)raise2(63) &&
                        n <= (int64_t)(raise2(63) - 1));
        }
        return (n >= (int64_t)raise2(width - 1) * (int64_t)-1 &&
                n < (int64_t)raise2(width - 1));
}

/* Returns value stored in particular location in word */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width != 0 && width <= WORD_SIZE && lsb + width <= WORD_SIZE);
        unsigned leftShift = WORD_SIZE - (width + lsb);
        return shiftRight(shiftLeft(word, leftShift), (lsb + leftShift));
}

/* Returns value stored in particular location in word */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        return Bitpack_getu(word, width, lsb) - (raise2(width) / 2);
}

/* Returns new word from old word with value packed into specified location */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
        assert(width <= WORD_SIZE && lsb + width <= WORD_SIZE);
        
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        
        uint64_t mask = -1;
        mask = ~(shiftRight(shiftLeft(mask, (WORD_SIZE - width)),
                            (WORD_SIZE - (lsb + width))));
        uint64_t maskedWord = word & mask;
        return maskedWord | shiftLeft(value, lsb);
}

/* Returns new word from old word with value packed into specified location */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value)
{
        assert(Bitpack_fitss(value, width));
        unsigned toStore = (unsigned)(value + (raise2(width) / 2));
        return Bitpack_newu(word, width, lsb, toStore);
}
