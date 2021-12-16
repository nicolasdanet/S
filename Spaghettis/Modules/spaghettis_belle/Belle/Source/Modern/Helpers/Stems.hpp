
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Utilities for group of stems. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Stems {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Array<int> getFlags (const Array<NodePtr>& chords)
{
    Array<int> flags;
    
    for (int i = 0; i < chords.size(); ++i) {
        flags.add (Utils::countFlags (chords[i]->getObject().getAttribute (mica::Value).toRatio()));
    }
    
    return flags;
}

static Array<mica::Concept> getDirections (const Array<NodePtr>& chords)
{
    Array<mica::Concept> directions;
    
    for (int i = 0; i < chords.size(); ++i) {
        directions.add (chords[i]->getObject().getAttribute (mica::Stem));
    }
    
    return directions;
}

/* Get stems bounds into another context (stamp bounds if no stem). */

static Array<Box> getBoxes (Music& music, const Array<NodePtr>& chords, const Affine& context)
{
    Array<Box> stems;
    
    for (int i = 0; i < chords.size(); ++i) { 
        NodePtr island = music.getIsland (chords[i]);
        Pointer<Stamp> stamp = island->getObject().getStamp();
        Box box = stamp->getBox (mica::Stem, stamp->getContext());
        if (box.isEmpty()) { box = stamp->getContext().appliedTo (stamp->getPath().getBounds()); }
        stems.add (Affine::inversed (context).appliedTo (box));
    }
    
    return stems;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
