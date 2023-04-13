/**
 * @file    desc.cpp
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
 * Implementation for the info and desc classes.
 */

#include "cardgen.h"
#include "desc.h"

#include <arpa/inet.h>
#include <sstream>
#include <fstream>


/**
 * print function for info class.
 *
 * @param  os - output stream.
 */
void info::print(ostream &os) const
{
    os << H << "\t(" << X << ", " << Y << ")";
}


/**
 * Check validity of the .png file.
 *
 * @param  buffer - raw bytes of the image file.
 * @return true if valid, false otherwise.
 */
bool desc::isValidPNG(const char * const buffer)
{
    const uint8_t magic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    for (int i = 0; i < sizeof(magic); ++i)
    {
        if ((uint8_t)buffer[i] != magic[i])
            return false;
    }

    return true;
}


/**
 * Read the size of the image in a png file and populate the class.
 *
 * @return error value or 0 if no errors.
 */
int desc::getImageSize(void)
{
    FileFound = false;
    WidthPX = 1;
    HeightPX = 1;
    AspectRatio = 1;

//- Open the binary file.
    ifstream file(FileName, ifstream::in|ifstream::binary);

    if (!file.is_open())
    {
        return 1;
    }

//- Success. Read header data.
    char buffer[28];

    file.read(buffer, 24);
    if ((file) && (isValidPNG(buffer)))
    {
        FileFound = true;
        WidthPX = htonl(*(uint32_t *)(buffer+16));
        HeightPX = htonl(*(uint32_t *)(buffer+20));
        AspectRatio = float(WidthPX) / HeightPX;
    }

    file.close();

    return 0;
}


/**
 * Set up "DrawString"" for drawing the .png file with the correct size and position.
 *
 * @return error value or 0 if no errors.
 */
 int desc::genDrawString(void)
{
    if ((Height == 0) || (Width == 0))
    {
        DrawString = "";	// Don't draw anything here.

        return 0;
    }

    stringstream outputStream;
    outputStream  << "\t-draw \"image over " << OriginX << ',' << OriginY << ' ' << ROUND(Width) << ',' << ROUND(Height) << " '" << FileName << "'\" \\" << endl;
    DrawString = outputStream.str();

    return 0;
}


/**
 * Constructor.
 *
 * @param  H - Height of image as a percentage of card height.
 * @param  X - X value of centre of image as a percentage of card width.
 * @param  Y - Y value of centre of image as a percentage of card height.
 * @param  fileName - Name of image file.
 * @return true if valid, false otherwise.
 */
desc::desc(float H, float X, float Y, const string & fileName)
: FileName(fileName), FileFound(false)
{
    getImageSize();
    Height  = H * cardHeight / 100;
    Width   = Height * WidthPX / HeightPX;
    CentreX = X * cardWidth / 100;
    CentreY = Y * cardHeight / 100;
    OriginX = ROUND(centre2OriginX(CentreX));
    OriginY = ROUND(centre2OriginY(CentreY));
    genDrawString();
}


/**
 * Constructor.
 *
 * @param  I - Height and position of image as a percentage of card size.
 * @param  fileName - Name of image file.
 * @return true if valid, false otherwise.
 */
desc::desc(info & I, const string & fileName)
: FileName(fileName), FileFound(false)
{
    getImageSize();
    Height  = I.getH() * cardHeight / 100;
    Width   = Height * WidthPX / HeightPX;
    CentreX = I.getX() * cardWidth / 100;
    CentreY = I.getY() * cardHeight / 100;
    OriginX = ROUND(centre2OriginX(CentreX));
    OriginY = ROUND(centre2OriginY(CentreY));
    genDrawString();
}


/**
 * Reposition image and adjust internal values.
 *
 * @param  X - X value of centre of image as a percentage of card width.
 * @param  Y - Y value of centre of image as a percentage of card height.
 */
void desc::repos(float X, float Y)
{
    CentreX = X * cardWidth / 100;
    CentreY = Y * cardHeight / 100;
    OriginX = ROUND(centre2OriginX(CentreX));
    OriginY = ROUND(centre2OriginY(CentreY));
    genDrawString();
}


/**
 * Change image file image and adjust internal values.
 *
 * @param  fileName - Name of new image file.
 */
void desc::setFileName(const string & fileName)
{
    FileName = fileName;
    getImageSize();
    Width   = Height * WidthPX / HeightPX;
    OriginX = ROUND(centre2OriginX(CentreX));
    genDrawString();
}

