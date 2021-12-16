
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace prim { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Math { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Ellipse {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void intersectionWithVerticalTangent (double a, double b, double phi, Point& pt);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Ellipse

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Math

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://pomax.github.io/bezierinfo/ > */
/* < http://www.tinaja.com/glib/hack62.pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Bezier {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Bezier()
    {
    }

    Bezier (Point p0, Point p1, Point p2)       /* From a quadratic Bezier curve. */
    {
        setControlPoints (p0, p1, p2);
    }

    Bezier (Point p0, Point p1, Point p2, Point p3)
    {
        setControlPoints (p0, p1, p2, p3);
    }

    Bezier (const Bezier&) = default;
    Bezier (Bezier&&) = default;
    Bezier& operator = (const Bezier&) = default;
    Bezier& operator = (Bezier&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Point get (double t) const
    {
        PRIM_ASSERT ((t >= 0.0) && (t <= 1.0));
        
        double t2 = t * t;
        double t3 = t * t2;
        double x = a_ * t3 + b_ * t2 + c_ * t + d_;
        double y = e_ * t3 + f_ * t2 + g_ * t + h_;

        return Point (x, y);
    }

    void trim (double t0, double t1)
    {
        PRIM_ASSERT ((t0 >= 0.0) && (t0 <= 1.0));
        PRIM_ASSERT ((t1 >= 0.0) && (t1 <= 1.0));
        
        double a1 = t1 - t0;
        double b1 = t0;

        double a2 = a1 * a1;
        double a3 = a2 * a1;
        double b2 = b1 * b1;
        double b3 = b2 * b1;

        double newA = a3 * a_;
        double newB = 3.0 * a2 * b1 * a_ + a2 * b_;
        double newC = 3.0 * a1 * b2 * a_ + 2.0 * a1 * b1 * b_ + a1 * c_;
        double newD = b3 * a_ + b2 * b_ + b1 * c_ + d_;
        double newE = a3 * e_;
        double newF = 3.0 * a2 * b1 * e_ + a2 * f_;
        double newG = 3.0 * a1 * b2 * e_ + 2.0 * a1 * b1 * f_ + a1 * g_;
        double newH = b3 * e_ + b2 * f_ + b1 * g_ + h_;

        a_ = newA;
        b_ = newB;
        c_ = newC;
        d_ = newD;
        e_ = newE;
        f_ = newF;
        g_ = newG;
        h_ = newH;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void getControlPoints (Point& p0, Point& p1, Point& p2, Point& p3) const
    {
        p0.getX() = d_;
        p1.getX() = d_ + c_ / 3.0;
        p2.getX() = d_ + 2.0 / 3.0 * c_ + b_ / 3.0;
        p3.getX() = d_ + c_ + b_ + a_;
        p0.getY() = h_;
        p1.getY() = h_ + g_ / 3.0;
        p2.getY() = h_ + 2.0 / 3.0 * g_ + f_ / 3.0;
        p3.getY() = h_ + g_ + f_ + e_;
    }
    
    void setControlPoints (Point p0, Point p1, Point p2, Point p3)
    {
        a_ = p3.getX() - p0.getX() + 3.0 * (p1.getX() - p2.getX());
        b_ = 3.0 * (p2.getX() - 2.0 * p1.getX() + p0.getX());
        c_ = 3.0 * (p1.getX() - p0.getX());
        d_ = p0.getX();
        e_ = p3.getY() - p0.getY() + 3.0 * (p1.getY() - p2.getY());
        f_ = 3.0 * (p2.getY() - 2.0 * p1.getY() + p0.getY());
        g_ = 3.0 * (p1.getY() - p0.getY());
        h_ = p0.getY();
    }
    
    void setControlPoints (Point p0, Point p1, Point p2)    /* < http://fontforge.org/bezier.html > */
    {
        a_ = 0;
        b_ = p2.getX() - 2.0 * p1.getX() + p0.getX();
        c_ = 2.0 * (p1.getX() - p0.getX());
        d_ = p0.getX();
        e_ = 0;
        f_ = p2.getY() - 2.0 * p1.getY() + p0.getY();
        g_ = 2.0 * (p1.getY() - p0.getY());
        h_ = p0.getY();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Bezier reversed (const Bezier& curve)
    {
        Point p0, p1, p2, p3;
        curve.getControlPoints (p0, p1, p2, p3);
        return Bezier (p3, p2, p1, p0);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Bezier makeConvexCurveInBox (Box box, double curve, bool isUp = true)
    {
        Point p0, p1, p2, p3;
        
        if (isUp) {
            p0 = box.getBottomLeft();
            p1 = box.getTopLeft();
            p2 = box.getTopRight();
            p3 = box.getBottomRight();
            
        } else {
            p0 = box.getTopLeft();
            p1 = box.getBottomLeft();
            p2 = box.getBottomRight();
            p3 = box.getTopRight();
        }
        
        PRIM_ASSERT ((curve >= 0.0) && (curve <= 1.0));
        
        Vector v ((box.getWidth() / 2.0) * curve, 0.0);
        
        p1 += v;
        p2 -= v;
        
        return Bezier (p0, p1, p2, p3);
    }
    
    /* Default is counterclockwise. */

    static Bezier makeEllipticalArc (Point origin, Vector scale, double rotation, int quadrant)
    {
        const double magicNumber = 0.551784;    /* < http://www.tinaja.com/glib/ellipse4.pdf > */
        
        double a = scale.getX() * 0.5;          /* Semi-major axis. */
        double b = scale.getY() * 0.5;          /* Semi-minor axis. */
        
        PRIM_ASSERT ((quadrant >= 0) && (quadrant <= 4));
        
        if (quadrant == 3 || quadrant == 4) { rotation += kPi; quadrant -= 2; }

        Point p0, p1, p2, p3;
        
        if (quadrant == 1) {
        //
        p0.setPolar (rotation, a);
        p3.setPolar (rotation + kHalfPi, b);
        p1.setPolar (rotation + kHalfPi, b * magicNumber);
        p1 += p0;
        p2.setPolar (rotation, a * magicNumber);
        p2 += p3;
        //    
        } else if (quadrant == 2) {
        //
        p0.setPolar (rotation + kHalfPi, b);
        p3.setPolar (rotation + kPi, a);
        p1.setPolar (rotation + kPi, a * magicNumber);
        p1 += p0;
        p2.setPolar (rotation + kHalfPi, b * magicNumber);
        p2 += p3;
        //
        }

        p0 += origin;
        p1 += origin;
        p2 += origin;
        p3 += origin;

        return Bezier (p0, p1, p2, p3);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Functions below are about ellipse geometry. */
    
public:
    static double parameterOfVerticalTangentInFourthQuadrant (Vector scale, double phi)
    {
        double dummy, d = 0.0;
        Bezier ellipse = Bezier::makeEllipticalArc (Point (0.0, 0.0), scale, phi, 4);
        ellipse.findBoundedRoots (d, dummy);
        return d;
    }
    
    /* < https://en.wikipedia.org/wiki/Binary_search_algorithm > */
    
    static double parameterFromVerticalTangentInFirstQuadrant (Vector scale, double phi, double d)
    {
        const double tolerance = 0.001;
        const int steps = 30;
        
        Bezier ellipse = Bezier::makeEllipticalArc (Point (0.0, 0.0), scale, phi, 1);

        Point pt;
        Math::Ellipse::intersectionWithVerticalTangent (scale.getX() / 2.0, scale.getY() / 2.0, phi, pt);
        double x = pt.getX() - d;

        double l = 0.0;
        double r = 1.0;
        double m = 0.0;

        for (int i = 0; i < steps; ++i) {
        //
        m = (r + l) / 2.0;
        Point plot = ellipse.get (m);
        
        if (Math::abs (plot.getX() - x) < tolerance) { break; }
        else if (plot.getX() < x) { r = m; }
        else { 
            l = m; 
        }
        //
        }
        
        return m;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int findBoundedRoots (double& r1, double& r2)
    {
        int n = Math::roots (3.0 * a_, 2.0 * b_, c_, r1, r2);   /* Derivative of the parametric equation. */

        if (n == 2) {
            if (Math::isBetween (r1, 0.0, 1.0)) {
            if (Math::isBetween (r2, 0.0, 1.0)) { return 2; }
            else { 
                r2 = 0.0; return 1; 
            }
            } 
            else if (Math::isBetween (r2, 0.0, 1.0)) { r1 = r2; r2 = 0.0; return 1; } 
            else { r1 = r2 = 0.0; return 0; }

        } else if (n == 1) {
            if (Math::isBetween (r1, 0.0, 1.0)) { return 1; }
            else { r1 = 0.0; return 0; }
        }
        
        return 0;
    }

private:
    double a_;
    double b_;
    double c_;
    double d_;
    double e_;
    double f_;
    double g_;
    double h_;

private:
    PRIM_LEAK_DETECTOR (Bezier)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://stackoverflow.com/a/88020 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Math::Ellipse::intersectionWithVerticalTangent (double a, double b, double phi, Point& pt)
{
    double t = std::atan (-b * std::tan (phi) / a);
    
    double x = a * std::cos (t) * std::cos (phi) - b * std::sin (t) * std::sin (phi);
    double y = b * std::sin (t) * std::cos (phi) + a * std::cos (t) * std::sin (phi);
    
    pt = Point (x, y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
