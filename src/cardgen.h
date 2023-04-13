/**
 * @file    cardgen.h
 * @author  Phil Lockett <phillockett65@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 'cardgen' is a playing card image generator.
 *
 * Common interfaces for the card generator.
 */

#if !defined _CARDGEN_H_INCLUDED_
#define _CARDGEN_H_INCLUDED_

#include <string>
#include "desc.h"

using namespace std;

#if !defined ELEMENTS
#define ELEMENTS(A) (sizeof(A)/sizeof(A[0]))
#endif

#if !defined ROUND
#define ROUND(v) (int)((v) + 0.5)
#endif


/**
 * @section Global variables.
 *
 */

extern int cardWidth;
extern int cardHeight;
extern string cardColour;

extern info indexInfo;
extern info cornerPipInfo;
extern info standardPipInfo;
extern info imagePipInfo;

extern string indexDirectory;
extern string pipDirectory;
extern string faceDirectory;

extern string scriptFilename;
extern string outputDirectory;
extern string refreshFilename;

extern bool keepAspectRatio;

extern float cornerRadius;
extern int radius;
extern int strokeWidth;
extern int borderOffset;
extern int outlineWidth;
extern int outlineHeight;

extern float boarderX;
extern float boarderY;
extern float originalImageWidth;
extern float originalImageHeight;
extern int originalWidthPX;
extern int originalHeightPX;

extern float winPX;
extern float winPY;

extern float imageWidth;
extern float imageHeight;
extern int widthPX;
extern int heightPX;
extern int offsetX;
extern int offsetY;
extern float imageX;
extern float imageY;


/**
 * @section Common functions.
 *
 */

extern void recalculate(void);
extern int init(int argc, char *argv[]);
extern int generateScript(int argc, char *argv[]);

#endif //!defined _CARDGEN_H_INCLUDED_

