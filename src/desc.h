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
    info(float h, float x, float y) : H(h), X(x), Y(y), ChangedH{}, ChangedX{}, ChangedY{} {}
    info(const info & a) : H(a.H), X(a.X), Y(a.Y), ChangedH{}, ChangedX{}, ChangedY{} {}

    float getH(void) const { return H; }
    float getX(void) const { return X; }
    float getY(void) const { return Y; }

    std::string getHStr(void) const { return std::to_string(H); }
    std::string getXStr(void) const { return std::to_string(X); }
    std::string getYStr(void) const { return std::to_string(Y); }

    bool isChangedH(void) const { return ChangedH; }
    bool isChangedX(void) const { return ChangedX; }
    bool isChangedY(void) const { return ChangedY; }

    void setH(float v) { ChangedH = true; H = v; }
    void setX(float v) { ChangedX = true; X = v; }
    void setY(float v) { ChangedY = true; Y = v; }

    friend std::ostream & operator<<(std::ostream &os, const info &A) { A.print(os); return os; }

protected:
    void print(std::ostream &os=std::cout) const;

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

    std::string drawString;
    std::string wardString;
    int imageWidthPx;
    int imageHeightPx;
    float aspectRatio;
    int portOriginXPx;
    int portOriginYPx;

    float portCentreXPx;
    float portCentreYPx;
    float portHeightPx;
    float portWidthPx;

    std::string fileName;
    bool fileFound;

public:
    desc(float H, float X, float Y, const std::string & FN);
    desc(const info & I, const std::string & FN);
    void reposition(float X, float Y);
    void setFileName(const std::string & fileName);
    const std::string & getFileName(void) const { return fileName; }

    float getPortCentreXPx(void) const { return portCentreXPx; }
    float getPortCentreYPx(void) const { return portCentreYPx; }
    float getPortHeightPx(void) const { return portHeightPx; }
    float getPortWidthPx(void) const { return portWidthPx; }

    int getImageHeightPx(void) const { return imageHeightPx; }
    int getImageWidthPx(void) const { return imageWidthPx; }
    float getAspectRatio(void) const { return aspectRatio; }
    int getPortOriginXPx(void) const { return portOriginXPx; }
    int getPortOriginYPx(void) const { return portOriginYPx; }
    const std::string & draw(int left = true) const { return left ? drawString : wardString; }

    float centre2OriginX(float centre) const { return centre - (getPortWidthPx()/2); }
    float centre2OriginY(float centre) const { return centre - (getPortHeightPx()/2); }

    bool isLandscape(void) const { return getImageHeightPx() < getImageWidthPx(); }
    bool isFileFound(void) const { return fileFound; }
    bool useStandardPips(void) const { return !isFileFound(); }

};

#endif //!defined _DESC_H_INCLUDED_

