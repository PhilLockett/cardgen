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
#include "cardgen.h"
#include "desc.h"


/**
 * @section Internal constants and variables.
 *
 */

static const char* suits[]  = { "C", "D", "H", "S" };
static const char* alts[]   = { "S", "H", "D", "C" };
static const char* cards[]  = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

static const char* suitNames[]  = { "Clubs", "Diamonds", "Hearts", "Spades" };
static const char* cardNames[]  = { "Joker", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };


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
    stringstream outputStream;
    outputStream  << "convert -size " << cardWidth << "x" << cardHeight << " xc:transparent  \\" << endl;
    outputStream  << "\t-fill '" << cardColour << "' -stroke black -strokewidth " << strokeWidth << " -draw 'roundRectangle " << borderOffset << ',' << borderOffset << ' ' << outlineWidth << ',' << outlineHeight << ' ' << radius << ',' << radius << "' \\" << endl;

    return outputStream.str();
}


/**
 * Constants for drawStandardPips().
 *
 */
static const float offsets[] = { (1.0F / 2), (0.0F), (1.0F), (1.0F / 4), (1.0F / 3), (1.0F / 6) };

static const struct
{
    int     xIndex;
    int     yIndex;
    bool    rotate;

}
    loc[] =
{
    { 0, 0, false },
    { 0, 1, true },
    { 0, 1, false },
    { 1, 1, true },
    { 2, 1, true },
    { 1, 1, false },
    { 2, 1, false },
    { 1, 0, false },
    { 2, 0, false },
    { 0, 3, false },
    { 0, 3, true },
    { 1, 4, true },
    { 2, 4, true },
    { 1, 4, false },
    { 2, 4, false },
    { 0, 5, true },
    { 0, 5, false }

};

static int getXIndex(int locIndex) { return loc[locIndex].xIndex; }
static int getYIndex(int locIndex) { return loc[locIndex].yIndex; }
static bool getRotate(int locIndex) { return loc[locIndex].rotate; }

static float getXOffset(int locIndex) { return offsets[getXIndex(locIndex)]; }
static float getYOffset(int locIndex) { return offsets[getYIndex(locIndex)]; }

static const int corners[] = { 3, 5 };
static const int ace[]     = { 0 };
static const int c2[]      = { 1, 2 };
static const int c3[]      = { 1, 0, 2 };
static const int c4[]      = { 3, 4, 5, 6 };
static const int c5[]      = { 3, 4, 0, 5, 6 };
static const int c6[]      = { 3, 4, 5, 6, 7, 8 };
static const int c7[]      = { 3, 4, 5, 6, 7, 8, 9 };
static const int c8[]      = { 3, 4, 10, 5, 6, 7, 8, 9 };
static const int c9[]      = { 3, 4, 11, 12, 0, 5, 6, 13, 14 };
static const int c10[]     = { 3, 4, 11, 12, 15, 5, 6, 13, 14, 16 };
static const int jack[]    = { 3, 4, 10, 11, 12, 0, 5, 6, 9, 13, 14 };
static const int queen[]   = { 1, 3, 4, 10, 11, 12, 2, 5, 6, 9, 13, 14 };
static const int king[]    = { 1, 3, 4, 10, 11, 12, 0, 2, 5, 6, 9, 13, 14 };

#define AD(a) { ELEMENTS(a), a }

const struct
{
    int length;
    const int * const locations;
}
patterns[] = { AD(corners), AD(ace), AD(c2), AD(c3), AD(c4), AD(c5), AD(c6), AD(c7), AD(c8), AD(c9), AD(c10), AD(jack), AD(queen), AD(king) };


/**
 * Generate the string for drawing the pips on the card. This is a two pass
 * process. The second pass is after the card image has been rotated.
 *
 * @param  rotate - Rotated on first pass.
 * @param  card - 1 to 13 (ace to king).
 * @param  fileName - name of image file for the pip.
 * @return the generated string.
 */
static string drawStandardPips(bool rotate, int card, desc & pipD)
{
    stringstream outputStream;

    for (int i = 0; i < patterns[card].length; ++i)
    {
        const int index = patterns[card].locations[i];
        if (getRotate(index) == rotate)
        {
            const float offX = standardPipInfo.getX() + (getXOffset(index) * winPX);
            const float offY = standardPipInfo.getY() + (getYOffset(index) * winPY);

            pipD.repos(offX, offY);
            outputStream  << pipD.draw();
        }
    }

    return outputStream.str();
}


/**
 * Generate the string for drawing the image on the card. Usually used for the
 * court cards. Numerous internal variables need to be recalculated if the
 * aspect ratio of the image is to be maintained, otherwise the image is
 * stretched to fill the card. Note that this is done for each image because
 * the dimensions can vary.
 *
 * @param  faceD - Image descriptor.
 * @param  fileName - name of image file for the pip.
 * @return the generated string.
 */
