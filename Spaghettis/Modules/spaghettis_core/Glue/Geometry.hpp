
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct Coordinates {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int realToLocal (int n)
{
    return Canvas::addOffset (n);
}

static juce::Point<int> realToLocal (juce::Point<int> pt)
{
    return juce::Point (realToLocal (pt.getX()), realToLocal (pt.getY()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int localToReal (int n)
{
    return Canvas::removeOffset (n);
}

static juce::Point<int> localToReal (juce::Point<int> pt)
{
    return juce::Point (localToReal (pt.getX()), localToReal (pt.getY()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Distance {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static float scaled (int n, float f)
{
    return (n * f);
}
    
static float unscaled (int n, float f)
{
    return (n / f);
}

template <class T> static juce::Point<T> scaled (juce::Point<T> pt, float f)
{
    return juce::Point<T> (pt.x * f, pt.y * f);
}

template <class T> static juce::Point<T> unscaled (juce::Point<T> pt, float f)
{
    return juce::Point<T> (pt.x / f, pt.y / f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------