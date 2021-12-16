
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that pageable painter is currently not supported. */
/* In that case line breaking algorithm should be implemented. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Music::start (Painter&)
{
    if (!graph_.size())       { return false; }
    else if (dirty_ == false) { return true;  }     /* Typesetting already done. */
    else {
    //
    PRIM_ASSERT (!graph_.isCyclic());
    
    /* Take care with the order of those function calls. */
    
    proceed (Typesetters::Allocate());              /* Allocate the stamps. */
    proceed (Typesetters::Parts());                 /* Determine the part of stamps. */
    proceed (Typesetters::Instants());              /* Determine the instant of stamps. */
    
    if (instants_.size()) {
    //
    proceed (Typesetters::Pack());                  /* Group stamps by instants. */
    proceed (Typesetters::Spacing());               /* Set spacing according to density of events. */
    proceed (Typesetters::Stems());                 /* Determine the stem direction of beamed chords. */
    proceed (Typesetters::Engrave());               /* Engrave the symbols (most of) on stamps. */
    proceed (Typesetters::Flags());                 /* Engrave the flags on stamps. */
    
    spacingVertical();                              /* Typeset stamps for Y axis. */
    spacingHorizontal();                            /* Typeset stamps for X axis. */
    
    proceed (Typesetters::Beams());                 /* Engrave the beams. */
    proceed (Typesetters::Ties());                  /* Engrave the ties. */
    
    /* For now, only one level of nested tuplets is supported. */
    
    proceed (Typesetters::Tuplets::inner());        /* Engrave inner tuplets. */
    proceed (Typesetters::Tuplets::outer());        /* Engrave outer tuplets. */

    proceed (Typesetters::Rest());                  /* Typeset the rests. */
    
    if (spacingEnlarge()) { proceed (Typesetters::Enlarge()); }     /* Avoid collision between parts. */
    
    spacingBounds();                                                /* Compute the global bounds. */
    
    return true;
    //
    }
    //
    }
    
    return false;
}

void Music::paint (Painter& painter)
{
    paintStart (painter);       /* Push the global context. */
    paintLines (painter);       /* Draw the staff lines. */
    paintBrace (painter);       /* Draw the brace. */
    paintAll (painter);         /* Draw the symbols. */
    paintEnd (painter);         /* Pop the global context. */
}

void Music::end (Painter&)
{
    if (dirty_) { clean(); dirty_ = false; }        /* Release useless materials. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int Music::getNumberOfPages (Painter&)
{
    return 1;   /* Not implemented yet. */
}

Points Music::getRequiredSize (Painter& painter)
{
    if (painter.isPageable()) { return painter.getPageSize(); }     /* Not implemented yet. */
    
    Box box = Affine::scale (scale_).appliedTo (bounds_);
    
    return Points (box.getWidth(), box.getHeight());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Music::paintStart (Painter& painter)
{
    Affine unit (Affine::scale (scale_));
    Affine origin, flip, offset;
    
    if (painter.isPageable()) {
        double w = origin_.width();
        double h = painter.getPageSize().height() - origin_.height();
        origin = Affine::translation (Vector (w, h));
        
    } else {
        origin = Affine::translation (origin_); flip = Affine::flipHorizontal();
    }
    
    /* Margin guard. */
    
    double y = bounds_.getTop() - Affine::inversed (unit).appliedTo (origin_).getY();
    
    if (y > 0) { offset = Affine::translation (Vector (0.0, -y)); }
    
    /* Push context. */
    
    painter.setState (Raster().setFillColor (House::kColorDefault));
    painter.pushAffine (origin * unit * offset * flip);
}

void Music::paintLines (Painter& painter)
{
    Box boxA (instants_.getFirst().getBounds());    /* Must be a barline. */
    Box boxB (instants_.getLast().getBounds());     /* Ditto. */
    
    if (!boxA.isEmpty() && !boxB.isEmpty()) {
    //
    Path staffline;
    Point a (boxA.getLeft() + (House::kStaffLineThickness / 2.0), 0.0);
    Point b (boxB.getRight() - (House::kStaffLineThickness / 2.0), 0.0);
    
    staffline.addLine (a, b, House::kStaffLineThickness, false);
        
    for (int i = 0; i < parts_.size(); ++i) {
        for (int j = -4; j <= 4; j += 2) {
            Vector v (0.0, parts_[i].getPosition() + Utils::getPosition (j));
            painter.draw (Path (staffline, Affine::translation (v)));
        }
    }
    //
    }
}

void Music::paintBrace (Painter& painter)
{
    if (parts_.size() == 2) {
        double y      = (parts_[0].getPosition() + parts_[1].getPosition()) / 2.0;
        double height = House::kBraceHeight + parts_[0].getExtra();
        double width  = House::kBraceWidthRelative;
        Vector v (-House::kBraceDistance, y);
        painter.draw (Path (Path (Shapes::SystemBrace (height, width)), Affine::translation (v)));
    }
}

void Music::paintAll (Painter& painter)
{
    for (int i = instants_.size() - 1; i >= 0; --i) {
    //
    for (int j = 0; j < parts_.size(); ++j) {
    //
    if (Pointer<Stamp> stamp = instants_[i].getStampAtPart (j)) { stamp->paint (painter); }
    //
    }
    //
    }
}

void Music::paintEnd (Painter& painter)
{
    painter.popAffine (1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Music::clean()
{
    getClefState().reset();
    getGroupState().reset();
    getChordState().reset();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
