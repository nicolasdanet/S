
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Spacing the score. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Spacing {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static bool aligned (const Array<int>& state)
{
    bool b = true;
    for (int i = 1; i < state.size(); ++i) { if (state[i] != state[i - 1]) { b = false; break; } }
    return b;
}

static void getPath (const Array<Instant>& instants, const Array<int>& state, int& last, Path& path)
{
    double f = 0.0;
    
    /* Get instant at the right-most position. */
    
    for (int i = 0; i < state.size(); ++i) {
        double t = instants[state[i]].getPosition();
        if (t > f) { last = state[i]; f = t; }
    }
    
    /* All symbols aligned on that position. */
    
    Path scoped;
    
    for (int i = 0; i < state.size(); ++i) {
        double d = instants[state[i]].getPosition() - instants[last].getPosition();
        Path t (instants[state[i]].getSpacing (i));
        scoped.addPath (t, Affine::translation (Vector (d, 0.0)));
    }
    
    scoped.swapWith (path);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Music::spacingVertical()
{
    for (int i = 0; i < parts_.size(); ++i) { 
        parts_[i].setPosition ((parts_.size() - 1 - i) * (House::kStaffHeight + House::kStaffDistance)); 
    }

    /* Adjust the context. */
    
    for (int i = 0; i < instants_.size(); ++i) {
        for (int j = 0; j < parts_.size(); ++j) {
            if (Pointer<Stamp> stamp = instants_[i].getStampAtPart (j)) {
                stamp->addContext (Affine::translation (Vector (0.0, parts_[j].getPosition())));
            }
        }
    }
}

void Music::spacingHorizontal()
{
    Array<int> state;            /* Contains the last instant position for each part. */
    state.resize (parts_.size());   
    
    /* Initialize with the first instant. */ 
    
    for (int i = 0; i < parts_.size(); ++i) { state[i] = 0; }
    instants_[0].setPosition (0.0);
    
    /* Arrange instants from left to right. */
    
    Path current;
    Path previous (instants_[0].getSpacing());
    
    for (int i = 1; i < instants_.size(); ++i) { 
    //
    double weight, minimum, distance;
    
    current = instants_[i].getSpacing();
    
    /* Partwise only. */
    
    if (instants_[i].isUnique()) {
        int part = instants_[i].getUnique();
        int last = state[part];
        previous = instants_[last].getSpacing (part);
        minimum  = Optics::nonCollidingHorizontalByHull (previous, current) + House::kSpacingMinimum;
        weight   = instants_[last].getWeight (part) * instants_[last].getUnit();
        distance = instants_[last].getPosition() + Math::max (minimum, weight);
        state[part] = i;
    
    /* Instantwise with resync alignment. */ 
    
    } else {
        int last = i - 1;
        if (Spacing::aligned (state) == false) { Spacing::getPath (instants_, state, last, previous); }
        minimum  = Optics::nonCollidingHorizontalByHull (previous, current) + House::kSpacingMinimum;
        weight   = instants_[last].getWeight() * instants_[last].getUnit();
        distance = instants_[last].getPosition() + Math::max (minimum, weight);
        for (int j = 0; j < parts_.size(); ++j) { state[j] = i; }
    }
    
    instants_[i].setPosition (distance);
    previous.swapWith (current);
    //
    }
    
    /* Adjust the context. */
    
    for (int i = 0; i < instants_.size(); ++i) {
        for (int j = 0; j < parts_.size(); ++j) {
            if (Pointer<Stamp> stamp = instants_[i].getStampAtPart (j)) {
                stamp->addContext (Affine::translation (Vector (instants_[i].getPosition(), 0.0)));
            }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Music::spacingEnlarge()
{
    for (int i = 0; i < instants_.size(); ++i) {
        for (int j = 0; j < parts_.size(); ++j) {
            if (Pointer<Stamp> stamp = instants_[i].getStampAtPart (j)) {
                parts_[stamp->getPart()].addSpacing (stamp->getPath (stamp->getContext()));
            }
        }
    }
    
    for (int i = parts_.size() - 1; i > 0; --i) { 
    //
    double y1 = parts_[i].getSpacing().getBounds().getTop();
    double y2 = parts_[i - 1].getSpacing().getBounds().getBottom();
    
    if (y1 - y2 > 0.0) {
        double d = Optics::nonCollidingVerticalByHull (parts_[i - 1].getSpacing(), parts_[i].getSpacing());
        parts_[i - 1].setExtra (d + House::kAdjustMinimum);
    }
    //
    }
    
    double sum = 0.0;
    
    for (int i = parts_.size() - 2; i >= 0; --i) { 
    //
    sum += parts_[i].getExtra();
        
    if (sum > 0.0) {
    //
    Affine affine = Affine::translation (Vector (0.0, sum));
    
    parts_[i].setPosition (parts_[i].getPosition() + sum);
    parts_[i].setSpacing (Path (parts_[i].getSpacing(), affine));
    
    for (int j = 0; j < instants_.size(); ++j) {
        if (Pointer<Stamp> stamp = instants_[j].getStampAtPart (i)) { stamp->addContext (affine); }
    }
    //
    }
    //
    }
    
    return (sum > 0.0);
}

void Music::spacingBounds()
{
    Point origin (Affine::inversed (Affine::scale (scale_)).appliedTo (origin_));
    
    double y = origin.getY() - (House::kStaffDistance + House::kStaffHeight);
    
    bounds_ = Box::empty();
    
    bounds_ += instants_.getLast().getBounds();
    bounds_ += Point (0.0, origin.getY());
    bounds_ += Point (-origin.getX(), 0.0);
    bounds_ += Point (0.0, -y);
    
    bounds_.setRight (bounds_.getRight() + origin.getX());
    bounds_.setTop (bounds_.getTop() - House::kMarginMinimum);
    
    for (int i = 0; i < parts_.size(); ++i) { bounds_ += parts_[i].getSpacing().getBounds(); }
    
    bounds_.setTop (bounds_.getTop() + House::kMarginMinimum);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
