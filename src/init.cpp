/**
 * @file    init.cpp
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
 * Command line parameter initialisation for the card generator.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "cardgen.h"
#include "config.h"

#include <iostream>


/**
 * Display help message.
 */
static void help(void)
{
    cout << "Usage: " << PACKAGE_NAME << " [Options]" << endl;
    cout << "  Generates the bash script \"" << scriptFilename << "\" which draws a pack of playing cards."<< endl;
    cout << endl;
    cout << "  Options:" << endl;
    cout << "\t--help \t\t\t\tThis help page and nothing else." << endl;
    cout << "\t-v --version \t\t\tDisplay version." << endl;
    cout << endl;
    cout << "\t-i --index directory \t\tSubdirectory of indices to use (default: \"" << indexDirectory << "\")." << endl;
    cout << "\t-p --pip directory \t\tSubdirectory of pips to use (default: \"" << pipDirectory << "\")." << endl;
    cout << "\t-f --face directory \t\tSubdirectory of faces to use (default: \"" << faceDirectory << "\")." << endl;
    cout << endl;
    cout << "\t-s --script filename \t\tScript filename (default: \"" << scriptFilename << "\")." << endl;
    cout << "\t-o --output directory \t\tOutput filename (default: same directory name as face)." << endl;
    cout << "\t-w --width integer \t\tCard width in pixels (default: " << cardWidthPx << ")." << endl;
    cout << "\t-h --height integer \t\tCard height in pixels (default: " << cardHeightPx << ")." << endl;
    cout << "\t-c --colour name \t\tBackground colour name (defined at: http://www.imagemagick.org/script/color.php, default: \"" << cardColour << "\")." << endl;
    cout << "\t-a --KeepAspectRatio \t\tKeep image Aspect Ratio (default: " << (keepAspectRatio ? "true" : "false") << ")." << endl;
    cout << "\t-m --MPC \t\t\tGenerate MakePlayingCards image (default: " << (mpc ? "true" : "false") << ", see: https://www.makeplayingcards.com/)." << endl;
    cout << endl;
    cout << "\t--IndexHeight value \t\tHeight of index as a % of card height (default: " << indexInfo.getH() << ")." << endl;
    cout << "\t--IndexCentreX value \t\tX value of centre of index as a % of card width (default: " << indexInfo.getX() << ")." << endl;
    cout << "\t--IndexCentreY value \t\tY value of centre of index as a % of card height (default: " << indexInfo.getY() << ")." << endl;
    cout << "\t--CornerPipHeight value \tHeight of corner pip as a % of card height (default: " << cornerPipInfo.getH() << ")." << endl;
    cout << "\t--CornerPipCentreX value \tX value of centre of corner pip as a % of card width (default: " << cornerPipInfo.getX() << ")." << endl;
    cout << "\t--CornerPipCentreY value \tY value of centre of corner pip as a % of card height (default: " << cornerPipInfo.getY() << ")." << endl;
    cout << "\t--StandardPipHeight value \tHeight of standard pip as a % of card height (default: " << standardPipInfo.getH() << ")." << endl;
    cout << "\t--StandardPipCentreX value \tX value of centre of standard pip as a % of card width (default: " << standardPipInfo.getX() << ")." << endl;
    cout << "\t--StandardPipCentreY value \tY value of centre of standard pip as a % of card height (default: " << standardPipInfo.getY() << ")." << endl;
    cout << "\t--ImageBorderX value \t\tImage Border in X direction as a % of card width (default: " << imageBorderX << ")." << endl;
    cout << "\t--ImageBorderY value \t\tImage Border in Y direction as a % of card height (default: " << imageBorderY << ")." << endl;
    cout << "\t--ImagePipOff \t\t\tDon't display image pip on the court cards." << endl;
    cout << "\t--ImagePipHeight value \t\tHeight of image pip as a % of card height (default: " << imagePipInfo.getH() << ")." << endl;
    cout << "\t--ImagePipCentreX value \tX value of centre of image pip as a % of card width relative to ImageBorderX (default: " << imagePipInfo.getX() << ")." << endl;
    cout << "\t--ImagePipCentreY value \tY value of centre of image pip as a % of card height relative to ImageBorderY (default: " << imagePipInfo.getY() << ")." << endl;
    cout << endl;
    cout << "\t--CentreX value \t\tShortcut for: --IndexCentreX value --CornerPipCentreX value." << endl;
    cout << "\t--Inputs value \t\t\tShortcut for: -f value -p value -i value." << endl;
}


