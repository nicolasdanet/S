
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Note_head > */
/* < http://faculty.washington.edu/garmar/notehead_specs.pdf > */
/* < https://en.wikipedia.org/wiki/Stem_%28music%29 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Note {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    Note() : hasBeam_ (false)
    {
    }

public:
    Note (const Note&) = default;
    Note (Note&&) = default;
    Note& operator = (const Note&) = default;
    Note& operator = (Note&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Note& hasBeam (bool beamed)
    {
        hasBeam_ = beamed; return *this;
    }
    
    Note& setHeight (double height)
    {
        stemHeight_ = height; return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addToPath (Path& p) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    enum Type { Whole = 0, Half, Quarter };
    
protected:
    Type type_;
    bool hasBeam_;
    bool hasHollow_;
    double stemHeight_;
    double hollowAngle_;
    double hollowScale_;

private:
    PRIM_LEAK_DETECTOR (Note)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct WholeNote : public Note {

public:
    WholeNote()
    {
        type_        = Note::Whole;
        hasHollow_   = true;
        stemHeight_  = 0.0;
        hollowAngle_ = House::kNoteHollowAngleWhole;
        hollowScale_ = House::kNoteHollowScaleWhole;
    }
};

struct HalfNote : public Note {

public:
    HalfNote() 
    {      
        type_        = Note::Half;
        hasHollow_   = true;
        stemHeight_  = House::kNoteStemHeight;
        hollowAngle_ = House::kNoteHollowAngle;
        hollowScale_ = House::kNoteHollowScale;
    }
};

struct QuarterNote : public Note {

public:
    QuarterNote() 
    {
        type_        = Note::Quarter;
        hasHollow_   = false;
        stemHeight_  = House::kNoteStemHeight;     
        hollowAngle_ = House::kNoteHollowAngle;
        hollowScale_ = House::kNoteHollowScale;
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

void Shapes::Note::addToPath (Path& path) const
{
    const Point origin_;

    double stemW = House::kNoteStemThickness;
    double stemH = House::kNoteHeadHeight * stemHeight_;
    double headW = House::kNoteHeadWidth;
    double headH = House::kNoteHeadHeight;
    
    Vector headVector (headW, headH);
    
    if (hasHollow_) {
    //
    Vector hollowVector = headVector * hollowScale_;
    
    if (type_ == Note::Whole) { path.addEllipse (origin_, hollowVector, hollowAngle_, false); }
    else {
        Point a = Point::withPolar (hollowAngle_ + kPi, hollowVector.getX() / 2.0);
        Point b = Point::withPolar (hollowAngle_, hollowVector.getX() / 2.0);
        
        a += origin_;
        b += origin_;
        
        path.addLine (a, b, hollowVector.getY(), true, false);
    }
    //
    }

    if (Math::abs (stemH) < (headH / 2.0)) { 
    //
    path.addEllipse (origin_, headVector, House::kNoteHeadAngle); 
    //
    } else {
    //
    Bezier b0, b1, b2, b3, b5, b6;
    Point l4Start, l4End, l7Start, l7End;
    
    double t0, t3;
    
    t0 = Bezier::parameterFromVerticalTangentInFirstQuadrant (headVector, House::kNoteHeadAngle, stemW);
    t3 = Bezier::parameterOfVerticalTangentInFourthQuadrant (headVector, House::kNoteHeadAngle);

    if (stemH > 0.0) {
    //
    b0 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle, 1);
    b0.trim (t0, 1.0);
    b1 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle, 2);
    b2 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle, 3);
    b3 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle, 4);
    b3.trim (0.0, t3);

    l4Start = b3.get (1.0);
    l4End = l4Start;
    l4End.getY() = origin_.getY() + stemH;

    Point capOrigin (l4End.getX() - (stemW / 2.0), l4End.getY());
    Vector capVector (stemW, stemW * House::kNoteStemCapRelative);
    
    b5 = Bezier::makeEllipticalArc (capOrigin, capVector, 0.0, 1);
    b6 = Bezier::makeEllipticalArc (capOrigin, capVector, 0.0, 2);
    
    l7Start = l4End;
    l7Start.getX() -= stemW;
    l7End = b0.get (0.0);
    //
    } else {
    //
    b0 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle + kPi, 1);
    b0.trim (t0, 1.0);
    b1 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle + kPi, 2);
    b2 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle + kPi, 3);
    b3 = Bezier::makeEllipticalArc (origin_, headVector, House::kNoteHeadAngle + kPi, 4);
    b3.trim (0.0, t3);
    
    l4Start = b3.get (1.0);
    l4End = l4Start;
    l4End.getY() = origin_.getY() + stemH;

    Vector capOrigin (l4End.getX() + stemW / 2.0, l4End.getY());
    Vector capVector (stemW, stemW * House::kNoteStemCapRelative);
    
    b5 = Bezier::makeEllipticalArc (capOrigin, capVector, 0.0, 3);
    b6 = Bezier::makeEllipticalArc (capOrigin, capVector, 0.0, 4);

    l7Start = l4End;
    l7Start.getX() += stemW;
    l7End = b0.get (0.0);
    //
    }
    
    path.add (Instruction::moveTo (l7End));
    path.add (Instruction::withBezier (b0));
    path.add (Instruction::withBezier (b1));
    path.add (Instruction::withBezier (b2));
    path.add (Instruction::withBezier (b3));
    
    if (hasBeam_) { path.add (Instruction::closePath()); }      /* Stem will be drawn as part of the beam. */
    else {
    //
    path.add (Instruction::lineTo (l4End));
    path.add (Instruction::withBezier (b5));
    path.add (Instruction::withBezier (b6));
    path.add (Instruction::lineTo (l7End));
    path.add (Instruction::closePath());
    //
    }
    
    path.getBox (mica::Stem) = Box (l4Start, l7Start);       /* The stem without the cap. */
    
    if (hasBeam_) {
    //
    /* Invisible stem for the spacing algorithm. */
    
    path.add (Instruction::moveTo ((l7End + l4Start) / 2.0));
    path.add (Instruction::lineTo ((l7Start + l4End) / 2.0));

    /* In case of large intervals beamed. */
    
    Box box (l4Start, l7Start);

    if (stemH > 0.0) { box.setTop (box.getTop() + stemH); }
    else {
        box.setBottom (box.getBottom() - stemH);
    }
    
    path.getBox (mica::Extra) += box;
    //
    }
    //
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
