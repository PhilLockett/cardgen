/**
 * @file    dump.cpp
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
 * Card generation script generator.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "cardgen.h"
#include "desc.h"
#include "Loc.h"
#include "Configuration.h"


/**
 * @section Internal constants and variables.
 *
 */

static const vector<const char*> suits{ "C", "D", "H", "S" };
static const vector<const char*> alts{ "S", "H", "D", "C" };
static const vector<const char*> cards{ "0", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

static const vector<const char*> suitNames{ "Clubs", "Diamonds", "Hearts", "Spades" };
static const vector<const char*> cardNames{ "Joker", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };


/**
 * @section main code.
 *
 */

/**
 * Generate the initial blank card string used as a template for each card.
 *
 * @return the generated string.
 */
static string genStartString(void)
{
    static string outputString{};
    if (outputString.empty())
    {
        stringstream stream{};
        const auto cardWidthPx{Config::getCardWidthPx()};
        const auto cardHeightPx{Config::getCardHeightPx()};
        const auto cardBorderPx{Config::getCardBorderPx()};
        const auto cardColour{Config::getCardColour()};
        const auto mpc{Config::isMpc()};
        const auto radius{Config::getRadius()};
        const auto borderOffset{Config::getBorderOffset()};
        const auto outlineWidth{Config::getOutlineWidth()};
        const auto outlineHeight{Config::getOutlineHeight()};
        const auto strokeWidth{Config::getStrokeWidth()};

        stream  << "convert -size " << cardWidthPx + (2 * cardBorderPx) << "x" << cardHeightPx + (2 * cardBorderPx) << " xc:transparent  \\" << endl;
        stream  << "\t-fill '" << cardColour << "' ";
        if (mpc)
            stream  << "-draw 'rectangle " << borderOffset << ',' << borderOffset << ' ' << outlineWidth + (2 * cardBorderPx) << ',' << outlineHeight + (2 * cardBorderPx) << "'";
        else
        if (radius)
            stream  << "-stroke black -strokewidth " << strokeWidth << " -draw 'roundRectangle " << borderOffset << ',' << borderOffset << ' ' << outlineWidth << ',' << outlineHeight << ' ' << radius << ',' << radius << "'";
        else
            stream  << "-stroke black -strokewidth " << strokeWidth << " -draw 'rectangle " << borderOffset << ',' << borderOffset << ' ' << outlineWidth << ',' << outlineHeight << "'";
        stream  << " \\" << endl;

        outputString = stream.str();
    }

    return outputString;
}

/**
 * Generate the final blank card string to end each card.
 *
 * @return the generated string.
 */
static void genEndString(ofstream & file, const std::string & fileName)
{
    file << "\t+dither -colors 256 \\" << endl;
    file << "\tcards/" << Config::getOutputDirectory() << "/" << fileName << ".png" << endl;
    file << endl;
}

/**
 * Generate the string for drawing the pips on the card. This is a two pass
 * process. The second pass is after the card image has been rotated.
 *
 * @param  rotate - Rotated on first pass.
 * @param  card - 1 to 13 (ace to king).
 * @param  pipD - standard pip descriptor.
 * @return the generated string.
 */
static string drawStandardPips(bool rotate, size_t card, desc & pipD)
{
    if (!PatternCollection::isIndex(card))
        return "";

    const float x{Config::getStandardPipX()};
    const float y{Config::getStandardPipY()};
    Pattern pattern{PatternCollection::getPattern(card)};
    pattern.setRotate(rotate);

    stringstream outputString{};
    for (auto [xOffset, yOffset] : pattern)
    {
        pipD.reposition(x + xOffset, y + yOffset);
        outputString  << pipD.draw();
    }

    return outputString.str();
}


/**
 * Generate the string for drawing the image on the card. Usually used for the
 * court cards. Numerous internal variables need to be recalculated if the
 * aspect ratio of the image is to be maintained, otherwise the image is
 * stretched to fill the card. Note that this is done for each image because
 * the dimensions of the image can vary.
 *
 * @param  faceD - Image descriptor.
 * @param  fileName - name of image file to use.
 * @return the generated string.
 */
static string drawImage(const desc & faceD, const string & fileName)
{
    const auto cardWidthPx{Config::getCardWidthPx()};
    const auto cardHeightPx{Config::getCardHeightPx()};
    const auto cardBorderPx{Config::getCardBorderPx()};
    const auto keepAspectRatio{Config::isKeepAspectRatio()};
    const auto imageWidthPx{Config::getImageWidthPx()};
    const auto imageHeightPx{Config::getImageHeightPx()};

    stringstream outputString{};
    int x{Config::getImageOffsetXPx() + cardBorderPx};
    int y{Config::getImageOffsetYPx() + cardBorderPx};
    int w{imageWidthPx};
    int h{imageHeightPx};
    float scale{1};
    float aspectRatio{};

//- Adjust values based on variables that may have been changed by the user.
    if (faceD.isLandscape())
    {
        if (keepAspectRatio)
        {
            aspectRatio = float(imageWidthPx) / imageHeightPx;
            if (faceD.getAspectRatio() < aspectRatio)
            {
                // Use imageHeightPx to redefine view port size.
                scale = (float)imageHeightPx / faceD.getImageHeightPx();
                w = ROUND(scale * faceD.getImageWidthPx()) + 1;
                x = ((cardWidthPx - w)/2) + cardBorderPx;
            }
            else
            {
                // Use imageWidthPx to redefine view port size.
                scale = (float)imageWidthPx / faceD.getImageWidthPx();
                h = ROUND(scale * faceD.getImageHeightPx());
                y = (cardHeightPx/2) - h + cardBorderPx;
            }
        }
    }
    else
    {
        h = 2*imageHeightPx;
        if (keepAspectRatio)
        {
            aspectRatio = (float)imageWidthPx / (2*imageHeightPx);
            if (faceD.getAspectRatio() < aspectRatio)
            {
                // Use 2*imageHeightPx to redefine view port size.
                scale = (float)imageHeightPx / faceD.getImageHeightPx();
                w = ROUND(scale * 2 * faceD.getImageWidthPx());
                x = ((cardWidthPx - w)/2) + cardBorderPx;
            }
            else
            {
                // Use imageWidthPx to redefine view port size.
                scale = (float)imageWidthPx / faceD.getImageWidthPx();
                h = ROUND(scale * faceD.getImageHeightPx());
                y = ((cardHeightPx - h)/2) + cardBorderPx;
            }
        }
    }

    outputString << "\t-draw \"image over " << x << ',' << y << ' ' << w << ',' << h << " '" << faceD.getFileName() << "'\" \\" << endl;

//- Check if image pips are required.
    if (Config::getImagePipH())
    {
        const auto imagePipScale{Config::getImagePipScale()};
        const info & imagePipInfo{Config::getImagePipInfo()};
        info scaledPip{imagePipInfo};

        // Rescale image pips, but only if they haven't been manually altered.
        if (!imagePipInfo.isChangedH())
            scaledPip.setH(imagePipScale * imagePipInfo.getH());

        if (!imagePipInfo.isChangedX())
            scaledPip.setX(imagePipScale * imagePipInfo.getX());

        if (!imagePipInfo.isChangedY())
            scaledPip.setY(imagePipScale * imagePipInfo.getY());

        // Pip Filename is only supplied for court cards if they need pips adding.
        desc pipD{scaledPip, fileName};
        if (pipD.isFileFound())
        {
            const int x2{pipD.getPortOriginXPx()+x};
            const int y2{pipD.getPortOriginYPx()+y};
            const int w2{ROUND(pipD.getPortWidthPx())};
            const int h2{ROUND(pipD.getPortHeightPx())};

            outputString << "\t-draw \"image over " << x2 << ',' << y2 << ' ' << w2 << ',' << h2 << " '" << fileName << "'\" \\" << endl;
            outputString << "\t-rotate 180 \\" << endl;
            outputString << "\t-draw \"image over " << x2 << ',' << y2 << ' ' << w2 << ',' << h2 << " '" << fileName << "'\" \\" << endl;
            outputString << "\t-rotate 180 \\" << endl;
        }
    }

    return outputString.str();
}


/**
 * ImageMagick Joker drawing routine.
 *
 * @param  file - output file stream.
 * @param  fileName - name of joker image file being generated.
 */
static void drawImageMagickJoker(ofstream & file, const string & fileName)
{
    const string faceFile{"boneyard/ImageMagick_logo.svg.png"};
    const desc faceD{95, 50, 50, faceFile};

    const string headerFile{"boneyard/ImageMagickUsage.png"};
    const desc headerD{4, 50, 10, headerFile};
    const string footerFile{"boneyard/ImageMagickURL.png"};
    const desc footerD{3, 50, 90, footerFile};

    file << genStartString();
    file << drawImage(faceD, "");
    file << headerD.draw();
    file << footerD.draw();
    genEndString(file, fileName);
}


/**
 * Default Joker drawing routine.
 *
 * @param  file - output file stream.
 * @param  fileName - name of joker image file being generated.
 * @param  indexD - joker index descriptor.
 */
static void drawDefaultJoker(ofstream & file, const string & fileName, const desc & indexD)
{
    const string faceFile{"boneyard/Back.png"};
    const desc faceD(95, 50, 50, faceFile);

    file << genStartString();

    // Draw "Joker" indices if provided.
    if (indexD.isFileFound())
    {
        file << indexD.draw();
        file << "\t-rotate 180 \\" << endl;
        file << indexD.draw();
    }

    file << drawImage(faceD, "");
    genEndString(file, fileName);
}


/**
 * Joker drawing routine - a bit messy, but gets the job done.
 *
 * @param  fails - default joker image output count.
 * @param  file - output file stream.
 * @param  suit - index of suit for the joker being generated.
 * @return 0 if joker image found and used, 1 if default joker created.
 */
static int drawJoker(int fails, ofstream & file, int suit)
{
    file << "# Draw the " << suitNames[suit] << " " << cardNames[0] << " as file " << suits[suit] << cardNames[0] << ".png" << endl;

    const string fileName{string(suits[suit]) + cardNames[0]};
    const string faceFile{"faces/" + Config::getFaceDirectory() + "/" + fileName + ".png"};
    const desc faceD{95, 50, 50, faceFile};

    const string indexFile{"indices/" + Config::getIndexDirectory() + "/" + fileName + ".png"};
    const desc indexD{Config::getIndexInfo(), indexFile};

    if ((indexD.isFileFound()) || (faceD.isFileFound()))
    {
        file << genStartString();

        if (indexD.isFileFound())
        {
            file << indexD.draw();
            file << "\t-rotate 180 \\" << endl;
            file << indexD.draw();
        }

        if (faceD.isFileFound())
            file << drawImage(faceD, "");

        genEndString(file, fileName);

        return 0;
    }

    // Got here so, Joker image file not found, so generate card.
    switch (fails)
    {
    case 0:
    case 2:
        drawImageMagickJoker(file, fileName);
        break;

    default:
        drawDefaultJoker(file, fileName, indexD);
        break;
    }

    return 1;
}


/**
 * The bulk of the script generation work.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int generateScript(int argc, char *argv[])
{
    const auto scriptFilename{Config::getScriptFilename()};
    const auto refreshFilename{Config::getRefreshFilename()};
    const auto indexDirectory{Config::getIndexDirectory()};
    const auto pipDirectory{Config::getPipDirectory()};
    const auto faceDirectory{Config::getFaceDirectory()};
    const auto outputDirectory{Config::getOutputDirectory()};
    ofstream file{scriptFilename.c_str()};

//- Open the script file for writing.
    if (!file)
    {
        cerr << "Can't open output file " << scriptFilename << " - aborting!" << endl;

        return 1;
    }

//- Generate the initial preamble of the script.
    file << "#!/bin/sh" << endl;
    file << endl;
    file << "# This file was generated as \"" << scriptFilename << "\" using the following command:" << endl;
    file << "#" << endl;
    file << "#  ";
    for (int i = 0; i < argc; ++i)
        file << argv[i] << ' ';

    file << endl;
    file << "#" << endl;
    file << endl;
    file << "# Make the directories."  << endl;
    file << "mkdir -p cards"  << endl;
    file << "mkdir -p cards/" << outputDirectory << endl;

    file << endl;
    file << "# Generate the refresh script."  << endl;
    file << "cat <<EOM >cards/" << outputDirectory  << "/" << refreshFilename << endl;
    file << "#!/bin/sh" << endl;
    file << endl;
    file << "# This file was generated using the following " << argv[0] << " command." << endl;
    file << "#" << endl;
    file << "cd ../../" << endl;
    for (int i = 0; i < argc; ++i)
        file << argv[i] << ' ';

    file << endl;
    file << "./" << scriptFilename << endl;
    file << "EOM" << endl;
    file << endl;
    file << "chmod +x cards/" << outputDirectory	<< "/" << refreshFilename << endl;
    file << endl;


//- Initial blank card string used as a template for each card.
    const string startString{genStartString()};
    const auto indexInfo{Config::getIndexInfo()};
    const auto cornerPipInfo{Config::getCornerPipInfo()};
    const auto standardPipInfo{Config::getStandardPipInfo()};
    const auto imageHeight{Config::getImageHeight()};
    const auto imageX{Config::getImageX()};
    const auto imageY{Config::getImageY()};
    const auto quad{Config::isQuad()};

//- Generate all the playing cards.
    for (size_t s = 0; s < suits.size(); ++s)
    {
        const string suit{string(suits[s])};

        string pipFile{string("pips/") + pipDirectory + "/" + suit + "S.png"};     // Try small pip file first.
        desc pipD{cornerPipInfo, pipFile};
        if (!pipD.isFileFound())
        {
            // Small pip file not found, so use standard pip file.
            pipFile = string("pips/") + pipDirectory + "/" + suit + ".png";
            pipD.setFileName(pipFile);
        }

        // Generate the playing cards in the current suit.
        pipFile = string("pips/") + pipDirectory + "/" + suit + ".png";             // Use standard pip file.
        desc standardPipD{standardPipInfo, pipFile};
        for (size_t c = 1; c < cards.size(); ++c)
        {
            // Set up the variables.
            string card{string(cards[c])};
            string fileName{suit + card};

            string indexFile{string("indices/") + indexDirectory + "/" + fileName + ".png"};
            desc indexD{indexInfo, indexFile};
            if (!indexD.isFileFound())
            {
                // indexInfo for suit file not found, so use alternate index file.
                indexFile = string("indices/") + indexDirectory + "/" + string(alts[s]) + card + ".png";
                indexD.setFileName(indexFile);
            }

            string faceFile{string("faces/") + faceDirectory + "/" + fileName + ".png"};
            desc faceD{imageHeight, imageX, imageY, faceFile};

            string drawFace{};

            if (faceD.useStandardPips())
            {
                // The face directory does not have the needed image file, use standard pips.
                drawFace = drawStandardPips(true, c, standardPipD);
            }
            else
            {
                // The face directory does have the needed image file, so use it.
                // Note, we only pass the pipfile name for the court cards (c > 10).
                drawFace = drawImage(faceD, c > 10 ? pipFile : "");
            }


            // Write to output file.
            file << "# Draw the " << cardNames[c] << " of " << suitNames[s] << " as file " << fileName << ".png." << endl;
            file << startString;

            if ((faceD.useStandardPips()) || (faceD.isFileFound() && faceD.isLandscape()))
                file << drawFace;			// Draw either half of the pips or one of the landscape images.

            file << pipD.draw();            // Draw corner pip.
            file << indexD.draw();          // Draw index.
            if (quad)
            {
                file << pipD.draw(false);   // Draw right-hand corner pip.
                file << indexD.draw(false); // Draw right-hand index.
            }

            file << "\t-rotate 180 \\" << endl;

            if (faceD.useStandardPips())
                drawFace = drawStandardPips(false, c, standardPipD);

            file << drawFace;				// Draw either the rest of the pips or the needed image.
            file << pipD.draw();            // Draw corner pip.
            file << indexD.draw();          // Draw index.
            if (quad)
            {
                file << pipD.draw(false);   // Draw right-hand corner pip.
                file << indexD.draw(false); // Draw right-hand index.
            }

            genEndString(file, fileName);
        }
    }


//- Add the Jokers using narrower borders.
    Config::setImageBorderX(7);
    Config::setImageBorderY(5);
    Config::getIndexInfo().setH(30.0);
    Config::getIndexInfo().setY(20.0);
    Config::instance().recalculate();

    int fails{};
    for (int s = 0; s < suits.size(); ++s)
        fails += drawJoker(fails, file, s);

    file << "echo Output created in cards/" << outputDirectory << "/" << endl;
    file << endl;

    return 0;
}
