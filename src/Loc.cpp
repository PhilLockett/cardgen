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

#if !defined ELEMENTS
#define ELEMENTS(A) (sizeof(A)/sizeof(A[0]))
#endif

static const struct
{
    size_t  xIndex;
    size_t  yIndex;
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

static const std::vector<size_t> corners{ 3, 5 };
static const std::vector<size_t> ace{ 0 };
static const std::vector<size_t> c2{ 1, 2 };
static const std::vector<size_t> c3{ 1, 0, 2 };
static const std::vector<size_t> c4{ 3, 4, 5, 6 };
static const std::vector<size_t> c5{ 3, 4, 0, 5, 6 };
static const std::vector<size_t> c6{ 3, 4, 5, 6, 7, 8 };
static const std::vector<size_t> c7{ 3, 4, 5, 6, 7, 8, 9 };
static const std::vector<size_t> c8{ 3, 4, 10, 5, 6, 7, 8, 9 };
static const std::vector<size_t> c9{ 3, 4, 11, 12, 0, 5, 6, 13, 14 };
static const std::vector<size_t> c10{ 3, 4, 11, 12, 15, 5, 6, 13, 14, 16 };
static const std::vector<size_t> jack{ 3, 4, 10, 11, 12, 0, 5, 6, 9, 13, 14 };
static const std::vector<size_t> queen{ 1, 3, 4, 10, 11, 12, 2, 5, 6, 9, 13, 14 };
static const std::vector<size_t> king{ 1, 3, 4, 10, 11, 12, 0, 2, 5, 6, 9, 13, 14 };

static const std::vector<std::vector<size_t>> _patterns{
    corners, ace, c2, c3, c4, c5, c6, c7, c8, c9, c10, jack, queen, king
};

const Loc::Container Loc::offsets{
    (1.0F / 2),
    (0.0F),
    (1.0F),
    (1.0F / 4),
    (1.0F / 3),
    (1.0F / 6)

};

Loc::Loc(size_t x, size_t y, bool r, float viewportWindowX, float viewportWindowY) : 
    xIndex{isIndex(x) ? offsets[x] * viewportWindowX : 0},
    yIndex{isIndex(y) ? offsets[y] * viewportWindowY : 0},
    rotate{r}
{ }


std::vector<Loc> Pattern::locations{};

void Pattern::init(float viewportWindowX, float viewportWindowY)
{
    const size_t MAX{ELEMENTS(loc)};
    locations.clear();
    for (int i{}; i < MAX; ++i)
        locations.emplace_back(loc[i].xIndex, loc[i].yIndex, loc[i].rotate, viewportWindowX, viewportWindowY);
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

PatternCollection::Container PatternCollection::patterns{};

void PatternCollection::init(void)
{
    const size_t MAX{_patterns.size()};
    patterns.clear();
    for (auto pattern : _patterns)
        patterns.emplace_back(pattern);
}

void PatternCollection::calibrate(float viewportWindowX, float viewportWindowY)
{
    Pattern::init(viewportWindowX, viewportWindowY);
    PatternCollection::init();
}

