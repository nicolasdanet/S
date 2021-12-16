
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Cosmetic adjustements for symbols. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Adjust {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void rests (Music& music)
    {
        Array<NodePtr> tokens (music.getGroupState().reclaimTokens());
        
        /* Lonely semibreve must be at the middle place between barlines. */
        /* Avoid collision between rests and beams. */
        
        if (tokens.size() == 2)     { semibreve (music, tokens); } 
        else if (tokens.size() > 1) { collision (music, tokens); }
        
        Pointer<Stamp> stamp = music.getIsland (tokens.getLast())->getObject().getStamp();
        music.getGroupState().setBox (mica::Barline, stamp->getBounds (stamp->getContext()));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static void semibreve (Music& music, const Array<NodePtr>& tokens)
    {
        if (tokens.getFirst()->getObject().getAttribute (mica::Size).getNumerator() == 0) {
        //
        if (tokens.getFirst()->getObject().getAttribute (mica::Value).toRatio() == Ratio (1, 1)) {
        //
        const Box barline (music.getGroupState().getBox (mica::Barline));
        
        if (!barline.isEmpty()) {
        //
        Pointer<Stamp> stamp1 = music.getIsland (tokens.getFirst())->getObject().getStamp();
        Pointer<Stamp> stamp2 = music.getIsland (tokens.getLast())->getObject().getStamp();
        Box box1 = stamp1->getBounds (stamp1->getContext());
        Box box2 = stamp2->getBounds (stamp2->getContext());
        double x1 = (box1.getCenter().getX() - barline.getLeft());
        double x2 = (box2.getCenter().getX() - barline.getLeft()) / 2.0;
        stamp1->addContext (Affine::translation (Vector (x2 - x1, 0.0)));
        //
        }
        //
        }
        //
        }
    }
    
    static void collision (Music& music, const Array<NodePtr>& tokens)
    {
        bool beamed = false;
        
        Array<NodePtr> rests;
        
        for (int i = 0; i < tokens.size() - 1; ++i) {
        //
        bool isRest = (tokens[i]->getObject().getAttribute (mica::Size).getNumerator() == 0);
        
        if (isRest && beamed) { rests.add (tokens[i]); }
        else {
            mica::Concept beam = tokens[i]->getObject().getAttribute (mica::Beam);
            beamed = ((beam == mica::Beginning) || (beam == mica::Middle));
            if (!beamed && rests.size()) {
                Pointer<Stamp> stamp = music.getIsland (tokens[i])->getObject().getStamp();
                Box bounds = stamp->getBox (mica::Beam, stamp->getContext());
                mica::Concept direction = tokens[i]->getObject().getAttribute (mica::Stem);
                arrange (music, rests, bounds, direction);
                rests.clear();
            }
        }
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static void arrange (Music& music, const Array<NodePtr>& rests, Box bounds, mica::Concept direction)
    {
        bool below = (direction == mica::Up);
        
        if (!bounds.isEmpty()) {
        //
        Path collision;
        collision.addRectangle (bounds);
         
        for (int i = 0; i < rests.size(); ++i) {
        //
        Pointer<Stamp> stamp = music.getIsland (rests[i])->getObject().getStamp();
        Path rest (stamp->getPath (stamp->getContext()));
        
        double d = 0.0;
        
        if (below) { 
            d = (collision.getBounds().getBottom() - House::kAdjustMinimum - rest.getBounds().getTop());
        } else {
            d = (rest.getBounds().getBottom() - House::kAdjustMinimum - collision.getBounds().getTop());
        }
        
        if (d < 0.0) {
        //
        int y = static_cast<int> ((d / House::kStaffSpaceWidth) - 1.0);
        
        if (below) { stamp->addContext (Affine::translation (Vector (0.0, y * House::kStaffSpaceWidth))); } 
        else {
            stamp->addContext (Affine::translation (Vector (0.0, -y * House::kStaffSpaceWidth)));
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

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
