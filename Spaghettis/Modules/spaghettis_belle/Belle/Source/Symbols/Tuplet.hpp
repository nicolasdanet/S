
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Tuplet > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Tuplet { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Numeral {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Numeral (const Box& beam, mica::Concept direction, int n) : beam_ (beam)
    {
        direction_ = direction;
        
        if (Math::isBetween (n, 1, 9)) { glyph_ = 0x0030 + n; }
        else {
            glyph_ = 0x0000;
        }
    }

public:
    Numeral (const Numeral&) = default;
    Numeral (Numeral&&) = default;
    Numeral& operator = (const Numeral&) = default;
    Numeral& operator = (Numeral&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void addToPath (Path& path) const
    {
        if (!beam_.isEmpty()) {
        //
        Path number (House::getInstance().getGentium().getGlyph (glyph_)->getPath());
        Vector v (number.getBounds().getCenter());
        path.addPath (number, getPlacement() * House::kAffineGentium * Affine::translation (-v));
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Affine getPlacement() const
    {
        Vector v (beam_.getCenter());
        
        if (direction_ == mica::Down) { v -= Vector (0.0, House::kTupletNumeralDistance); }
        else {
            v += Vector (0.0, House::kTupletNumeralDistance);
        }
        
        /* Avoid collision with staff lines. */
        
        int linespace = Utils::getLinespace (v.getY());
        
        if (Utils::isOverLines (linespace)) {
            if (direction_ == mica::Down) { 
                v.getY() = Utils::getPosition (Math::min (linespace, Utils::getBottomLine() - 2));
            } else {
                v.getY() = Utils::getPosition (Math::max (linespace, Utils::getTopLine() + 2));
            }
        }
        
        return Affine::translation (v);
    }
    
private:
    Box beam_;
    mica::Concept direction_;
    unicode glyph_;

private:
    PRIM_LEAK_DETECTOR (Numeral)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Bracket {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Bracket (const Array<Box>& stems, const Array<mica::Concept>& directions, const Polygon& h, int n)
    {
        stems_      = stems;
        directions_ = directions;
        hull_       = h;
        angle_      = 0.0;
        unit_       = House::kDensityUnit;
        weight_     = 0.0;
        position_   = mica::Above;
        
        if (Math::isBetween (n, 1, 9)) { glyph_ = 0x0030 + n; }
        else {
            glyph_ = 0x0000;
        }
    }

public:
    Bracket (const Bracket&) = default;
    Bracket (Bracket&&) = default;
    Bracket& operator = (const Bracket&) = default;
    Bracket& operator = (Bracket&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Bracket& setAngle (double angle)
    {
        angle_ = angle;
        angle_ = Math::min (angle_, House::kTupletBracketAngleMaximum);
        angle_ = Math::max (angle_, -House::kTupletBracketAngleMaximum);
         
        return *this;
    }
    
    Bracket& setPosition (mica::Concept position)
    {
        position_ = position; return *this;
    }
    
    Bracket& setUnit (double unit)
    {
        unit_ = unit; return *this;         /* Spacing unit. */
    }
    
    Bracket& setWeight (double weight)      /* Spacing weight (cumulative) for the entire tuplet. */
    {
        weight_ = weight; return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void addToPath (Path& path) const
    {
        if ((stems_.size() >= 2) && (stems_.size() == directions_.size())) {
        //
        Path bracket (getBracket());
        Affine affine (getPlacement (bracket));
        path.addPath (bracket, affine);
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Path getBracket() const
    { 
        Point p0, p1, p2, p3, p4, p5, middle;
        
        p1 = getFirst();
        p4 = getSecond();
        
        double magnitude = (p4.getX() - p1.getX()) / std::cos (angle_);
        p4 = p1 + Vector::withPolar (angle_, Math::max (magnitude, getMagnitude()));
        
        middle = (p1 + p4) / 2.0;
        
        Path number (House::getInstance().getGentium().getGlyph (glyph_)->getPath(), House::kAffineGentium);
        number.apply (Affine::translation (middle - Vector (number.getBounds().getCenter())));
        
        if (position_ != mica::Below) {
            p0 = p1 - Vector (0.0, House::kTupletBracketHook);
            p5 = p4 - Vector (0.0, House::kTupletBracketHook);
            
        } else {
            p0 = p1 + Vector (0.0, House::kTupletBracketHook);
            p5 = p4 + Vector (0.0, House::kTupletBracketHook);
        }
        
        Line baseline (p1, p4);
        baseline.intersectionWithVertical (number.getBounds().getLeft() - House::kTupletBracketExtra, p2);
        baseline.intersectionWithVertical (number.getBounds().getRight() + House::kTupletBracketExtra, p3);
        
        Path path;
        
        path.addPath (number);
        path.addLine (p0, p1, House::kTupletBracketThickness);
        path.addLine (p1, p2, House::kTupletBracketThickness);
        path.addLine (p3, p4, House::kTupletBracketThickness);
        path.addLine (p4, p5, House::kTupletBracketThickness);
        
        return path;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Affine getPlacement (const Path& bracket) const
    {
        Path hull;
        hull.addPolygon (hull_);
        
        Vector v;
        
        /* Non colliding distance with the material of the chords. */
        
        if (position_ != mica::Below) {
            double d = Optics::nonCollidingVerticalByOutlines (bracket, hull) + House::kTupletBracketDistance;
            v = Vector (0.0, d);

        } else {
            double d = Optics::nonCollidingVerticalByOutlines (hull, bracket) + House::kTupletBracketDistance;
            v = Vector (0.0, -d);
        }
        
        /* Avoid collision with staff lines. */
        
        int linespace = Utils::getLinespace ((bracket.getBounds().getCenter() + v).getY());
        
        if (Utils::isOverLines (linespace)) {
            if (position_ != mica::Below) { 
                v += Vector (0.0, (Utils::getTopLine() + 1 - linespace) * House::kStaffSpaceWidth);
            } else {
                v += Vector (0.0, (Utils::getBottomLine() - 1 - linespace) * House::kStaffSpaceWidth);
            }
        }
        
        return Affine::translation (v);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Point getFirst() const
    {
        Point pt;
        
        if (position_ != mica::Below) { pt = stems_.getFirst().getTopLeft(); } 
        else {
            pt = stems_.getFirst().getBottomLeft();
        }
    
        if (directions_.getFirst() == mica::Up) { pt -= Vector (House::kNoteHeadWidth, 0.0); }
        
        return pt;
    }
    
    Point getSecond() const
    {
        Point pt;
        
        if (position_ != mica::Below) { pt = stems_.getLast().getTopRight(); } 
        else {
            pt = stems_.getLast().getBottomRight();
        }
        
        if (directions_.getLast() == mica::Down) { pt += Vector (House::kNoteHeadWidth, 0.0); }
        
        return pt;
    }
    
    double getMagnitude() const     /* To reserve empty space in case of mixed tuplet. */
    {
        if (weight_ > 0.0) { return (weight_ * unit_) / std::cos (angle_); }
        else {
            return 0.0;
        }
    }
    
private:
    Array<Box> stems_;
    Array<mica::Concept> directions_;
    Polygon hull_;
    double angle_;
    double unit_;
    double weight_;
    mica::Concept position_;
    unicode glyph_;

private:
    PRIM_LEAK_DETECTOR (Bracket)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