static string drawImage(const desc & faceD, const string & fileName)
{
    stringstream outputStream;
    int x = offsetX;
    int y = offsetY;
    int w = widthPX;
    int h = heightPX;
    float scale = 1;
    float aspectRatio = 0.0;

    if (faceD.isLandscape())
    {
        if (keepAspectRatio)
        {
            aspectRatio = float(widthPX) / heightPX;
            if (faceD.getAspectRatio() < aspectRatio)
            {
                // Use heightPX to redefine view port size.
                scale = (float)heightPX / faceD.getHeightPX();
                w = ROUND(scale * faceD.getWidthPX()) + 1;
                x = (cardWidth - w)/2;
            }
            else
            {
                // Use widthPX to redefine view port size.
                scale = (float)widthPX / faceD.getWidthPX();
                h = ROUND(scale * faceD.getHeightPX());
                y = (cardHeight/2) - h;
            }
        }
    }
    else
    {
        h = 2*heightPX;
        if (keepAspectRatio)
        {
            aspectRatio = (float)widthPX / (2*heightPX);
            if (faceD.getAspectRatio() < aspectRatio)
            {
                // Use 2*heightPX to redefine view port size.
                scale = (float)(heightPX * 2) / faceD.getHeightPX();
                w = ROUND(scale * faceD.getWidthPX());
                x = (cardWidth - w)/2;
            }
            else
            {
                // Use widthPX to redefine view port size.
                scale = (float)widthPX / faceD.getWidthPX();
                h = ROUND(scale * faceD.getHeightPX());
                y = (cardHeight - h)/2;
            }
        }
    }

    outputStream << "\t-draw \"image over " << x << ',' << y << ' ' << w << ',' << h << " '" << faceD.getFileName() << "'\" \\" << endl;

//- Check if image pips are required.
    if (imagePipInfo.getH())
    {
        info scaledPip(imagePipInfo);

//- Rescale image pips, but only if they haven't been manually altered.
        if (!imagePipInfo.isChangedH())
        {
            scale = (float)h / originalHeightPX;
            if (!faceD.isLandscape())
            {
                scale /= 2;
            }
            scaledPip.setH(scale * imagePipInfo.getH());
        }

        if (!imagePipInfo.isChangedX())
        {
            scale = (float)w / originalWidthPX;
            scaledPip.setX(scale * imagePipInfo.getX());
        }

        if (!imagePipInfo.isChangedY())
        {
            scale = (float)h / originalHeightPX;
            if (!faceD.isLandscape())
            {
                scale /= 2;
            }
            scaledPip.setY(scale * imagePipInfo.getY());
        }

//- Pip Filename is only supplied for court cards if they need pips adding.
        desc pipD(scaledPip, fileName);
        if (pipD.isFileFound())
        {
            outputStream << "\t-draw \"image over " << pipD.getOriginX()+x << ',' << pipD.getOriginY()+y << ' ' << ROUND(pipD.getWidth()) << ',' << ROUND(pipD.getHeight()) << " '" << fileName << "'\" \\" << endl;
            outputStream << "\t-rotate 180 \\" << endl;
            outputStream << "\t-draw \"image over " << pipD.getOriginX()+x << ',' << pipD.getOriginY()+y << ' ' << ROUND(pipD.getWidth()) << ',' << ROUND(pipD.getHeight()) << " '" << fileName << "'\" \\" << endl;
            outputStream << "\t-rotate 180 \\" << endl;
        }
    }

    return outputStream.str();
}


/**
 * ImageMagick Joker drawing routine.
 *
 * @param  file - output file stream.
 * @param  fileName - name of joker image file being generated.
 */
static void drawImageMagickJoker(ofstream & file, const string & fileName)
{
    string startString = genStartString();

    string faceFile = string("boneyard/ImageMagick_logo.svg.png");
    desc faceD(95, 50, 50, faceFile);

    string headerFile = string("boneyard/ImageMagickUsage.png");
    desc headerD(4, 50, 10, headerFile);
    string footerFile = string("boneyard/ImageMagickURL.png");
    desc footerD(3, 50, 90, footerFile);

    file << startString;
    file << drawImage(faceD, "");
    file << headerD.draw();
    file << footerD.draw();
    file << "\t+dither -colors 256 \\" << endl;
    file << "\tcards/" << outputDirectory << "/" << fileName << ".png" << endl;
    file << endl;
}


/**
 * Default Joker drawing routine.
 *
 * @param  file - output file stream.
 * @param  fileName - name of joker image file being generated.
 * @param  suit - index of suit for the joker being generated.
 */