/**
 * Display version message.
 */
static void version(void)
{
    cout << "Version " VERSION " of " << PACKAGE_NAME << endl;
    cout << "Feedback to: " PACKAGE_BUGREPORT << endl;
}


/**
 * Process command line parameters with help from getopt_long() and update
 * global variables.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
static int parseCommandLine(int argc, char *argv[])
{
    while (1)
    {
        int option_index{};
        int optchr{};

        static struct option long_options[]
        {
            {"width",   required_argument,0,'w'},
            {"height",  required_argument,0,'h'},
            {"colour",  required_argument,0,'c'},

            {"index",   required_argument,0,'i'},
            {"pip",     required_argument,0,'p'},
            {"face",    required_argument,0,'f'},

            {"script",  required_argument,0,'s'},
            {"output",  required_argument,0,'o'},
            {"help",    no_argument,0,0},
            {"KeepAspectRatio",  no_argument,0,'a'},
            {"MPC",     no_argument,0,'m'},

            {"IndexHeight", required_argument,0,1},
            {"IndexCentreX", required_argument,0,2},
            {"IndexCentreY", required_argument,0,3},

            {"CornerPipHeight", required_argument,0,4},
            {"CornerPipCentreX", required_argument,0,5},
            {"CornerPipCentreY", required_argument,0,6},

            {"StandardPipHeight", required_argument,0,7},
            {"StandardPipCentreX", required_argument,0,8},
            {"StandardPipCentreY", required_argument,0,9},

            {"ImageBorderX", required_argument,0,10},
            {"ImageBorderY", required_argument,0,11},
            {"ImagePipOff", no_argument,0,12},
            {"ImagePipHeight", required_argument,0,13},
            {"ImagePipCentreX", required_argument,0,14},
            {"ImagePipCentreY", required_argument,0,15},

            {"CentreX", required_argument,0,16},
            {"Inputs", required_argument,0,17},
            {"version", no_argument,0,'v'},
            {0,0,0,0}
        };

        optchr = getopt_long(argc, argv ,"w:h:c:i:p:f:s:o:amv", long_options, &option_index);
        if (optchr == -1)
            return 0;

        switch (optchr)
        {
            case 'w': cardWidthPx = atoi(optarg);           break;
            case 'h': cardHeightPx = atoi(optarg);          break;
            case 'c': cardColour = string(optarg);          break;

            case 'i': indexDirectory = string(optarg);      break;
            case 'p': pipDirectory = string(optarg);        break;
            case 'f': faceDirectory = string(optarg);       break;

            case 's': scriptFilename = string(optarg);      break;
            case 'o': outputDirectory = string(optarg);     break;

            case 'a': keepAspectRatio = true;               break;
            case 'm': mpc = true;                           break;

            case 1:   indexInfo.setH(atof(optarg));         break;
            case 2:   indexInfo.setX(atof(optarg));         break;
            case 3:   indexInfo.setY(atof(optarg));         break;

            case 4:   cornerPipInfo.setH(atof(optarg));     break;
            case 5:   cornerPipInfo.setX(atof(optarg));     break;
            case 6:   cornerPipInfo.setY(atof(optarg));     break;

            case 7:   standardPipInfo.setH(atof(optarg));   break;
            case 8:   standardPipInfo.setX(atof(optarg));   break;
            case 9:   standardPipInfo.setY(atof(optarg));   break;

            case 10:  imageBorderX = atof(optarg);          break;
            case 11:  imageBorderY = atof(optarg);          break;
            case 12:  imagePipInfo.setH(0);                 break;
            case 13:  imagePipInfo.setH(atof(optarg));      break;
            case 14:  imagePipInfo.setX(atof(optarg));      break;
            case 15:  imagePipInfo.setY(atof(optarg));      break;

            case 16:
                indexInfo.setX(atof(optarg));
                cornerPipInfo.setX(atof(optarg));
                break;

            case 17:
                indexDirectory = string(optarg);
                pipDirectory   = string(optarg);
                faceDirectory  = string(optarg);
                break;

            case 'v':   version();  return 2;

            case 0:     help();     return 1;

            default:    help();     return -1;
        }//end switch
    }//end while

    return 0;
}


/**
 * Recalculate global variables using command line input.
 *
 */
