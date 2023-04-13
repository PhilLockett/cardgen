/**
 * @file    desc.h
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
 * Interface for the info and desc classes.
 */

#if !defined _DESC_H_INCLUDED_
#define _DESC_H_INCLUDED_

#include <iostream>
#include <string>

using namespace std;


/**
 * @section info class.
 *
 * Used to encapsulate position and size information of an image.
 */
class info
{
private:
    float H;
    float X;
    float Y;
    bool ChangedH;
    bool ChangedX;
    bool ChangedY;

public:
    info(float h, float x, float y) : H(h), X(x), Y(y), ChangedH(false), ChangedX(false), ChangedY(false) {}
    info(const info & a) : H(a.H), X(a.X), Y(a.Y), ChangedH(false), ChangedX(false), ChangedY(false) {}

    float getH(void) { return H; }
    float getX(void) { return X; }
    float getY(void) { return Y; }

    bool isChangedH(void) { return ChangedH; }
    bool isChangedX(void) { return ChangedX; }
    bool isChangedY(void) { return ChangedY; }

    void setH(float v) { ChangedH = true; H = v; }
    void setX(float v) { ChangedX = true; X = v; }
    void setY(float v) { ChangedY = true; Y = v; }

    friend ostream & operator<<(ostream &os, const info &A) { A.print(os); return os; }

protected:
    void print(ostream &os=cout) const;

};


/**
 * @section desc class.
 *
 * Used to capture image file info and the image info on the canvas.
 */
class desc
{
private:
    bool isValidPNG(const char * const buffer);
    int getImageSize(void);
    int genDrawString(void);

    string DrawString;
    bool FileFound;
    int WidthPX;
    int HeightPX;
    float AspectRatio;
    int OriginX;
    int OriginY;

    float CentreX;
    float CentreY;
    float Height;
    float Width;

    string FileName;

public:
    desc(float H, float X, float Y, const string & FN);
    desc(info & I, const string & FN);
    void repos(float X, float Y);
    void setFileName(const string & fileName);
    const string & getFileName(void) const { return FileName; }

    float getCentreX(void) const { return CentreX; }
    float getCentreY(void) const { return CentreY; }
    float getHeight(void) const { return Height; }
    float getWidth(void) const { return Width; }

    int getHeightPX(void) const { return HeightPX; }
    int getWidthPX(void) const { return WidthPX; }
    float getAspectRatio(void) const { return AspectRatio; }
    int getOriginX(void) const { return OriginX; }
    int getOriginY(void) const { return OriginY; }
    const string & draw(void) const { return DrawString; }

    float centre2OriginX(float centre) const { return centre - (Width/2); }
    float centre2OriginY(float centre) const { return centre - (Height/2); }

    bool isLandscape(void) const { return HeightPX < WidthPX; }
    bool isFileFound(void) const { return FileFound; }
    bool useStandardPips(void) const { return !FileFound; }

};

#endif //!defined _DESC_H_INCLUDED_

