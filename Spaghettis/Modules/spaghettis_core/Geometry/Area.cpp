
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core::Area {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::tuple<float, float> Real::getProportions (Point::Real pt)
{
    if (!contains (pt)) { return { 0.0f, 0.0f }; }
    
    const float dX = static_cast<float> (pt.getPoint().getX() - r_.getX());
    const float dY = static_cast<float> (pt.getPoint().getY() - r_.getY());
    const float rX = dX / r_.getWidth();
    const float rY = dY / r_.getHeight();
    
    return { rX, rY };
}

Point::Real Real::getOffsetForProportions (Point::Real pt, float rX, float rY)
{
    const float dX = rX * r_.getWidth();
    const float dY = rY * r_.getHeight();
    const float x  = pt.getPoint().getX() - dX;
    const float y  = pt.getPoint().getY() - dY;
    
    return Point::Real (static_cast<int> (x), static_cast<int> (y));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
