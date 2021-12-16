
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Bar_%28music%29 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Barline {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Barline() : type_ (Standard), extra_ (0.0), isLarge_ (false)
    {
    }

public:
    Barline (const Barline&) = default;
    Barline (Barline&&) = default;
    Barline& operator = (const Barline&) = default;
    Barline& operator = (Barline&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
public:
    void addToPath (Path& path) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    enum Type { Standard = 0, Double, End, EndRepeat };
    
protected:
    Type type_;
    double extra_;
    bool isLarge_;

private:
    PRIM_LEAK_DETECTOR (Barline)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct DoubleBarline : public Barline {

public:
    DoubleBarline()
    {
        type_ = Double;
    }
};

struct EndBarline : public Barline {

public:
    EndBarline()
    {
        type_ = End;
    }
};

struct EndRepeatBarline : public Barline {

public:
    EndRepeatBarline()
    {
        type_ = EndRepeat;
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct BarlineLarge : public Barline {

public:
    BarlineLarge (double d = 0.0)
    {
        extra_   = d;
        isLarge_ = true;
    }
};

struct DoubleBarlineLarge : public DoubleBarline {

public:
    DoubleBarlineLarge (double d = 0.0)
    {
        extra_   = d;
        isLarge_ = true;
    }
};

struct EndBarlineLarge : public EndBarline {

public:
    EndBarlineLarge (double d = 0.0)
    {
        extra_   = d;
        isLarge_ = true;
    }
};

struct EndRepeatBarlineLarge : public EndRepeatBarline {

public:
    EndRepeatBarlineLarge (double d = 0.0)
    {
        extra_   = d;
        isLarge_ = true;
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Shapes::Barline::addToPath (Path& path) const
{
    Point a (0.0, (House::kStaffHeight + House::kStaffLineThickness) / 2.0);
    Point b = -a;
    
    if (isLarge_) {
        path.setProperty (mica::Large, mica::True);
        b -= Vector (0.0, House::kStaffDistance - House::kStaffLineThickness + extra_); 
    }
    
    if (type_ == Double) { path.addLine (a, b, House::kDoubleBarlineThickness, false); }
    else {
        path.addLine (a, b, House::kBarlineThickness, false);
    }
    
    if (type_ == Double) {
        Vector v (House::kDoubleBarlineDistance, 0.0);
        path.addLine ((a + v), (b + v), House::kDoubleBarlineThickness, false);
    }
    
    if (type_ == End || type_ == EndRepeat) {
        Vector v (House::kBarlineDoubleDistance, 0.0);
        path.addLine ((a + v), (b + v), House::kBarlineDoubleThickness, false);
    }
    
    if (type_ == EndRepeat) {
        double x = House::kBarlineDotDistance;
        double y = House::kBarlineDotSize;
        path.addCircle (Vector (-x,  y), y);
        path.addCircle (Vector (-x, -y), y);
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
