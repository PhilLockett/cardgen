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

#if !defined ROUND
#define ROUND(v) (int)((v) + 0.5)
#endif


/**
 * @section Default values for global variables.
 *
 */

const int _cardWidthPx{380};
const int _cardHeightPx{532};
const std::string _cardColour{"white"};

const info _indexInfo{10.5, 8.07, 9.84};
const info _cornerPipInfo{7.5, 8.07, 20.41};
const info _standardPipInfo{18.0, 25.7, 18.65};
const info _imagePipInfo{14.29, 12.63, 9.77};

const std::string _indexDirectory{"1"};
const std::string _pipDirectory{"1"};
const std::string _faceDirectory{"1"};

const std::string _scriptFilename{"draw.sh"};
const std::string _refreshFilename{"x_refresh.sh"};

const bool _keepAspectRatio{};
const bool _mpc{};
const int mpcWidth{750};
const int mpcHeight{1050};
const int mpcBorder{36};
const bool _quad{};

const float _cornerRadius{3.76};
const int _strokeWidth{2};
const int _borderOffset{1};

const float _imageBorderX{14.54};
const float _imageBorderY{10.14};
const float _originalImageWidth{100 - (2 * _imageBorderX)};
const float _originalImageHeight{50 - _imageBorderY};


/**
 * @section Common functions.
 *
 */

extern int generateScript(int argc, char *argv[]);


#endif //!defined _CARDGEN_H_INCLUDED_

