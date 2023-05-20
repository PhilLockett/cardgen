/**
 * @file    Configuration.h
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
 * Command line parameter initialisation.
 */

#if !defined(_CONFIGURATION_H__20230519_1343__INCLUDED_)
#define _CONFIGURATION_H__20230519_1343__INCLUDED_

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <tuple>

#include "config.h"
#include "cardgen.h"


/**
 * @section duit configuration Singleton.
 */

class Config
{
private:
//- Hide the default constructor and destructor.
    Config(void) : 
        cardWidthPx{_cardWidthPx},
        cardHeightPx{_cardHeightPx},
        cardBorderPx{},
        cardColour{_cardColour},

        indexInfo{_indexInfo},
        cornerPipInfo{_cornerPipInfo},
        standardPipInfo{_standardPipInfo},
        imagePipInfo{_imagePipInfo},

        indexDirectory{_indexDirectory},
        pipDirectory{_pipDirectory},
        faceDirectory{_faceDirectory},

        scriptFilename{_scriptFilename},
        refreshFilename{_refreshFilename},
        outputDirectory{},

        keepAspectRatio{_keepAspectRatio},
        mpc{_mpc},
        quad{_quad},

        cornerRadius{_cornerRadius},
        radius{},
        strokeWidth{_strokeWidth},
        borderOffset{_borderOffset},
        outlineWidth{},
        outlineHeight{},

        imageBorderX{_imageBorderX},
        imageBorderY{_imageBorderY},
        originalImageWidth{100 - (2 * imageBorderX)},
        originalImageHeight{50 - imageBorderY},

        viewportWindowX{},
        viewportWindowY{},

        imageWidth{},
        imageHeight{},
        imageWidthPx{},
        imageHeightPx{},
        imageOffsetXPx{},
        imageOffsetYPx{},
        imagePipScale{},
        imageX{},
        imageY{},

        debug{}, error{}
        {  }
    virtual ~Config(void) {}

    void display(std::ostream &os) const;

    int cardWidthPx;
    int cardHeightPx;
    int cardBorderPx;
    std::string cardColour;

    info indexInfo;
    info cornerPipInfo;
    info standardPipInfo;
    info imagePipInfo;

    std::string indexDirectory;
    std::string pipDirectory;
    std::string faceDirectory;

    std::string scriptFilename;
    std::string refreshFilename;
    std::string outputDirectory;

    bool keepAspectRatio;
    bool mpc;
    bool quad;

    float cornerRadius;
    int radius;
    int strokeWidth;
    int borderOffset;
    int outlineWidth;
    int outlineHeight;

    float imageBorderX;
    float imageBorderY;
    float originalImageWidth;
    float originalImageHeight;

    float viewportWindowX;
    float viewportWindowY;

    float imageWidth;
    float imageHeight;
    int imageWidthPx;
    int imageHeightPx;
    int imageOffsetXPx;
    int imageOffsetYPx;
    float imagePipScale;
    float imageX;
    float imageY;

    std::string name;
    bool debug;
    int error;

    void setName(std::string value) { name = value; }
    void enableDebug(void) { debug = true; }

    int version(void);
    int help(const std::string & error);
    int parseCommandLine(int argc, char *argv[]);
    int initialise(int argc, char *argv[]);

public:
//- Delete the copy constructor and assignement operator.
    Config(const Config &) = delete;
    void operator=(const Config &) = delete;

    int init(int argc, char *argv[]);
    void recalculate(void);
    friend std::ostream & operator<<(std::ostream &os, const Config &A) { A.display(os); return os; }

    static Config & instance() { static Config neo; return neo; }

    static void setImageBorderX(float v) { instance().imageBorderX = v; }
    static void setImageBorderY(float v) { instance().imageBorderY = v; }

    static int getCardWidthPx(void) { return instance().cardWidthPx; }
    static int getCardHeightPx(void) { return instance().cardHeightPx; }
    static int getCardBorderPx(void) { return instance().cardBorderPx; }
    static std::string getCardColour(void) { return instance().cardColour; }

    static info & getIndexInfo(void) { return instance().indexInfo; }
    static info & getCornerPipInfo(void) { return instance().cornerPipInfo; }
    static info & getStandardPipInfo(void) { return instance().standardPipInfo; }
    static float getStandardPipH(void) { return instance().standardPipInfo.getH(); }
    static float getStandardPipX(void) { return instance().standardPipInfo.getX(); }
    static float getStandardPipY(void) { return instance().standardPipInfo.getY(); }
    static info & getImagePipInfo(void) { return instance().imagePipInfo; }
    static float getImagePipH(void) { return instance().imagePipInfo.getH(); }
    static float getImagePipX(void) { return instance().imagePipInfo.getX(); }
    static float getImagePipY(void) { return instance().imagePipInfo.getY(); }

    static std::string getIndexDirectory(void) { return instance().indexDirectory; }
    static std::string getPipDirectory(void) { return instance().pipDirectory; }
    static std::string getFaceDirectory(void) { return instance().faceDirectory; }

    static std::string getScriptFilename(void) { return instance().scriptFilename; }
    static std::string getRefreshFilename(void) { return instance().refreshFilename; }
    static std::string getOutputDirectory(void) { return instance().outputDirectory; }

    static bool isKeepAspectRatio(void) { return instance().keepAspectRatio; }
    static bool isMpc(void) { return instance().mpc; }
    static bool isQuad(void) { return instance().quad; }

    static float getCornerRadius(void) { return instance().cornerRadius; }
    static int getRadius(void) { return instance().radius; }
    static int getStrokeWidth(void) { return instance().strokeWidth; }
    static int getBorderOffset(void) { return instance().borderOffset; }
    static int getOutlineWidth(void) { return instance().outlineWidth; }
    static int getOutlineHeight(void) { return instance().outlineHeight; }

    static float getImageBorderX(void) { return instance().imageBorderX; }
    static float getImageBorderY(void) { return instance().imageBorderY; }
    static float getOriginalImageWidth(void) { return instance().originalImageWidth; }
    static float getOriginalImageHeight(void) { return instance().originalImageHeight; }

    static float getViewportWindowX(void) { return instance().viewportWindowX; }
    static float getViewportWindowY(void) { return instance().viewportWindowY; }

    static float getImageWidth(void) { return instance().imageWidth; }
    static float getImageHeight(void) { return instance().imageHeight; }
    static int getImageWidthPx(void) { return instance().imageWidthPx; }
    static int getImageHeightPx(void) { return instance().imageHeightPx; }
    static int getImageOffsetXPx(void) { return instance().imageOffsetXPx; }
    static int getImageOffsetYPx(void) { return instance().imageOffsetYPx; }
    static float getImagePipScale(void) { return instance().imagePipScale; }
    static float getImageX(void) { return instance().imageX; }
    static float getImageY(void) { return instance().imageY; }

    static bool isName(void) { return !instance().name.empty(); }
    static std::string & getName(void) { return instance().name; }
    static bool isValid(bool showErrors = false);
    static bool isDebug(void) { return instance().debug; }
    static bool isError(void) { return instance().error != 0; }
    static int getError(void) { return instance().error; }

};


#endif //!defined _CONFIGURATION_H__20230519_1343__INCLUDED_

