/*
 *      input.h
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      This determines parameters from the command line used in compression.
 */

#ifndef __HW4__input__
#define __HW4__input__

#include <stdio.h>
#include "compression.h"
#include "sharedFunctions.h"

void determineParameters(int argc, char *argv[], FILE **file,
                         CompressionOption *cOption);

#endif /* defined(__HW4__input__) */
