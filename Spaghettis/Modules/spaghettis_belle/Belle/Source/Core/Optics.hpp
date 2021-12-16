
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Optics {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static double nonCollidingByOutlines (const Path& p1, const Path& p2, Point pt, double angle)
    {
        const Array<Polygon>& a = p1.getOutlines();
        const Array<Polygon>& f = p2.getOutlines();
                  
        double distance = 0.0;
        
        for (int i = 0; i < a.size(); ++i) {
        //
        for (int j = 0; j < f.size(); ++j) {
        //
        double d = nonCollidingDistance (a[i], f[j], pt, angle);
        distance = Math::max (distance, d);
        //
        }
        //
        }
        
        return distance;
    }
    
    static double nonCollidingByHull (const Path& p1, const Path& p2, Point pt, double angle)
    {
        Polygon a (p1.getHull());
        Polygon f (p2.getHull());

        return Math::max (nonCollidingDistance (a, f, pt, angle), 0.0);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static double nonCollidingHorizontalByHull (const Path& left, const Path& right)
    {
        if (left.isEmpty() || right.isEmpty()) { return 0.0; }
        else {
            return Optics::nonCollidingByHull (right, left, Point(), kPi);
        }
    }
    
    static double nonCollidingHorizontalByOutlines (const Path& left, const Path& right)
    {
        if (left.isEmpty() || right.isEmpty()) { return 0.0; }
        else {
            return Optics::nonCollidingByOutlines (right, left, Point(), kPi);
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static double nonCollidingVerticalByHull (const Path& top, const Path& bottom)
    {
        if (top.isEmpty() || bottom.isEmpty()) { return 0.0; }
        else {
            return Optics::nonCollidingByHull (bottom, top, Point(), kHalfPi);
        }
    }
    
    static double nonCollidingVerticalByOutlines (const Path& top, const Path& bottom)
    {
        if (top.isEmpty() || bottom.isEmpty()) { return 0.0; }
        else {
            return Optics::nonCollidingByOutlines (bottom, top, Point(), kHalfPi);
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static double nonCollidingDistance (const Polygon& a, const Polygon& f, Point pt, double angle)
    {
        /* Rotate to get an horizontal placement. */
        
        Affine aAffine = Affine::rotation (-angle);  
        Affine fAffine = aAffine * Affine::translation (pt);
        
        /* Get the sides of the polygons. */
        
        Array<Line> aLines;
        Array<Line> fLines;
          
        aLines.resize (a.size());
        fLines.resize (f.size());
          
        for (int i = 0; i < a.size(); ++i) {
            int n = (i ? i - 1 : a.size() - 1);
            aLines[i] = Line (aAffine.appliedTo (a.get (i)), aAffine.appliedTo (a.get (n)));
        }

        for (int i = 0; i < f.size(); ++i) {
            int n = (i ? i - 1 : f.size() - 1);
            fLines[i] = Line (fAffine.appliedTo (f.get (i)), fAffine.appliedTo (f.get (n)));
        }
        
        /* Compare them all one by one. */
        
        double temp, distance = 0.0;                      
        
        for (int i = 0; i < aLines.size(); ++i) {
        //
        Line line1 = aLines[i];
        
        for (int j = 0; j < fLines.size(); ++j) {
        //
        Line line2 = Line::makeClipped (fLines[j], line1.getStart().getY(), line1.getEnd().getY());
        
        Point pt1 = line2.getStart();
        Point pt2 = line2.getEnd();
        
        if (line1.horizontalDistanceFromPoint (pt1, temp)) { distance = Math::min (temp, distance); }
        if (line1.horizontalDistanceFromPoint (pt2, temp)) { distance = Math::min (temp, distance); }
        //
        }
        //
        }
        
        return Math::abs (Math::min (distance, 0.0));
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
