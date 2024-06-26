
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core::Vector {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

Scaled::Scaled (const Real& r, float f) : Scaled (Geometry::scaled (r.getPoint(), f), f)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Real::Real (const Scaled& s) : Real (Geometry::unscaled (s.getPoint(), s.getScale()))
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Real operator + (const Real& a, const Real& b)
{
    return Real (a.getPoint() + b.getPoint());
}

Real operator / (const Real& a, float f)
{
    return Real (a.getPoint() / f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
