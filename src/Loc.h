/**
 * @file    Loc.h
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
 * Interface for the drawStandardPips() constants.
 */

#if !defined _LOC_H_INCLUDED_
#define _LOC_H_INCLUDED_

#include <vector>
#include <tuple>


class Loc
{
public:
	using Index = size_t;
	using Offset = float;
	using Container = std::vector<Offset>;

private:
    static const Container offsets;

    const Offset    xIndex;
    const Offset    yIndex;
    const bool      rotate;

    static bool isIndex(Index index) { return index < offsets.size(); }

public:
    Loc(size_t x, size_t y, bool r, float viewportWindowX, float viewportWindowY);

    Offset getX(void) const { return xIndex; }
    Offset getY(void) const { return yIndex; }
    bool isRotate(void) const { return rotate; }

};

struct Loc_s
{
    size_t  xIndex;
    size_t  yIndex;
    bool    rotate;

};

class Pattern
{
public:
	using Index = size_t;
	using Container = std::vector<Loc>;
	using Pair = std::pair<Loc::Offset, Loc::Offset>;
	using Pairs = std::vector<Pair>;
	using Iterator = Pairs::const_iterator;

private:
    static const std::vector<Loc_s> locs;
    static std::vector<Loc> locations;

    Pairs   northern;
    Pairs   southern;
    bool    rotate;

    static const Loc & getSafeLoc(Index loc)
        { return isIndex(loc) ? locations.at(loc) : locations.at(0); }

public:
    Pattern(const std::vector<Index> & v);
    static void init(float viewportWindowX, float viewportWindowY);

    const Iterator begin(void) const { return rotate ? southern.begin() : northern.begin(); }
    const Iterator end(void) const { return rotate ? southern.end() : northern.end(); }

    static bool isIndex(Index index) { return index < locations.size(); }

    bool isRotate(void) const { return rotate; }

    void setRotate(bool state) { rotate = state; }

};

class PatternCollection
{
public:
	using Index = size_t;
	using Container = std::vector<Pattern>;

private:
    static const std::vector<std::vector<size_t>> pats;
    static Container patterns;

    static const Pattern & getSafePat(Index pat)
        { return isIndex(pat) ? patterns.at(pat) : patterns.at(0); }

    static void init(void);

public:
    PatternCollection(void) { }

    static void calibrate(float viewportWindowX, float viewportWindowY);

    static bool isIndex(Index index) { return index < patterns.size(); }
    static const Pattern & getPattern(Index pat) { return getSafePat(pat); }

};


#endif //!defined _LOC_H_INCLUDED_

