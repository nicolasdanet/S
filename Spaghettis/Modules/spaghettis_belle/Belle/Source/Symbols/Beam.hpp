
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Beam_%28music%29 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Parallelogram {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Parallelogram (Point tRight, Point tLeft, Point bLeft, Point bRight)
    {
        polygon_.add (tRight);
        polygon_.add (tLeft);
        polygon_.add (bLeft);
        polygon_.add (bRight);
    }

public:
    Parallelogram (const Parallelogram&) = default;
    Parallelogram (Parallelogram&&) = default;
    Parallelogram& operator = (const Parallelogram&) = default;
    Parallelogram& operator = (Parallelogram&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    operator const Polygon&() const
    {
        return polygon_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Box getBounds() const
    {
        Box bounds (getBottomLeft(), getTopRight());
        
        bounds += getBottomRight();
        bounds += getTopLeft();
        
        return bounds;
    }

    mica::Concept getDirection() const
    {
        if (getTopLeft().getY() > getTopRight().getY()) { return mica::Down; }
        else {
            return mica::Up;
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Point getTopRight() const
    {
        return polygon_.get (0);
    }
    
    Point getTopLeft() const
    {
        return polygon_.get (1);
    }
    
    Point getBottomLeft() const
    {
        return polygon_.get (2);
    }
    
    Point getBottomRight() const
    {
        return polygon_.get (3);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Parallelogram withAffine (const Parallelogram& p, const Affine& affine)
    {
        Point tRight = affine.appliedTo (p.getTopRight());
        Point tLeft  = affine.appliedTo (p.getTopLeft());
        Point bLeft  = affine.appliedTo (p.getBottomLeft());
        Point bRight = affine.appliedTo (p.getBottomRight());
        
        return Parallelogram (tRight, tLeft, bLeft, bRight);
    }
    
private:
    Polygon polygon_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Beam {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Beam (const Array<Box>& stems, mica::Concept direction) : stems_ (stems), direction_ (direction)
    {
        slant_ = 0.0;
        angle_ = 0.0;
    }

public:
    Beam (const Beam&) = default;
    Beam (Beam&&) = default;
    Beam& operator = (const Beam&) = default;
    Beam& operator = (Beam&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Beam& setSlant (double slant)
    {
        slant_ = slant; return *this;
    }
    
    Beam& setFlags (const Array<int>& flags)
    {
        flags_ = flags; return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addToPath (Path& path) const
    {
        if (stems_.size() >= 2) {
        //
        /* Note the two state pass to drawing primary then secondary beams. */
        
        if (comb_.size()) { addSecondary (path); comb_.clear(); }
        else {
            Parallelogram temp (getPrimary());
            Affine affine (getPlacement (temp));
            Parallelogram primary (Parallelogram::withAffine (temp, affine));
            addPrimary (path, primary); 
            path.getBox (mica::Beam) = primary.getBounds();
            path.setProperty (mica::Beam, primary.getDirection());
        }
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addPrimary (Path& path, const Parallelogram& primary) const
    {
        getComb (primary);
        
        if (direction_ == mica::Down) {
            path.add (Instruction::moveTo (primary.getBottomRight()));
            path.add (Instruction::lineTo (primary.getBottomLeft()));
        
        } else {
            path.add (Instruction::moveTo (primary.getTopRight()));
            path.add (Instruction::lineTo (primary.getTopLeft()));
        }
        
        for (int i = 0; i < comb_.size(); ++i) { path.add (Instruction::lineTo (comb_[i])); }
        
        path.add (Instruction::closePath());
    }

    void addSecondary (Path& path) const
    {
        if (flags_.size() == stems_.size()) {
        //
        for (int i = 2; i < (House::kBeamMaximum + 1); ++i) {
        //
        for (int j = 1; j < flags_.size(); ++j) {
        //
        bool left  = (flags_[j - 1] >= i);
        bool right = (flags_[j] >= i);
        bool next  = false;
        
        if (j < (flags_.size() - 1)) { next = (flags_[j + 1] >= i); }
        
        bool abort = (!left && right && (next || (!next && (j != (flags_.size() - 1))))); 
        
        if (!abort && (left || right)) {
        //
        Point a (comb_[((j - 1) * 4) + 2]);
        Point b (comb_[(j * 4) - 1]);
        
        double f = House::kNoteStemThickness / (std::cos (angle_) * 2.0);
        Vector v (Vector::withPolar (angle_, f));
        
        a -= v;
        b += v;
        
        double d = Math::min (House::kBeamShort / std::cos (angle_), (b - a).magnitude() * 0.67);
        
        Vector w (Vector::withPolar (angle_, d));
        Vector y1 (0.0, (i - 1) * (House::kBeamThickness + House::kBeamDistance));
        Vector y2 (0.0, House::kBeamThickness);
        
        if (left && !right)      { b = a + w; }
        else if (right && !left) { a = b - w; }
        
        if (direction_ == mica::Down) { 
            path.addPolygon (Parallelogram (b + y1, a + y1, a + y1 - y2, b + y1 - y2)); 
        } else {
            path.addPolygon (Parallelogram (b - y1 + y2, a - y1 + y2, a - y1, b - y1));
        }
        //
        }
        //
        }
        //
        }
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Parallelogram getPrimary() const
    {
        Point a, b;
        
        if (direction_ == mica::Down) {
            a = stems_.getFirst().getBottomLeft();
            b = stems_.getLast().getBottomRight();
            
        } else {
            a = stems_.getFirst().getTopLeft();
            b = stems_.getLast().getTopRight();
        }
        
        return Beam::makeParallelogram (b, (b.getX() - a.getX()), slant_);
    }

    Affine getPlacement (const Parallelogram& primary) const
    {
        Path beam;
        beam.addPolygon (primary);
        
        Path stems;
        for (int i = 0; i < stems_.size(); ++i) { stems.addRectangle (stems_[i]); }

        if (direction_ == mica::Down) {
            double d = Optics::nonCollidingVerticalByHull (stems, beam) - House::kBeamThickness;
            return Affine::translation (Vector (0.0, -d));
            
        } else {
            double d = Optics::nonCollidingVerticalByHull (beam, stems) - House::kBeamThickness;
            return Affine::translation (Vector (0.0, d));
        }
    }

    void getComb (const Parallelogram& primary) const
    {
        /* The beam baseline. */ 
        
        Point a, b;
        
        if (direction_ == mica::Down) { a = primary.getTopLeft(); b = primary.getTopRight(); } 
        else {
            a = primary.getBottomLeft(); b = primary.getBottomRight();
        }
        
        Line baseline (a, b);
        
        angle_ = baseline.angle();
        
        /* Projections of the stems onto the baseline. */
        
        Array<Point> intersections;
        
        for (int i = 0; i < stems_.size(); ++i) {
        //
        if (i != 0) {
            Point p1;
            baseline.intersectionWithVertical (stems_[i].getLeft(), p1);
            intersections.add (p1);
        }
        
        if (i != stems_.size() - 1) {
            Point p2;
            baseline.intersectionWithVertical (stems_[i].getRight(), p2);
            intersections.add (p2);
        }
        //
        }
        
        /* Bases of the stems. */
        
        Array<Point> bases;
                
        if (direction_ == mica::Down) {
        //
        for (int i = 0; i < stems_.size(); ++i) {
            bases.add (stems_[i].getTopLeft());
            bases.add (stems_[i].getTopRight());
        }
        //
        } else {
        //
        for (int i = 0; i < stems_.size(); ++i) {
            bases.add (stems_[i].getBottomLeft());
            bases.add (stems_[i].getBottomRight());
        }
        //
        }
        
        /* Merge to get the comb. */
        
        Array<Point> scoped;
        
        scoped.add (bases[0]);
        scoped.add (bases[1]);
        
        for (int i = 0; i < intersections.size(); i += 2) { 
            scoped.add (intersections[i]);
            scoped.add (intersections[i + 1]);
            scoped.add (bases[i + 2]);
            scoped.add (bases[i + 3]);
        }
        
        scoped.swapWith (comb_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static Parallelogram makeParallelogram (Point tRight, double width, double slant)
    {
        Point tLeft = tRight - Vector (width, slant);
        Point bLeft = tLeft - Vector (0.0, House::kBeamThickness);
        Point bRight = bLeft + Vector (width, slant);
        
        return Parallelogram (tRight, tLeft, bLeft, bRight);
    }
    
private:
    Array<Box> stems_;
    Array<int> flags_;
    mica::Concept direction_;
    double slant_;

private:
    mutable double angle_;
    mutable Array<Point> comb_;

private:
    PRIM_LEAK_DETECTOR (Beam)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
