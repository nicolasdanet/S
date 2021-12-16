
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

class Polygon {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    class SortablePoint {
    
    public:
        SortablePoint() : angle_ (0.0), distance_ (0.0)
        {
        }
        
        SortablePoint (Point pt, double ang, double mag) : pt_ (pt), angle_ (ang), distance_ (mag)
        {
        }
    
        SortablePoint (const SortablePoint&) = default;
        SortablePoint (SortablePoint&&) = default;
        SortablePoint& operator = (const SortablePoint&) = default;
        SortablePoint& operator = (SortablePoint&&) = default;

    public:
        Point toPoint()
        {
            return pt_;
        }
        
    public:
        bool operator < (const SortablePoint & o) const
        {
            return (angle_ < o.angle_) || (angle_ == o.angle_ && distance_ < o.distance_);
        }
        
        bool operator > (const SortablePoint & o) const
        {
            return (angle_ > o.angle_) || (angle_ == o.angle_ && distance_ > o.distance_);
        }
        
        bool operator == (const SortablePoint & o) const
        {
            return (angle_ == o.angle_) && (distance_ == o.distance_);
        }
        
    private:
        Point pt_;
        double angle_;
        double distance_;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Polygon()
    {
    }

    Polygon (const Polygon&) = default;
    Polygon (Polygon&&) = default;
    Polygon& operator = (const Polygon&) = default;
    Polygon& operator = (Polygon&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (Polygon& o)
    {
        points_.swapWith (o.points_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
public:
    int size() const
    {
        return points_.size();
    }

    void clear()
    {
        points_.clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const Point get (int i) const
    {
        return points_[i];
    }
    
    void add (Point pt)
    {
        points_.add (pt);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.dcs.gla.ac.uk/~pat/52233/slides/Hull1x1.pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void convertToConvexHull (Array<Point>* removed = nullptr)      /* The Graham scan algorithm. */
    {
        if (points_.size() > 2)   {
        //
        if (points_.size() > 100) { quickElimination (removed); }       /* Does it worth the cost? */
        
        int j = 0;
        double x = points_.getFirst().getX();
        double y = points_.getFirst().getY();
        
        for (int i = 1; i < points_.size(); ++i) { 
            if ((points_[i].getY() < y) || (points_[i].getY() == y && points_[i].getX() < x)) { 
                y = points_[i].getY(); 
                x = points_[i].getX();
                j = i; 
            }
        }
        
        Array<SortablePoint> l1;
        
        l1.add (SortablePoint (points_[j], 0.0, 0.0));
        
        for (int i = 0; i < points_.size(); ++i) {
        //
        if (i != j) { 
            Vector v (points_[i] - points_[j]);
            l1.add (SortablePoint (points_[i], v.angle(), v.magnitude())); 
        }
        //
        }
        
        l1.sort();
        
        Array<Point> l2;
        
        l2.add (l1[0].toPoint());
        l2.add (l1[1].toPoint());
        
        for (int i = 2; i < l1.size(); ++i) {
        //
        Point pt = l1[i].toPoint();
        while (l2.size() > 1 && clockwiseOrder (l2[l2.size() - 2], l2[l2.size() - 1], pt) != -1) {
            if (removed) { removed->add (l2[l2.size() - 1]); }
            l2.resize (l2.size() - 1);
        }
        
        l2.add (pt);
        //
        }
   
        l2.swapWith (points_);
        //
        }
    }
    
    void quickElimination (Array<Point> * removed)
    {
        Point t[4]; t[0] = t[1] = t[2] = t[3] = points_.getFirst();
        
        for (int i = 1; i < points_.size(); ++i) {
        //
        if (points_[i].getY() > t[0].getY()) { t[0] = points_[i]; }
        if (points_[i].getX() < t[1].getX()) { t[1] = points_[i]; }
        if (points_[i].getY() < t[2].getY()) { t[2] = points_[i]; }
        if (points_[i].getX() > t[3].getX()) { t[3] = points_[i]; }
        //
        }
        
        Array<Point> pt;
        for (int i = 0; i < 4; ++i) { if (pt.contains (t[i]) == false) { pt.add (t[i]); } }
        
        if (pt.size() == 4) {   /* Must be 4 different points. */
        //
        Array<Point> scoped;
        
        for (int i = 0; i < points_.size(); ++i) {
        //
        int a = clockwiseOrder (points_[i], pt[0], pt[1]);
        int b = clockwiseOrder (points_[i], pt[1], pt[2]);
        int c = clockwiseOrder (points_[i], pt[2], pt[3]);
        int d = clockwiseOrder (points_[i], pt[3], pt[0]);
        
        bool inside = ((a == b) && (b == c) && (c == d));
        
        if (inside) { if (removed) { removed->add (points_[i]); } }
        else {
            scoped.add (points_[i]);
        }
        //
        }
        
        points_.swapWith (scoped);
        //
        }
    }
    
private:
    Array<Point> points_;

private:
    PRIM_LEAK_DETECTOR (Polygon)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void swap (Polygon& a, Polygon& b);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void swap (Polygon& a, Polygon& b)
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace prim

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
