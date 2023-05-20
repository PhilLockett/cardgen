/**
 * @file    Configuration.cpp
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
 * Implementation of the Command line parameter configuration Singleton.
 */

#include <future>
#include <algorithm>
#include <fstream>

#include "cardgen.h"

#include "Opts.h"
#include "Configuration.h"
#include "Loc.h"


/**
 * @section duit configuration Singleton implementation.
 *
 * Command line parameter initialisation.
 */

/**
 * Display version message.
 *
 * @param  name - of application.
 */
int Config::version(void)
{
    std::cout << "Version " VERSION " of " << name << "\n";
    std::cout << "Feedback to: " PACKAGE_BUGREPORT "\n";

    return 2;
}


const Opts::OptsType optList
{
    { 'H', "help",              NULL,       "This help page and nothing else." },
    { 'v', "version",           NULL,       "Display version." },
    { 0,  NULL, NULL, "" },
    { 'i', "index",             "directory","Subdirectory of indices to use (default: \"" + _indexDirectory + "\")." },
    { 'p', "pip",               "directory","Subdirectory of pips to use (default: \"" + _pipDirectory + "\")." },
    { 'f', "face",              "directory","Subdirectory of faces to use (default: \"" + _faceDirectory + "\")." },
    { 0,  NULL, NULL, "" },
    { 's', "script",            "filename", "Script filename (default: \"" + _scriptFilename + "\")." },
    { 'o', "output",            "directory","Output directory (default: same directory name as face)." },
    { 'w', "width",             "integer",  "Card width in pixels (default: " + std::to_string(_cardWidthPx) + ")." },
    { 'h', "height",            "integer",  "Card height in pixels (default: " + std::to_string(_cardHeightPx) + ")." },
    { 'c', "colour",            "name",     "Background colour name (defined at: http://www.imagemagick.org/script/color.php, default: \"" + _cardColour + "\")." },
    { 'a', "KeepAspectRatio",   NULL,       "Keep image Aspect Ratio (default: " + std::string(_keepAspectRatio ? "true" : "false") + ")." },
    { 'm', "MPC",               NULL,       "Generate MakePlayingCards image (default: " + std::string(_mpc ? "true" : "false") + ", see: https://www.makeplayingcards.com/)." },
    { 'q', "Quad",              NULL,       "Generate pips and indices in all 4 corners (default: " + std::string(_quad ? "true" : "false") + ")." },
    { 0,  NULL, NULL, "" },
    { 1,   "IndexHeight",       "value",    "Height of index as a % of card height (default: " + _indexInfo.getHStr() + ")." },
    { 2,   "IndexCentreX",      "value",    "X value of centre of index as a % of card width (default: " + _indexInfo.getXStr() + ")." },
    { 3,   "IndexCentreY",      "value",    "Y value of centre of index as a % of card height (default: " + _indexInfo.getYStr() + ")." },
    { 4,   "CornerPipHeight",   "value",    "Height of corner pip as a % of card height (default: " + _cornerPipInfo.getHStr() + ")." },
    { 5,   "CornerPipCentreX",  "value",    "X value of centre of corner pip as a % of card width (default: " + _cornerPipInfo.getXStr() + ")." },
    { 6,   "CornerPipCentreY",  "value",    "Y value of centre of corner pip as a % of card height (default: " + _cornerPipInfo.getYStr() + ")." },
    { 7,   "StandardPipHeight", "value",    "Height of standard pip as a % of card height (default: " + _standardPipInfo.getHStr() + ")." },
    { 8,   "StandardPipCentreX","value",    "X value of centre of standard pip as a % of card width (default: " + _standardPipInfo.getXStr() + ")." },
    { 9,   "StandardPipCentreY","value",    "Y value of centre of standard pip as a % of card height (default: " + _standardPipInfo.getYStr() + ")." },
    { 10,  "ImageBorderX",      "value",    "Image Border in X direction as a % of card width (default: " + std::to_string(_imageBorderX) + ")." },
    { 11,  "ImageBorderY",      "value",    "Image Border in Y direction as a % of card height (default: " + std::to_string(_imageBorderY) + ")." },
    { 12,  "ImagePipOff",       NULL,       "Don't display image pip on the court cards." },
    { 13,  "ImagePipHeight",    "value",    "Height of image pip as a % of card height (default: " + _imagePipInfo.getHStr() + ")." },
    { 14,  "ImagePipCentreX",   "value",    "X value of centre of image pip as a % of card width relative to ImageBorderX (default: " + _imagePipInfo.getXStr() + ")." },
    { 15,  "ImagePipCentreY",   "value",    "Y value of centre of image pip as a % of card height relative to ImageBorderY (default: " + _imagePipInfo.getYStr() + ")." },
    { 0,  NULL, NULL, "" },
    { 'C', "CentreX",           "value",    "Shortcut for: --IndexCentreX value --CornerPipCentreX value." },
    { 'I', "Inputs",            "value",    "Shortcut for: -f value -p value -i value." },
    { 'x', NULL, NULL, "" },

};
Opts optSet{optList, "    "};

