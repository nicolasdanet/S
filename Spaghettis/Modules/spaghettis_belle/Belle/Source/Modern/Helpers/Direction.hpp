
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Determining the stems direction. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Direction {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void setStems (Music& music)
    {
        Array<NodePtr> chords (music.getGroupState().reclaimTokens());
        
        if (chords.size()) {
        //
        mica::Concept direction = mica::Down;
        
        /* Define the default stem direction for each note. */
        
        Array<mica::Concept> base;
        
        for (int i = 0; i < chords.size(); ++i) { base.add (getDefault (music, chords[i])); }
        
        /* All directions are equaled or not? */
        
        bool equals = true;
        
        for (int i = 0; i < base.size() - 1; ++i) { if (base[i] != base[i + 1]) { equals = false; break; } }
        
        if (equals) { direction = base.getFirst(); }
        else { 
            direction = getMixed (music, chords, getMajority (base));
        }
        
        /* Set all the stems in the same direction for beaming. */
        
        for (int i = 0; i < chords.size(); ++i) { 
            chords[i]->getObject().setAttribute (mica::Stem, direction); 
        }
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static mica::Concept getDefault (Music& music, NodePtr chord) 
    {
        Array<int> linespaces;
        Array<NodePtr> notes = music.getAllTokensNext (chord);
        
        for (int i = 0; i < notes.size(); ++i) {
            mica::Concept chromatic = notes[i]->getObject().getAttribute (mica::Value);
            mica::Concept linespace = music.getClefState().getLinespace (chromatic);
            linespaces.add (linespace.getNumerator());
        }
        
        return Utils::getDirection (linespaces);
    }
    
    static mica::Concept getMixed (Music& music, const Array<NodePtr>& chords, mica::Concept majority)
    {
        /* Set the stems according to the majority if possible. */
        
        if (majority != mica::Undefined) { return majority; }   
        else {
        //
        /* Set the stems according to the furthest note from the middle line. */
        
        int furthest = 0;
        
        for (int i = 0; i < chords.size(); ++i) {
            int current = getFurthest (music, chords[i]);
            if (Math::abs (current) > Math::abs (furthest)) { furthest = current; }
            else if (current + furthest == 0) { furthest = 0; } 
        }
                
        if (furthest < 0)       { return mica::Up; }
        else if (furthest > 0)  { return mica::Down; }
        //
        }
        
        /* In case of equal distances up and down, stems down are preferred. */
        
        return mica::Down;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static mica::Concept getMajority (const Array<mica::Concept>& base) 
    {
        int up, down; up = down = 0;
        
        for (int i = 0; i < base.size(); ++i) { 
            up += (base[i] == mica::Up); down += (base[i] == mica::Down); 
        }
        
        if (up == down) { return mica::Undefined; }
        else {
            return ((up > down) ? mica::Up : mica::Down);
        }
    }
    
    static int getFurthest (Music& music, NodePtr chord) 
    {
        Array<NodePtr> notes = music.getAllTokensNext (chord);
        
        int furthest = 0;
        
        for (int i = 0; i < notes.size(); ++i) {
            mica::Concept chromatic = notes[i]->getObject().getAttribute (mica::Value);
            mica::Concept linespace = music.getClefState().getLinespace (chromatic);
            int current = linespace.getNumerator();
            if (Math::abs (current) > Math::abs (furthest)) { furthest = current; }
        }
        
        return furthest;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
