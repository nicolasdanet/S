
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A Path with indexed Concepts and Boxes. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Path {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Path()
    {
    }
    
    template <class T> explicit Path (const T& shape)
    {
        Path scoped;
        shape.addToPath (scoped);
        scoped.swapWith (*this);
    }
    
    Path (const Path& o, const Affine& affine)
    {
        (*this).addPath (o, affine);
    }
    
    Path& operator = (const Path& o)
    {
        if (this != &o) {
            Path scoped (o); scoped.swapWith (*this);
        }
        
        return *this;
    }

public:
    Path (const Path&) = default;
    Path (Path&&) = default;
    Path& operator = (Path&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void swapWith (Path& o)
    {
        using std::swap;
        
        instructions_.swapWith (o.instructions_);
        outlines_.swapWith (o.outlines_);
        boxes_.swapWith (o.boxes_);
        properties_.swapWith (o.properties_);
    
        swap (bounds_, o.bounds_);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isEmpty() const
    {
        return (size() == 0);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void clear()
    {
        Path scoped; scoped.swapWith (*this);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
public:
    void apply (const Affine& affine)
    {
        Path scoped (*this, affine); scoped.swapWith (*this);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int size() const
    {
        return instructions_.size();
    }
    
    const Instruction& get (int i) const
    {
        return instructions_[i];
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A Path contains a Table of Boxes indexed by a MICA concept. */

public:
    Box& getBox (mica::Concept concept)
    {
        return boxes_.get (concept);
    }

    const Box& getBox (mica::Concept concept) const
    {
        return boxes_.get (concept);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A Path contains a Table of MICA concept properties. */

public:
    void setProperty (mica::Concept key, mica::Concept value)
    {
        properties_[key] = value;
    }
    
    mica::Concept getProperty (mica::Concept key) const
    {
        return properties_[key];
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void add (const Instruction& instruction)
    {
        instructions_.add (instruction);
        
        Point end = instruction.getEnd();
        Point cp1 = instruction.getControl1();
        Point cp2 = instruction.getControl2();
      
        if (instruction.isMoveTo()) {
            Polygon p; 
            p.add (end);
            outlines_.add (p);
            bounds_ = bounds_ + end;

        } else if (instruction.isLineTo()) {
            outlines_.getLast().add (end);
            bounds_ = bounds_ + end;
            
        } else if (instruction.isCubicTo()) {
            outlines_.getLast().add (cp1);
            outlines_.getLast().add (cp2);
            outlines_.getLast().add (end);
            bounds_ = bounds_ + cp1 + cp2 + end;    /* Quick approximation rather than perfect computation. */
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::string asPDFString() const
    {
        std::ostringstream s; s.setf (std::ios::fixed, std::ios::floatfield);
        
        for (int i = 0; i < size(); ++i) { s << get (i).asPDFString(); }
        
        return s.str();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addPath (const Path& p, const Affine& affine = Affine::identity())
    {
        for (int i = 0; i < p.instructions_.size(); ++i) { 
            add (Instruction::withAffine (p.instructions_[i], affine)); 
        }
        
        for (int i = 0; i < p.boxes_.size(); ++i) { 
            boxes_[p.boxes_.getAtIndex (i).getKey()] = affine.appliedTo (p.boxes_.getAtIndex (i).getValue()); 
        }
        
        for (int i = 0; i < p.properties_.size(); ++i) { 
            properties_[p.properties_.getAtIndex (i).getKey()] = p.properties_.getAtIndex (i).getValue(); 
        }
    }

    void addRectangle (const Box& b, const Affine& affine = Affine::identity())
    {
        if (!b.isEmpty()) {
        //
        add (Instruction::moveTo (affine.appliedTo (b.getBottomLeft())));
        add (Instruction::lineTo (affine.appliedTo (b.getBottomRight())));
        add (Instruction::lineTo (affine.appliedTo (b.getTopRight())));
        add (Instruction::lineTo (affine.appliedTo (b.getTopLeft())));
        add (Instruction::closePath());
        //
        }
    }
    
    void addPolygon (const Polygon& p, const Affine& affine = Affine::identity())
    {
        if (p.size() > 2) {
        //
        add (Instruction::moveTo (affine.appliedTo (p.get (0))));
        for (int i = 1; i < p.size(); ++i) { add (Instruction::lineTo (affine.appliedTo (p.get (i)))); }
        add (Instruction::closePath());
        //
        }
    }
    
    template <class T> void addShape (const T& shape, const Affine& affine = Affine::identity())
    {
        Path temp;
        shape.addToPath (temp);
        addPath (temp, affine);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void addEllipse (Point origin, Vector scale, double rotation, bool ccw = true)
    {
        Bezier b0, b1, b2, b3;
        Point  p0, p1, p2, p3;
        
        if (ccw) {
        //
        b0 = Bezier::makeEllipticalArc (origin, scale, rotation, 1);
        b1 = Bezier::makeEllipticalArc (origin, scale, rotation, 2);
        b2 = Bezier::makeEllipticalArc (origin, scale, rotation, 3);
        b3 = Bezier::makeEllipticalArc (origin, scale, rotation, 4);
        
        b0.getControlPoints (p0, p1, p2, p3);
            
        add (Instruction::moveTo (p0));
        add (Instruction::withBezier (b0));
        add (Instruction::withBezier (b1));
        add (Instruction::withBezier (b2));
        add (Instruction::withBezier (b3));
        //
        } else {
        //
        b0 = Bezier::reversed (Bezier::makeEllipticalArc (origin, scale, rotation, 1));
        b1 = Bezier::reversed (Bezier::makeEllipticalArc (origin, scale, rotation, 2));
        b2 = Bezier::reversed (Bezier::makeEllipticalArc (origin, scale, rotation, 3));
        b3 = Bezier::reversed (Bezier::makeEllipticalArc (origin, scale, rotation, 4));
        
        b3.getControlPoints (p0, p1, p2, p3);
            
        add (Instruction::moveTo (p0));
        add (Instruction::withBezier (b3));
        add (Instruction::withBezier (b2));
        add (Instruction::withBezier (b1));
        add (Instruction::withBezier (b0));
        //
        }
        
        add (Instruction::closePath());
    }

    void addCircle (Point origin, double diameter, bool ccw = true)
    {
        addEllipse (origin, Vector (diameter, diameter), 0.0, ccw);
    }

    void addLine (Point a, Point b, double width, bool isCap = true, bool ccw = true)
    {
        double angle = (b - a).angle();

        Point p0 = a + Vector::withPolar (angle - kHalfPi, width / 2.0);
        Point p1 = b + Vector::withPolar (angle - kHalfPi, width / 2.0);
        Point p2 = b - Vector::withPolar (angle - kHalfPi, width / 2.0);
        Point p3 = a - Vector::withPolar (angle - kHalfPi, width / 2.0);

        add (Instruction::moveTo (p0));

        if (isCap == false) {
        //
        if (ccw) {
            add (Instruction::lineTo (p1));
            add (Instruction::lineTo (p2));
            add (Instruction::lineTo (p3));
              
        } else {
            add (Instruction::lineTo (p3));
            add (Instruction::lineTo (p2));
            add (Instruction::lineTo (p1));
        }
        //
        } else {
        //
        Bezier e1, e2, s1, s2;
        
        if (ccw) {
        //
        e1 = Bezier::makeEllipticalArc (b, Vector (width, width), angle - kHalfPi, 1);
        e2 = Bezier::makeEllipticalArc (b, Vector (width, width), angle - kHalfPi, 2);
        s1 = Bezier::makeEllipticalArc (a, Vector (width, width), angle - kHalfPi, 3);
        s2 = Bezier::makeEllipticalArc (a, Vector (width, width), angle - kHalfPi, 4);
        
        add (Instruction::lineTo (p1));
        add (Instruction::withBezier (e1));
        add (Instruction::withBezier (e2));
        add (Instruction::lineTo (p3));
        add (Instruction::withBezier (s1));
        add (Instruction::withBezier (s2));
        //
        } else {
        //
        e1 = Bezier::reversed (Bezier::makeEllipticalArc (b, Vector (width, width), angle - kHalfPi, 1));
        e2 = Bezier::reversed (Bezier::makeEllipticalArc (b, Vector (width, width), angle - kHalfPi, 2));
        s1 = Bezier::reversed (Bezier::makeEllipticalArc (a, Vector (width, width), angle - kHalfPi, 3));
        s2 = Bezier::reversed (Bezier::makeEllipticalArc (a, Vector (width, width), angle - kHalfPi, 4));
        
        add (Instruction::withBezier (s2));
        add (Instruction::withBezier (s1));
        add (Instruction::lineTo (p2));
        add (Instruction::withBezier (e2));
        add (Instruction::withBezier (e1));
        //
        }
        //
        }
        
        add (Instruction::closePath());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const Array<Polygon>& getOutlines() const
    {
        return outlines_;
    }
    
    Polygon getHull() const
    {
        Polygon polygon;
        
        for (int i = 0; i < outlines_.size(); ++i) {
            for (int j = 0; j < outlines_[i].size(); ++j) {     
                polygon.add (outlines_[i].get (j)); 
            }
        }
        
        polygon.convertToConvexHull();
        
        return polygon;
    }
    
    Box getBounds (const Affine& affine = Affine::identity()) const
    {
        Box box;
        
        if (!bounds_.isEmpty()) {
        //
        box += affine.appliedTo (bounds_.getBottomLeft());
        box += affine.appliedTo (bounds_.getTopLeft());
        box += affine.appliedTo (bounds_.getTopRight());
        box += affine.appliedTo (bounds_.getBottomRight());
        //
        }
        
        return box;
    }

    const Array<Instruction>& getInstructions() const
    {
        return instructions_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Handy for debugging purpose. */

public:
    static Path asBorders (const Box& box, double width)
    {
        Path path;
        
        path.addLine (box.getTopRight(), box.getTopLeft(), width);
        path.addLine (box.getTopLeft(), box.getBottomLeft(), width);
        path.addLine (box.getBottomLeft(), box.getBottomRight(), width);
        path.addLine (box.getBottomRight(), box.getTopRight(), width);
        
        return path;
    }
    
    static Path asBorders (const Polygon& p, double width)
    {
        Path path;
        
        if (p.size() > 2) {
        //
        path.addLine (p.get (0), p.get (1), width);
        for (int i = 1; i < p.size() - 1; ++i) { path.addLine (p.get (i), p.get (i + 1), width); }
        path.addLine (p.get (p.size() - 1), p.get (0), width);
        //
        }
        
        return path;
    }
    
private:
    Array<Instruction> instructions_;
    Array<Polygon> outlines_;
    Table<mica::Concept, Box> boxes_;
    Table<mica::Concept> properties_;
    Box bounds_;

private:
    PRIM_LEAK_DETECTOR (Path)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void swap (Path& a, Path& b);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void swap (Path& a, Path& b)
{
    a.swapWith (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
