/*
 *      rgbcomponentvideo.c
 *      for Comp 40 Homework 4
 *      by Mike Yeung and Madeleine Golison
 *      last updated on 2014.10.14
 *
 *      Converts among RGBs, RGB floats, and component videos.
 */

#include "rgbcomponentvideo.h"
#include <math.h>
#include "sharedFunctions.h"
#include "assert.h"

float scaledIntToFloat(unsigned scaled, unsigned denominator)
{
        return (float)scaled / (float)denominator;
}

RGBFloat RGBToRGBFloat(RGB rgb, unsigned denominator)
{
        RGBFloat floats;
        floats.red = scaledIntToFloat(rgb.red, denominator);
        floats.green = scaledIntToFloat(rgb.green, denominator);
        floats.blue = scaledIntToFloat(rgb.blue, denominator);
        return floats;
}

unsigned floatToScaledInt(float original, unsigned scale)
{
        int toReturn = roundf(original * scale);
        toReturn = forceIntoRange(toReturn, 0, (int)scale);
        return (unsigned)toReturn;
}

RGB RGBFloatToRGB(RGBFloat rgbFloat, unsigned scale)
{
        RGB rgb;
        rgb.red = floatToScaledInt(rgbFloat.red, scale);
        rgb.green = floatToScaledInt(rgbFloat.green, scale);
        rgb.blue = floatToScaledInt(rgbFloat.blue, scale);
        return rgb;
}

ComponentVideo RGBFloatToComponent(RGBFloat rgb)
{
        ComponentVideo component = {
                0.299 * rgb.red + 0.587 * rgb.green + 0.114 * rgb.blue,
                -0.168736 * rgb.red - 0.331264 * rgb.green + 0.5 * rgb.blue,
                0.5 * rgb.red - 0.418688 * rgb.green - 0.081312 * rgb.blue
        };
        
        return component;
}

RGBFloat ComponentToRGBFloat(ComponentVideo component)
{
        RGBFloat rgbFloat = {
                component.y + 1.402 * component.pr,
                component.y - 0.344136 * component.pb - 0.714136 * component.pr,
                component.y + 1.772 * component.pb
        };
        
        return rgbFloat;
}
