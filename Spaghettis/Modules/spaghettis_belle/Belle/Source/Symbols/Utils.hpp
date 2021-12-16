
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Various utilities while constructing symbols. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Utils {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Ratio getUndottedValue (Ratio noteValue)
{
    Ratio undotted = 1;
    
    while (undotted < noteValue) { undotted *= 2; }
    while (undotted > noteValue) { undotted /= 2; }
    
    return undotted;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int countDots (Ratio noteValue)
{
    const int maximum = House::kRhythmicDotMaximum;
    
    Ratio base = getUndottedValue (noteValue);
    
    if (base == noteValue) { return 0; }
    else {
    //
    Ratio t = 1;
    
    for (int i = 1; i <= maximum; ++i) {
        t *= Ratio (1, 2);
        Ratio dotted = base * (Ratio (2) - t);
        if (dotted == noteValue) { return i; }
    }
    
    return -1;
    //
    }
}

static int countFlags (Ratio noteValue)
{
    int flags = 0;
    Ratio base = getUndottedValue (noteValue);
    
    while (base < Ratio (1, 4)) { flags++; base *= 2; }
    
    return flags;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int getTopLine()
{
    return House::kStaffLines - 1;
}

static int getBottomLine()
{
    return 1 - House::kStaffLines;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static bool isOverLines (int linespace)
{
    return ((linespace <= getTopLine() + 1) && (linespace >= getBottomLine() - 1));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static double getPosition (int linespace)
{
    return linespace * (House::kStaffSpaceWidth / 2.0); 
}

static int getLinespace (double position)
{
    if (position >= 0.0) { return static_cast<int> ((position / (House::kStaffSpaceWidth / 2.0)) + 0.5); } 
    else {
        return static_cast<int> ((position / (House::kStaffSpaceWidth / 2.0)) - 0.5);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Binary_logarithm > */

// -- Half          -> sqrt2 * sqrt2
// -- Quarter       -> sqrt2
// -- Eighth        -> 1
// -- Sixteenth     -> 1 / sqrt2
// -- ThirtySecond  -> 1 / (sqrt2 * sqrt2)

static double getWeight (Ratio noteValue)
{
    if (noteValue.isUndefined()) { return 0.0; }
    else {
        return std::pow (kRoot2, std::log ((noteValue / Ratio (1, 8)).asDouble()) * kLog2Inv);
    }
}

static mica::Concept getDirection (Array<int>& linespaces)
{
    mica::Concept direction = mica::Down;
    
    if (linespaces.size()) {
    //
    linespaces.sort();
    
    int a = linespaces.getLast();
    int b = linespaces.getFirst();
      
    if (a < 0)                  { direction = mica::Up;   }
    else if (b > 0)             { direction = mica::Down; }
    else if (a < Math::abs (b)) { direction = mica::Up;   }
    else if (a > Math::abs (b)) { direction = mica::Down; }
    else {
        int sum = 0;
        for (int i = 0; i < linespaces.size(); ++i) { sum += linespaces[i]; }
        direction = (sum < 0) ? mica::Up : mica::Down;
    }
    //
    }
    
    return direction;
}

static mica::Concept getMeter (int beats, int noteValue)
{
    PRIM_ASSERT (beats >= 1);
    PRIM_ASSERT (noteValue >= 1);
    
    if (Math::isPowerOfTwo (noteValue)) {
    //
    if (beats % 3 == 0)      { return mica::CompoundMeter; }
    else if (beats % 2 == 0) { return mica::SimpleMeter;   }
    else {
        return mica::ImperfectMeter;
    }
    //
    }
    
    return mica::IrrationalMeter;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
