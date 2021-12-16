
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
// MARK: -

class Line {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Line() 
    {
    }
    
    Line (double x1, double y1, double x2, double y2) : a_ (x1, y1), b_ (x2, y2) 
    {
    }

    Line (Point a, Point b) : a_ (a), b_ (b) 
    {
    }

    Line (const Line&) = default;
    Line (Line&&) = default;
    Line& operator = (const Line&) = default;
    Line& operator = (Line&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
public:
    const Point getStart() const
    {
        return a_;
    }
    
    const Point getEnd() const
    {
        return b_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    double angle() const
    {
        return (b_ - a_).angle();
    }
    
    double length() const
    {
        return (b_ - a_).magnitude();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf > */
/* < http://web.engr.illinois.edu/~jeffe/teaching/373/notes/x06-sweepline.pdf > */
/* < http://stackoverflow.com/q/15311018 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    bool intersects (const Line& o) const           /* True with extremities contained. */
    {
        int t1 = clockwiseOrder (a_, b_, o.a_);
        int t2 = clockwiseOrder (a_, b_, o.b_);
        int t3 = clockwiseOrder (o.a_, o.b_, a_);
        int t4 = clockwiseOrder (o.a_, o.b_, b_);
        
        if ((t1 * t2 < 0) && (t3 * t4 < 0)) { return true; }
        else {
            if (t1 == 0 && Box (a_, b_).contains (o.a_))   { return true; }
            if (t2 == 0 && Box (a_, b_).contains (o.b_))   { return true; }
            if (t3 == 0 && Box (o.a_, o.b_).contains (a_)) { return true; }
            if (t3 == 0 && Box (o.a_, o.b_).contains (b_)) { return true; }
        }
        
        return false;
    }
    
    /* Get the point of intersection. */
    
    bool intersects (const Line& o, Point& pt) const
    {
        if (intersects (o)) { return intersectionWithLine (o, pt); }
        else {
            return false;
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool horizontalDistanceFromPoint (Point pt, double& distance) const
    {
        Point t;
        
        if (intersectionWithHorizontal (pt.getY(), t)) { distance = ((pt - t).getX()); return true; }
        else {
            return false;
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Line makeClipped (const Line& line, double f1, double f2)
    {
        using std::swap;
        
        bool swapped = false;
        bool clipped = false;
        
        double x1 = line.a_.getX();
        double y1 = line.a_.getY();
        double x2 = line.b_.getX();
        double y2 = line.b_.getY();
        
        if (y1 > y2) { swap (x1, x2); swap (y1, y2); swapped = true; }
        if (f1 > f2) { swap (f1, f2); }
        
        Point a (x1, y1);
        Point b (x2, y2);
        
        if (y1 < f1 && f1 < y2) { line.intersectionWithHorizontal (f1, a); clipped = true; }
        if (y1 < f2 && f2 < y2) { line.intersectionWithHorizontal (f2, b); clipped = true; }
      
        if (!clipped)       { return Line (line); }     /* Return original. */
        else if (!swapped)  { return Line (a, b); }
        else                { return Line (b, a); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool intersectionWithVertical (double x, Point& pt) const
    {
        double x1 = a_.getX();
        double x2 = b_.getX();
        double y1 = a_.getY();
        double y2 = b_.getY();
      
        if (x1 == x) { pt = a_; return true; } 
        else if (x2 == x) { pt = b_; return true; }
        
        double dx = -x + 1.0;       /* Convert to x = 1 case. */
        x1 += dx;
        x2 += dx;
      
        if ((x1 == x2) || (x1 > 1.0 && x2 > 1.0) || (x1 < 1.0 && x2 < 1.0)) { return false; }
        else {
        //
        double t = (x1 - 1.0) / (x1 - x2);      /* Substitution in parametric equation. */
        pt = Point (x, y1 + t * (y2 - y1));
        return true;
        //
        }
    }
    
    bool intersectionWithHorizontal (double y, Point& pt) const
    {
        double x1 = a_.getX();
        double x2 = b_.getX();
        double y1 = a_.getY();
        double y2 = b_.getY();
      
        if (y1 == y) { pt = a_; return true; } 
        else if (y2 == y) { pt = b_; return true; }
        
        double dy = -y + 1.0;       /* Convert to y = 1 case. */
        y1 += dy;
        y2 += dy;
      
        if ((y1 == y2) || (y1 > 1.0 && y2 > 1.0) || (y1 < 1.0 && y2 < 1.0)) { return false; }
        else {
        //
        double t = (y1 - 1.0) / (y1 - y2);      /* Substitution in parametric equation. */
        pt = Point (x1 + t * (x2 - x1), y);
        return true;
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    bool intersectionWithLine (const Line& o, Point& pt) const
    {
        double a1 = b_.getY() - a_.getY();
        double b1 = a_.getX() - b_.getX();
        double c1 = b_.getX() * a_.getY() - a_.getX() * b_.getY();
        
        double a2 = o.b_.getY() - o.a_.getY();
        double b2 = o.a_.getX() - o.b_.getX();
        double c2 = o.b_.getX() * o.a_.getY() - o.a_.getX() * o.b_.getY();

        double det = a1 * b2 - a2 * b1;
        
        if (det == 0.0) { return false; }
        else {
            pt = Point (((b1 * c2 - b2 * c1) / det), ((a2 * c1 - a1 * c2) / det)); 
            return true;
        }
    }

private:
    Point a_;
    Point b_;

private:
    PRIM_LEAK_DETECTOR (Line)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
