/**
 * @file    Loc.cpp
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
 * Constants for drawStandardPips().
 */

#include <iostream>
#include <iterator>

#include "Loc.h"


/**
 * @section Loc class method code.
 *
 */

const Loc::Container Loc::offsets{
    (1.0F / 2),
    (0.0F),
    (1.0F),
    (1.0F / 4),
    (1.0F / 3),
    (1.0F / 6)

};

Loc::Loc(const Loc_s & loc, float viewportWindowX, float viewportWindowY) : 
    xIndex{isIndex(loc.xIndex) ? offsets[loc.xIndex] * viewportWindowX : 0},
    yIndex{isIndex(loc.yIndex) ? offsets[loc.yIndex] * viewportWindowY : 0},
    rotate{loc.rotate}
{ }


/**
 * @section Pattern class method code.
 *
 */

const std::vector<Loc_s> Pattern::locs{
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

std::vector<Loc> Pattern::locations{};

void Pattern::init(float viewportWindowX, float viewportWindowY)
{
    locations.clear();
    for (const Loc_s & loc : locs)
        locations.emplace_back(loc, viewportWindowX, viewportWindowY);
}

Pattern::Pattern(const std::vector<Index> & v)
{
    for (auto index : v)
    {
        if (isIndex(index))
        {
            const auto & location{locations.at(index)};
            if (location.isRotate())
                southern.emplace_back(location.getX(), location.getY());
            else
                northern.emplace_back(location.getX(), location.getY());
        }
    }
    rotate = true;
}


/**
 * @section PatternCollection class method code.
 *
 */

const std::vector<std::vector<size_t>> PatternCollection::pats{
    { 3, 5 },
    { 0 },
    { 1, 2 },
    { 1, 0, 2 },
    { 3, 4, 5, 6 },
    { 3, 4, 0, 5, 6 },
    { 3, 4, 5, 6, 7, 8 },
    { 3, 4, 5, 6, 7, 8, 9 },
    { 3, 4, 10, 5, 6, 7, 8, 9 },
    { 3, 4, 11, 12, 0, 5, 6, 13, 14 },
    { 3, 4, 11, 12, 15, 5, 6, 13, 14, 16 },
    { 3, 4, 10, 11, 12, 0, 5, 6, 9, 13, 14 },
    { 1, 3, 4, 10, 11, 12, 2, 5, 6, 9, 13, 14 },
    { 1, 3, 4, 10, 11, 12, 0, 2, 5, 6, 9, 13, 14 },

};

PatternCollection::Container PatternCollection::patterns{};

void PatternCollection::init(void)
{
    patterns.clear();
    for (auto pattern : pats)
        patterns.emplace_back(pattern);
}

void PatternCollection::calibrate(float viewportWindowX, float viewportWindowY)
{
    Pattern::init(viewportWindowX, viewportWindowY);
    PatternCollection::init();
}