void recalculate(void)
{
//- Set up for Make Playing Cards output requirements.
    if (mpc)
    {
        cardWidthPx  = mpcWidth;
        cardHeightPx = mpcHeight;
        cardBorderPx = mpcBorder;
        cornerRadius = 0.0;
    }

//- Card outline values in pixels.
    radius = ROUND(cornerRadius * cardHeightPx / 100);
    outlineWidth = cardWidthPx-borderOffset-1;
    outlineHeight = cardHeightPx-borderOffset-1;

//- Calculate viewport window size as percentages of the card size. In this
//  context the viewport is the area of the card not occupied by the standard
//  pip borders.
    viewportWindowX = (100.0F - (2.0F * standardPipInfo.getX()));
    viewportWindowY = (100.0F - (2.0F * standardPipInfo.getY()));

//- Card face image values in pixels.
    imageWidth  = 100 - (2 * imageBorderX);
    imageHeight = 50 - imageBorderY;
    imageWidthPx    = ROUND(imageWidth * cardWidthPx / 100);
    imageHeightPx   = ROUND(imageHeight * cardHeightPx / 100);
    imageOffsetXPx  = ROUND(imageBorderX * cardWidthPx / 100);
    imageOffsetYPx  = ROUND(imageBorderY * cardHeightPx / 100);

    const float scaleX{imageWidth/originalImageWidth};
    const float scaleY{imageHeight/originalImageHeight};
    imagePipScale = (scaleX < scaleY) ? scaleX : scaleY;

    imageX      = 50;
    imageY      = imageBorderY + (imageHeight / 2);

//- If "outputDirectory" isn't explicitly set, use "face".
    if (!outputDirectory.length())
        outputDirectory = faceDirectory;
}


#if 0
#define DEBUG
static void dumpValues(void)
{
	cout << "cardWidthPx\t" << cardWidthPx << endl;
	cout << "cardHeightPx\t" << cardHeightPx << endl;
	cout << "cardBorderPx\t" << cardBorderPx << endl;
	cout << "cardColour\t" << cardColour << endl;
	cout << endl;
	cout << "indexInfo\t" << indexInfo << endl;
	cout << "cornerPipInfo\t" << cornerPipInfo << endl;
	cout << "standardPipInfo\t" << standardPipInfo << endl;
	cout << "imagePipInfo\t" << imagePipInfo << endl;
	cout << endl;
	cout << "indexDirectory\t" << indexDirectory << endl;
	cout << "pipDirectory\t" << pipDirectory << endl;
	cout << "faceDirectory\t" << faceDirectory << endl;
	cout << endl;
	cout << "scriptFilename\t" << scriptFilename << endl;
	cout << "refreshFilename\t" << refreshFilename << endl;
	cout << "outputDirectory\t" << outputDirectory << endl;
	cout << endl;
	cout << "keepAspectRatio\t" << keepAspectRatio << endl;
	cout << "MPC\t\t" << mpc << endl;
	cout << endl;
	cout << "cornerRadius\t" << cornerRadius << endl;
	cout << "radius\t\t" << radius << endl;
	cout << "strokeWidth\t" << strokeWidth << endl;
	cout << "borderOffset\t" << borderOffset << endl;
	cout << "outlineWidth\t" << outlineWidth << endl;
	cout << "outlineHeight\t" << outlineHeight << endl;
	cout << endl;
	cout << "imageBorderX\t" << imageBorderX << endl;
	cout << "imageBorderY\t" << imageBorderY << endl;
	cout << "originalImageWidth\t" << originalImageWidth << endl;
	cout << "originalImageHeight\t" << originalImageHeight << endl;
	cout << endl;
	cout << "viewportWindowX\t" << viewportWindowX << endl;
	cout << "viewportWindowY\t" << viewportWindowY << endl;
	cout << endl;
	cout << "imageWidth\t" << imageWidth << endl;
	cout << "imageHeight\t" << imageHeight << endl;
	cout << "imageWidthPx\t" << imageWidthPx << endl;
	cout << "imageHeightPx\t" << imageHeightPx << endl;
	cout << "imageOffsetXPx\t" << imageOffsetXPx << endl;
	cout << "imageOffsetYPx\t" << imageOffsetYPx << endl;
	cout << "imageX\t\t" << imageX << endl;
	cout << "imageY\t\t" << imageY << endl;
}
#endif


/**
 * Initialise cardgen using command line input and recalculate global
 * variables.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int init(int argc, char *argv[])
{
//- Process command line input.
    int ret{parseCommandLine(argc, argv)};
    if (!ret)
        recalculate();

#if defined DEBUG
    dumpValues();
#endif

    return ret;
}

