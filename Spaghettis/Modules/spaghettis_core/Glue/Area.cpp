
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core::Area {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::tuple<float, float> Real::getProportions (core::Point::Real pt)
{
    if (!contains (pt)) { return { 0.0f, 0.0f }; }
    
    const float dX = static_cast<float> (pt.getPoint().getX() - r_.getX());
    const float dY = static_cast<float> (pt.getPoint().getY() - r_.getY());
    const float rX = dX / r_.getWidth();
    const float rY = dY / r_.getHeight();
    
    return { rX, rY };
}

core::Point::Real Real::getOffsetForProportions (core::Point::Real pt, float rX, float rY)
{
    const float dX = rX * r_.getWidth();
    const float dY = rY * r_.getHeight();
    const float x  = pt.getPoint().getX() - dX;
    const float y  = pt.getPoint().getY() - dY;
    
    return core::Point::Real (static_cast<int> (x), static_cast<int> (y));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