/**
 * Display help message.
 *
 * @param  name - of application.
 */
int Config::help(const std::string & error)
{
    std::cout << "Usage: " << name << " [Options]\n";
    std::cout << "\n";
    std::cout << "  Generates the bash script \"" << scriptFilename << "\" which uses 'convert' to draw a deck of playing cards."<< endl;
    std::cout << "\n";
    std::cout << "  Options:\n";
    std::cout << optSet;

    if (error.empty())
        return 1;

    std::cerr << "\nError: " << error << "\n";

    if (optSet.isErrors())
    {
        std::cerr << "\n";
        optSet.streamErrors(std::cerr);
    }

    return -1;
}


/**
 * Process command line parameters.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return value if precessing should be aborted or 0 otherwise.
 */
int Config::parseCommandLine(int argc, char *argv[])
{
    if (argc < 2)
        return help("valid arguments required.");

    optSet.process(argc, argv);

    if (optSet.isErrors())
        return help("valid arguments required.");

    for (const auto & option : optSet)
    {
        switch (option.getOpt())
        {
        case 'H': return help("");
        case 'v': return version();

        case 'w': cardWidthPx = option.getArgInt();             break;
        case 'h': cardHeightPx = option.getArgInt();            break;
        case 'c': cardColour = option.getArg();                 break;

        case 'i': indexDirectory = option.getArg();             break;
        case 'p': pipDirectory = option.getArg();               break;
        case 'f': faceDirectory = option.getArg();              break;

        case 's': scriptFilename = option.getArg();             break;
        case 'o': outputDirectory = option.getArg();            break;

        case 'a': keepAspectRatio = true;                       break;
        case 'm': mpc = true;                                   break;
        case 'q': quad = true;                                  break;

        case 1:   indexInfo.setH(option.getArgFloat());         break;
        case 2:   indexInfo.setX(option.getArgFloat());         break;
        case 3:   indexInfo.setY(option.getArgFloat());         break;

        case 4:   cornerPipInfo.setH(option.getArgFloat());     break;
        case 5:   cornerPipInfo.setX(option.getArgFloat());     break;
        case 6:   cornerPipInfo.setY(option.getArgFloat());     break;

        case 7:   standardPipInfo.setH(option.getArgFloat());   break;
        case 8:   standardPipInfo.setX(option.getArgFloat());   break;
        case 9:   standardPipInfo.setY(option.getArgFloat());   break;

        case 10:  imageBorderX = option.getArgFloat();          break;
        case 11:  imageBorderY = option.getArgFloat();          break;
        case 12:  imagePipInfo.setH(0);                         break;
        case 13:  imagePipInfo.setH(option.getArgFloat());      break;
        case 14:  imagePipInfo.setX(option.getArgFloat());      break;
        case 15:  imagePipInfo.setY(option.getArgFloat());      break;

        case 'C':
            indexInfo.setX(option.getArgFloat());
            cornerPipInfo.setX(option.getArgFloat());
            break;

        case 'I':
            indexDirectory = option.getArg();
            pipDirectory   = option.getArg();
            faceDirectory  = option.getArg();
            break;

        case 'x': enableDebug(); break;

        default: return help("internal error.");
        }
    }

    if (isError())
        return -2;

    return 0;
}


