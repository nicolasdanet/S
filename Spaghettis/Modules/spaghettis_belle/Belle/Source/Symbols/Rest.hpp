
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Rest_%28music%29 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Rest {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Rest (Ratio duration) : duration_ (duration)
    {
    }

public:
    Rest (const Rest&) = default;
    Rest (Rest&&) = default;
    Rest& operator = (const Rest&) = default;
    Rest& operator = (Rest&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addToPath (Path& path) const
    {
        double distance = House::kRhythmicDotDistance;
        double extra    = House::kRhythmicDotExtra;
        
        int count  = Utils::countDots (duration_);
        Ratio base = Utils::getUndottedValue (duration_);
        
        int linespace = 1;          /* Default dots placement. */
        unicode glyph = 0x004f;     /* Default is a whole rest. */
        
        if (base == Ratio (1, 2))        { glyph += 1; }
        else if (base == Ratio (1, 4))   { glyph += 2; }
        else if (base == Ratio (1, 8))   { glyph += 3; }
        else if (base == Ratio (1, 16))  { glyph += 4; }
        else if (base == Ratio (1, 32))  { glyph += 5; linespace = Utils::getTopLine() - 1; }
        else if (base == Ratio (1, 64))  { glyph += 6; linespace = Utils::getTopLine() - 1; }
        else if (base == Ratio (1, 128)) { glyph += 7; linespace = Utils::getTopLine() + 1; }
        
        Path rest;
        rest.addPath (House::getInstance().getJoie().getGlyph (glyph)->getPath(), House::kAffineJoie);
        rest.apply (Affine::translation (Vector (-rest.getBounds().getWidth() / 2.0, 0.0)));
        
        Path dots;
        double x = rest.getBounds().getRight() + distance;
        double y = Utils::getPosition (linespace);
        
        for (int i = 0; i < count; ++i) {
            Affine origin = Affine::translation (Vector (x + i * extra, y));
            dots.addShape (Shapes::RhythmicDot(), origin);
        }
        
        path.addPath (rest);
        path.addPath (dots);
    }

private:
    Ratio duration_;

private:
    PRIM_LEAK_DETECTOR (Rest)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
