/**
 * @file    cardgen.cpp
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
 * System entry point for the card generator.
 */

#include "cardgen.h"
#include <sys/stat.h>


/**
 * @section Global variables.
 *
 */

int cardWidthPx{380};
int cardHeightPx{532};
int cardBorderPx{};
string cardColour{"white"};

info indexInfo(10.5, 8.07, 9.84);
info cornerPipInfo(7.5, 8.07, 20.41);
info standardPipInfo(18.0, 25.7, 18.65);
info imagePipInfo(14.29, 12.63, 9.77);

string indexDirectory{"1"};
string pipDirectory{"1"};
string faceDirectory{"1"};

string scriptFilename{"draw.sh"};
string refreshFilename{"x_refresh.sh"};
string outputDirectory{};

bool keepAspectRatio{};
bool mpc{};
bool quad{};

float cornerRadius{3.76};
int radius{};
int strokeWidth{2};
int borderOffset{1};
int outlineWidth{};
int outlineHeight{};

float imageBorderX{14.54};
float imageBorderY{10.14};
float originalImageWidth{100 - (2 * imageBorderX)};
float originalImageHeight{50 - imageBorderY};

float viewportWindowX{};
float viewportWindowY{};

float imageWidth{};
float imageHeight{};
int imageWidthPx{};
int imageHeightPx{};
int imageOffsetXPx{};
int imageOffsetYPx{};
float imagePipScale{};
float imageX{};
float imageY{};

bool debug{};


/**
 * System entry point.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int main(int argc, char *argv[])
{
//- Get the command line parameters.
    int ret{init(argc, argv)};

    if (ret < 0)
        return 1;

//- If all is well, generate the script.
    if (!ret)
    {
        generateScript(argc, argv);

        // Ensure output scripts are executable.
        chmod(scriptFilename.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
    }

    return 0;
}

