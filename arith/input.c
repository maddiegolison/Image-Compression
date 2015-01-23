/*
 *      input.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This determines parameters from the command line used in compression.
 */

#include "input.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"

void determineParameters(int argc, char *argv[], FILE **file,
                         CompressionOption *cOption)
{
        int i;
        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        *cOption = COMPRESS;
                } else if (strcmp(argv[i], "-d") == 0) {
                        *cOption = DECOMPRESS;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s -d [filename]\n"
                                "       %s -c [filename]\n",
                                argv[0], argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                *file = fopen(argv[i], "r");
                assert(*file != NULL);
        } else {
                *file = stdin;
        }
}