static void drawDefaultJoker(ofstream & file, const string & fileName, int suit)
{
    string startString = genStartString();

    string faceFile = string("boneyard/Back.png");
    desc faceD(95, 50, 50, faceFile);

    startString = genStartString();
    file << startString;

    // Draw "Joker" indices if provided.
    string indexFile = string("indices/") + indexDirectory + "/" + string(suits[suit]) + "Joker.png";
    desc indexD(indexInfo, indexFile);
    if (indexD.isFileFound())
    {
        file << indexD.draw();
        file << "\t-rotate 180 \\" << endl;
        file << indexD.draw();
    }

    file << drawImage(faceD, "");
    file << "\t+dither -colors 256 \\" << endl;
    file << "\tcards/" << outputDirectory << "/" << fileName << ".png" << endl;
    file << endl;
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

    string fileName = string(suits[suit]) + cardNames[0];
    string faceFile = string("faces/") + faceDirectory + "/" + fileName + ".png";
    desc faceD(95, 50, 50, faceFile);

    string indexFile = string("indices/") + indexDirectory + "/" + fileName + ".png";
    desc indexD(indexInfo, indexFile);

    if ((indexD.isFileFound()) || (faceD.isFileFound()))
    {
        string startString = genStartString();
        file << startString;

        if (indexD.isFileFound())
        {
            file << indexD.draw();          // Draw index.
            file << "\t-rotate 180 \\" << endl;
            file << indexD.draw();          // Draw index.
        }

        if (faceD.isFileFound())
        {
            file << drawImage(faceD, "");
        }

        file << "\t+dither -colors 256 \\" << endl;
        file << "\tcards/" << outputDirectory << "/" << fileName << ".png" << endl;
        file << endl;

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
        drawDefaultJoker(file, fileName, suit);
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
    ofstream file(scriptFilename.c_str());

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
    {
        file << argv[i] << ' ';
    }
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
    {
        file << argv[i] << ' ';
    }
    file << endl;
    file << "./" << scriptFilename << endl;
    file << "EOM" << endl;
    file << endl;
    file << "chmod +x cards/" << outputDirectory	<< "/" << refreshFilename << endl;
    file << endl;


//- Initial blank card string used as a template for each card.
    string startString = genStartString();
    string suit;
    string card;

//- Generate all the playing cards.
    for (int s = 0; s < ELEMENTS(suits); ++s)
    {
        suit    = string(suits[s]);

        string pipFile = string("pips/") + pipDirectory + "/" + suit + "S.png";     // Try small pip file first.
        desc pipD(cornerPipInfo, pipFile);
        if (!pipD.isFileFound())
        {
            // Small pip file not found, so use standard pip file.
            pipFile = string("pips/") + pipDirectory + "/" + suit + ".png";
            pipD.setFileName(pipFile);
        }

        // Generate the playing cards in the current suit.
        pipFile = string("pips/") + pipDirectory + "/" + suit + ".png";             // Use standard pip file.
        desc standardPipD(standardPipInfo, pipFile);
        for (int c = 1; c < ELEMENTS(cards); ++c)
        {
            // Set up the variables.
            card = string(cards[c]);

            string indexFile = string("indices/") + indexDirectory + "/" + suit + card + ".png";
            desc indexD(indexInfo, indexFile);
            if (!indexD.isFileFound())
            {
                // indexInfo for suit file not found, so use alternate index file.
                indexFile = string("indices/") + indexDirectory + "/" + string(alts[s]) + card + ".png";
                indexD.setFileName(indexFile);
            }

            string faceFile = string("faces/") + faceDirectory + "/" + suit + card + ".png";
            desc faceD(imageHeight, imageX, imageY, faceFile);

            string drawFace;

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
            file << "# Draw the " << cardNames[c] << " of " << suitNames[s] << " as file " << suit << card << ".png." << endl;
            file << startString;

            if ((faceD.useStandardPips()) || (faceD.isFileFound() && faceD.isLandscape()))
            {
                file << drawFace;			// Draw either half of the pips or one of the landscape images.
            }
            file << pipD.draw();			// Draw corner pip.
            file << indexD.draw();			// Draw index.

            file << "\t-rotate 180 \\" << endl;

            if (faceD.useStandardPips())
            {
                drawFace = drawStandardPips(false, c, standardPipD);
            }
            file << drawFace;				// Draw either the rest of the pips or the needed image.
            file << pipD.draw();			// Draw corner pip.
            file << indexD.draw();			// Draw index.

            file << "\t+dither -colors 256 \\" << endl;
            file << "\tcards/" << outputDirectory << "/" << suit << card << ".png" << endl;

            file << endl;
        }
    }


//- Add the Jokers using narrower boarders.
    boarderX = 7;
    boarderY = 5;
    indexInfo.setH(30.0);
    indexInfo.setY(20.0);
    recalculate();

    int fails = 0;
    for (int s = 0; s < ELEMENTS(suits); ++s)
    {
        fails += drawJoker(fails, file, s);
    }

    file << "echo Output created in cards/" << outputDirectory << "/" << endl;
    file << endl;

    return 0;
}
