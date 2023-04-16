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

extern int cardWidthPx;
extern int cardHeightPx;
extern int cardBorderPx;
extern string cardColour;

extern info indexInfo;
extern info cornerPipInfo;
extern info standardPipInfo;
extern info imagePipInfo;

extern string indexDirectory;
extern string pipDirectory;
extern string faceDirectory;

extern string scriptFilename;
extern string refreshFilename;
extern string outputDirectory;

extern bool keepAspectRatio;
extern bool mpc;
const int mpcWidth{750};
const int mpcHeight{1050};
const int mpcBorder{36};

extern float cornerRadius;
extern int radius;
extern int strokeWidth;
extern int borderOffset;
extern int outlineWidth;
extern int outlineHeight;

extern float imageBorderX;
extern float imageBorderY;
extern float originalImageWidth;
extern float originalImageHeight;

extern float viewportWindowX;
extern float viewportWindowY;

extern float imageWidth;
extern float imageHeight;
extern int imageWidthPx;
extern int imageHeightPx;
extern int imageOffsetXPx;
extern int imageOffsetYPx;
extern float imagePipScale;
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

