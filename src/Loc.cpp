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

#include <iterator>

#include "Loc.h"


const Loc::Container Loc::offsets{
    (1.0F / 2),
    (0.0F),
    (1.0F),
    (1.0F / 4),
    (1.0F / 3),
    (1.0F / 6)

};

Loc::Loc(size_t x, size_t y, bool r) : 
    xIndex{isIndex(x) ? offsets[x] : 0},
    yIndex{isIndex(y) ? offsets[y] : 0},
    rotate{r},
    valid{isIndex(x) && isIndex(y)}
{ }


const Pattern::Container Pattern::locations{
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

Pattern::Pattern(const std::vector<Index> & v)
{
    valid = true;
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
        else
            valid = false;
    }
    rotate = true;
}

static const Pattern corners{{ 3, 5 }};
static const Pattern ace{{ 0 }};
static const Pattern c2{{ 1, 2 }};
static const Pattern c3{{ 1, 0, 2 }};
static const Pattern c4{{ 3, 4, 5, 6 }};
static const Pattern c5{{ 3, 4, 0, 5, 6 }};
static const Pattern c6{{ 3, 4, 5, 6, 7, 8 }};
static const Pattern c7{{ 3, 4, 5, 6, 7, 8, 9 }};
static const Pattern c8{{ 3, 4, 10, 5, 6, 7, 8, 9 }};
static const Pattern c9{{ 3, 4, 11, 12, 0, 5, 6, 13, 14 }};
static const Pattern c10{{ 3, 4, 11, 12, 15, 5, 6, 13, 14, 16 }};
static const Pattern jack{{ 3, 4, 10, 11, 12, 0, 5, 6, 9, 13, 14 }};
static const Pattern queen{{ 1, 3, 4, 10, 11, 12, 2, 5, 6, 9, 13, 14 }};
static const Pattern king{{ 1, 3, 4, 10, 11, 12, 0, 2, 5, 6, 9, 13, 14 }};

const PatternCollection::Container PatternCollection::patterns{
    corners,
    ace, c2, c3, c4, c5, c6, c7, c8, c9, c10,
    jack, queen, king

};