/**
 * Initialise using command line input.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int Config::initialise(int argc, char *argv[])
{
    setName(argv[0]);   // Store program name;

    return parseCommandLine(argc, argv);
}


/**
 * Initialise using command line input and ensure we only do it once.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int Config::init(int argc, char *argv[])
{
    static std::once_flag virgin{};
    int ret{};

    std::call_once(virgin, [&](){ ret = initialise(argc, argv); });
    if (!ret)
        recalculate();

    if (debug)
        display(std::cout);

    return ret;
}

/**
 * Recalculate global variables using command line input.
 */
void Config::recalculate(void)
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

    PatternCollection::calibrate(viewportWindowX, viewportWindowY);
}


/**
 * @section duit configuration Singleton implementation.
 *
 * Support code.
 */
void Config::display(std::ostream &os) const
{
    os << "Config is " << (isValid() ? "" : "NOT ") << "valid\n";

	os << "cardWidthPx\t" << cardWidthPx << endl;
	os << "cardHeightPx\t" << cardHeightPx << endl;
	os << "cardBorderPx\t" << cardBorderPx << endl;
	os << "cardColour\t" << cardColour << endl;
	os << endl;
	os << "indexInfo\t" << indexInfo << endl;
	os << "cornerPipInfo\t" << cornerPipInfo << endl;
	os << "standardPipInfo\t" << standardPipInfo << endl;
	os << "imagePipInfo\t" << imagePipInfo << endl;
	os << endl;
	os << "indexDirectory\t" << indexDirectory << endl;
	os << "pipDirectory\t" << pipDirectory << endl;
	os << "faceDirectory\t" << faceDirectory << endl;
	os << endl;
	os << "scriptFilename\t" << scriptFilename << endl;
	os << "refreshFilename\t" << refreshFilename << endl;
	os << "outputDirectory\t" << outputDirectory << endl;
	os << endl;
	os << "keepAspectRatio\t" << keepAspectRatio << endl;
	os << "MPC\t\t" << mpc << endl;
	os << "Quad\t\t" << quad << endl;
	os << endl;
	os << "cornerRadius\t" << cornerRadius << endl;
	os << "radius\t\t" << radius << endl;
	os << "strokeWidth\t" << strokeWidth << endl;
	os << "borderOffset\t" << borderOffset << endl;
	os << "outlineWidth\t" << outlineWidth << endl;
	os << "outlineHeight\t" << outlineHeight << endl;
	os << endl;
	os << "imageBorderX\t" << imageBorderX << endl;
	os << "imageBorderY\t" << imageBorderY << endl;
	os << "originalImageWidth\t" << originalImageWidth << endl;
	os << "originalImageHeight\t" << originalImageHeight << endl;
	os << endl;
	os << "viewportWindowX\t" << viewportWindowX << endl;
	os << "viewportWindowY\t" << viewportWindowY << endl;
	os << endl;
	os << "imageWidth\t" << imageWidth << endl;
	os << "imageHeight\t" << imageHeight << endl;
	os << "imageWidthPx\t" << imageWidthPx << endl;
	os << "imageHeightPx\t" << imageHeightPx << endl;
	os << "imageOffsetXPx\t" << imageOffsetXPx << endl;
	os << "imageOffsetYPx\t" << imageOffsetYPx << endl;
	os << endl;
	os << "imagePipScale\t" << imagePipScale << endl;
	os << "imageX\t\t" << imageX << endl;
	os << "imageY\t\t" << imageY << endl;
}


/**
 * @brief Validate the configuration.
 * 
 * @param showErrors flag to indicate if errors should be displayed.
 * @return true if configuration is valid, false otherwise.
 */
bool Config::isValid(bool showErrors)
{

    return true;
}



