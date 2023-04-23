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
    const bool      valid;

    static bool isIndex(Index index) { return index < offsets.size(); }

public:
    Loc(size_t x, size_t y, bool r);

    Offset getX(void) const { return xIndex; }
    Offset getY(void) const { return yIndex; }
    bool isRotate(void) const { return rotate; }
    bool isValid(void) const { return valid; }

};

class Pattern
{
public:
	using Index = size_t;
	using Container = std::vector<Loc>;
	using Iterator = Container::const_iterator;

private:
    static const Container locations;

    Container indices;
    bool      valid;

    static const Loc & getSafeLoc(Index loc)
        { return isIndex(loc) ? locations.at(loc) : locations.at(0); }

public:
    Pattern(const std::vector<Index> & v);

    const Iterator begin(void) const { return indices.begin(); }
    const Iterator end(void) const { return indices.end(); }

    static bool isIndex(Index index) { return index < locations.size(); }
    const Loc & getLocation(Index loc) { return getSafeLoc(loc); }
    bool isValid(void) const { return valid; }

};

class PatternCollection
{
public:
	using Index = size_t;
	using Container = std::vector<Pattern>;

private:
    static const Container patterns;

    static const Pattern & getSafePat(Index pat)
        { return isIndex(pat) ? patterns.at(pat) : patterns.at(0); }

public:
    PatternCollection(void) { }

    static bool isIndex(Index index) { return index < patterns.size(); }
    const Pattern & getPattern(Index pat) { return getSafePat(pat); }

};



#endif //!defined _LOC_H_INCLUDED_

