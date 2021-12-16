
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

struct Tupletting {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static bool isMixed (const Array<NodePtr>& chords)
{
    if (chords.size() >= 2) {
    //
    mica::Concept previous = chords.getFirst()->getObject().getAttribute (mica::Value);
            
    for (int i = 1; i < chords.size(); ++i) {
        mica::Concept current = chords[i]->getObject().getAttribute (mica::Value);
        if (current != previous) { return true; }
        else { 
            previous = current;
        }
    }
    //
    }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Compute the angle according to the slant between the first and the last? */

static double getAngle (const Array<Box>& stems, Array<mica::Concept>& directions)
{
    double angle = 0.0;
    
    if (stems.size() == directions.size()) {
    //
    int i = 0;
    int j = directions.size() - 1;
    while ((i < j) && (directions[i] == mica::Undefined)) { i++; }      /* Avoid rests at start. */
    while ((j > 0) && (directions[j] == mica::Undefined)) { j--; }      /* Avoid rests at end. */
    
    Box b1 (stems[i]);
    Box b2 (stems[j]);
    
    if (!b1.isEmpty() && !b2.isEmpty()) {
    //
    Point a, b;
    
    /* Slant is computed according to the position of the heads. */
    
    if (directions.getFirst() != mica::Down) { a = b1.getBottomRight(); }
    else { a = b1.getTopRight(); }
    
    if (directions.getLast() != mica::Down)  { b = b2.getBottomLeft(); }
    else { b = b2.getTopLeft(); }
    
    Box box (a, b);
    angle = Line (box.getBottomLeft(), box.getTopRight()).angle();
    
    if (a.getY() > b.getY()) { angle = -angle; }
    //
    }
    //
    }
    
    return angle;
}

static mica::Concept getPosition (const Array<NodePtr>& chords, Array<mica::Concept>& directions)
{
    /* According to majority of stem directions? */
    
    int above = 0;
    int below = 0;
    
    for (int i = 0; i < directions.size(); ++i) {
        above += (directions[i] == mica::Up);
        below += (directions[i] == mica::Down);
    }
    
    /* Extra care to avoid collision between nested bracket and numeral. */
    
    bool allBeamed = true;
    
    for (int i = 0; i < chords.size(); ++i) {
        if (chords[i]->getObject().getAttribute (mica::Beam) == mica::Undefined) { allBeamed = false; break; }
    }
    
    if (below > above) { return (allBeamed ? mica::Above : mica::Below); }
    else {
        return (allBeamed ? mica::Below : mica::Above);
    }
}

/* The weight for mixed tuplet (e.g. with rest). */
/* Notice that it returns zero when the tuplet isn't mixed. */

static double getWeight (const Array<NodePtr>& chords) 
{
    double weight = 0.0;
    
    if (isMixed (chords)) {
        for (int i = 0; i < chords.size(); ++i) {
            weight += Utils::getWeight (getNoteValue (chords[i]));
        }
    }

    return weight;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static Ratio getNoteValue (NodePtr chord)
{
    Ratio value (chord->getObject().getAttribute (mica::Value).toRatio());
    
    int m = chord->getObject().getAttribute (mica::NestedTupletNumber).getNumerator();
    int n = chord->getObject().getAttribute (mica::TupletNumber).getNumerator();
    
    if (m > 0) {
    //
    if (m == 2) { value *= Ratio (3, 2); }
    else {
        value *= Ratio (2, n);
    }
    //
    }
    
    if (n > 0) {
    //
    if (n == 2) { value *= Ratio (3, 2); }
    else {
        value *= Ratio (2, n);
    }
    //
    }
    
    return value;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
