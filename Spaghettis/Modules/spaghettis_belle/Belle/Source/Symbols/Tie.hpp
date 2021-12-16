
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Tie_%28music%29 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Tie {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Tie (Box start, Box end) : start_ (start), end_ (end), position_ (mica::Above), isChord_ (false)
    {
    }

public:
    Tie (const Tie&) = default;
    Tie (Tie&&) = default;
    Tie& operator = (const Tie&) = default;
    Tie& operator = (Tie&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Tie& setPosition (mica::Concept position)
    {
        position_ = position; return *this;
    }
    
    Tie& isChord (bool chord)
    {
        isChord_ = chord; return *this;
    }
    
    Tie& setDirections (mica::Concept start, mica::Concept end)
    {
        directionStart_ = start; directionEnd_ = end; return *this;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Tie& setPathAtStart (const Path& path)      /* Set to place ties if more precision needed. */
    {
        pathStart_ = path; return *this;
    }
    
    Tie& setPathAtEnd (const Path& path)
    {
        pathEnd_ = path; return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addToPath (Path& path) const
    {
        Box box (getFirst(), getSecond());

        if (box.getWidth() > House::kTieAdjustThresholdWidth) { 
        //
        if (position_ != mica::Below) { box.setTop (box.getTop() + House::kStaffSpaceWidth); }
        else {
            box.setBottom (box.getBottom() - House::kStaffSpaceWidth); 
        }
        //
        }
        
        if (!pathStart_.isEmpty() || !pathEnd_.isEmpty()) { adjustTie (box); }
        
        path.addPath (getTie (box));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Point getFirst() const
    {
        Point pt (start_.getBottomRight());
        
        /* Y adjust. */
        
        int linespace = Utils::getLinespace (pt.getY());
        
        if (linespace > 0) { 
            pt.getY() = Utils::getPosition ((linespace % 2) ? linespace + 1 : linespace);
        } else {
            pt.getY() = Utils::getPosition ((linespace % 2) ? linespace - 1 : linespace);
        }
       
        Vector v (0.0, House::kNoteHeadHeight / 2.0);
        
        if (isChord_ || (linespace % 2)) { v = Vector (0.0, House::kNoteHeadHeight / 4.0); }
        if (position_ != mica::Below) { pt += v; }
        else {
            pt -= v;
        }
        
        /* X adjust. */
        
        /* Adjust optical illusion due to stems. */
        
        if (directionStart_ == mica::Up) { pt += Vector (House::kTieOptical, 0.0); }
                
        return pt;
    }
    
    Point getSecond() const
    {
        Point pt (end_.getBottomLeft());
        
        /* Y adjust. */
        
        int linespace = Utils::getLinespace (pt.getY());
        
        if (position_ == mica::Below) {
            pt.getY() = Utils::getPosition ((linespace % 2) ? linespace - 3 : linespace - 2);
        } else {
            pt.getY() = Utils::getPosition ((linespace % 2) ? linespace + 3 : linespace + 2);
        }
        
        if (!isChord_ && (linespace % 2)) { 
            if (position_ == mica::Below) { pt += Vector (0.0, House::kNoteHeadHeight / 4.0); }
            else {
                pt -= Vector (0.0, House::kNoteHeadHeight / 4.0);
            }
        }
        
        /* X adjust. */
        
        /* Adjust optical illusion due to stems. */
        
        if (directionEnd_ == mica::Down) { pt -= Vector (House::kTieOptical, 0.0); }
        
        return pt;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void adjustTie (Box& box) const
    {
        if (pathStart_.isEmpty() == false) {
        //
        Vector v (-start_.getWidth() / 2.0, 0.0);
        Path p (getDraft (box), Affine::translation (v));
        double d = Optics::nonCollidingHorizontalByOutlines (pathStart_, p);
        box.setLeft (box.getLeft() + v.getX() + (d + House::kTieAdjustMinimum));
        //
        }
        
        if (pathEnd_.isEmpty() == false) {
        //
        Vector v (end_.getWidth() / 2.0, 0.0);
        Path p (getDraft (box), Affine::translation (v));
        double d = Optics::nonCollidingHorizontalByOutlines (p, pathEnd_);
        box.setRight (box.getRight() + v.getX() - (d + House::kTieAdjustMinimum));
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Path getTie (Box& box) const
    {
        double curve = 0.5;
        double h = box.getHeight();
        
        Point a, b;
        
        if (position_ != mica::Below) { a = box.getBottomLeft(); }
        else {
            a = box.getTopLeft();
        }
        
        Bezier c1 = Bezier::makeConvexCurveInBox (box, curve, (position_ != mica::Below));
        
        if (position_ != mica::Below) { 
            box.setTop (box.getTop() - (h * House::kTieHeight)); 
            box.setBottom (box.getBottom() - (h * House::kTieThickness));
            b = box.getBottomRight();
            
        } else {
            box.setBottom (box.getBottom() + (h * House::kTieHeight));
            box.setTop (box.getTop() + (h * House::kTieThickness));
            b = box.getTopRight();
        }
        
        Bezier c2 = Bezier::reversed (Bezier::makeConvexCurveInBox (box, curve, (position_ != mica::Below)));
        
        Path path;
        path.add (Instruction::moveTo (a));
        path.add (Instruction::withBezier (c1));
        path.add (Instruction::lineTo (b));
        path.add (Instruction::withBezier (c2));
        path.add (Instruction::closePath());
        return path;
    }
    
    Path getDraft (Box& box) const                          /* Rough approximation to test for collision. */
    {
        Polygon p;
        
        if (position_ != mica::Below) {
            p.add (box.getBottomLeft());
            p.add (box.getBottomRight());
            p.add ((box.getTopLeft() + box.getTopRight()) / 2.0);
            
        } else {
            p.add ((box.getBottomLeft() + box.getBottomRight()) / 2.0);
            p.add (box.getTopRight());
            p.add (box.getTopLeft());
        }
        
        Path path;
        path.addPolygon (p);
        return path;
    }
    
private:
    Box start_;
    Box end_;
    mica::Concept position_;
    bool isChord_;
    mica::Concept directionStart_;
    mica::Concept directionEnd_;
    Path pathStart_;                    /* For precise adjustments. */
    Path pathEnd_;

private:
    PRIM_LEAK_DETECTOR (Tie)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
