/*
 *      40image.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This program reads in command line arguments and either compresses a PBM
 *      or decompresses a Comp 40 Compressed Format 2 file.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "input.h"

int main(int argc, char *argv[])
{
        FILE *file;
        CompressionOption cOption;
        determineParameters(argc, argv, &file, &cOption);
        
        if (cOption == COMPRESS) {
                compress40(file);
        } else if (cOption == DECOMPRESS) {
                decompress40(file);
        } else {
                exit(EXIT_FAILURE);
        }
        
        fclose(file);
        return 0;
}
