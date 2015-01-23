/*
 *      rgbcomponentvideo.h
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      Converts among RGBs, RGB floats, and component videos.
 */

#ifndef __HW4__rgbcomponentvideo__
#define __HW4__rgbcomponentvideo__

#include <stdio.h>
#include "pnm.h"

typedef struct Pnm_rgb RGB;

typedef struct {
        float red;
        float green;
        float blue;
} RGBFloat;

typedef struct {
        float y;        /* Brightness of pixel */
        float pb;       /* Blue component of y */
        float pr;       /* Red component of y */
} ComponentVideo;

float scaledIntToFloat(unsigned scaled, unsigned denominator);
RGBFloat RGBToRGBFloat(RGB rgb, unsigned denominator);
unsigned floatToScaledInt(float original, unsigned scale);
RGB RGBFloatToRGB(RGBFloat rgbFloat, unsigned scale);
ComponentVideo RGBFloatToComponent(RGBFloat rgb);
RGBFloat ComponentToRGBFloat(ComponentVideo component);

#endif /* defined(__HW4__rgbcomponentvideo__) */
