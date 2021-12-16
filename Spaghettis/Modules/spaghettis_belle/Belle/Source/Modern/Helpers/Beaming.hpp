
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Everything related to beams rendering. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Beaming {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static double getSlant (Music& music, const Array<NodePtr>& chords, mica::Concept direction)
    {
        double slant = 0.0;
        
        if ((chords.size() > 1) && !hasHorizontalSlant (music, chords, direction)) {
        //
        NodePtr island1 = music.getIsland (chords.getFirst());
        NodePtr island2 = music.getIsland (chords.getLast());
        Pointer<Stamp> stamp1 = island1->getObject().getStamp();
        Pointer<Stamp> stamp2 = island2->getObject().getStamp();
        Box box1 = stamp1->getBox (mica::Stem, stamp1->getContext());
        Box box2 = stamp2->getBox (mica::Stem, stamp2->getContext());
        
        if (!box1.isEmpty() && !box2.isEmpty()) {
        //
        double t1 = box1.getTop();
        double t2 = box2.getTop();
        double b1 = box1.getBottom();
        double b2 = box2.getBottom();
        double y1 = (Math::abs (t1) > Math::abs (b1)) ? t1 : b1;
        double y2 = (Math::abs (t2) > Math::abs (b2)) ? t2 : b2;
       
        slant = y2 - y1;
        
        slant = Math::min (slant,  House::kBeamSlantMaximum);
        slant = Math::max (slant, -House::kBeamSlantMaximum);
        //
        }
        //
        }
        
        return slant;
    }

    static double getAngle (const Box& beam, mica::Concept direction)
    {
        double angle = 0.0;
        
        if (!beam.isEmpty()) {
            Line line (beam.getBottomLeft(), beam.getTopRight() - Vector (0.0, House::kBeamThickness));
            angle = line.angle();
            if (direction == mica::Down) { angle = -angle; }
        }
        
        return angle;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.behindbarsnotation.co.uk/contents/sample_pages.pdf > */

private:
    static bool hasHorizontalSlant (Music& music, const Array<NodePtr>& chords, mica::Concept direction)
    {
        if (chords.size() <= 2) { return false; }
        else {
        //
        Array<int> linespaces;
        
        for (int i = 0; i < chords.size(); ++i) { 
            linespaces.add (getLinespace (music, chords[i], direction)); 
        }
        
        if (hasConcaveShape (linespaces, direction) || hasRepeatedPattern (linespaces)) { return true; }
        else {
            return false;
        }
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static bool hasConcaveShape (const Array<int>& linespaces, mica::Concept direction)
    {
        bool concave = false;
        
        int a = linespaces.getFirst();
        int b = linespaces.getLast();
        
        for (int i = 1; i < linespaces.size() - 1; ++i) {
        //
        if (direction == mica::Down) { if (linespaces[i] < Math::min (a, b)) { concave = true; break; } } 
        else {
            if (linespaces[i] > Math::max (a, b)) { concave = true; break; }
        }
        //
        }
        
        return concave;
    }
    
    static bool hasRepeatedPattern (const Array<int>& linespaces)
    {
        bool repeated = false;
        
        if ((linespaces.size() % 2) == 0) {
        //
        int iterate = Math::min (House::kBeamPatternMaximum, static_cast<int> ((linespaces.size() / 2)));
        for (int i = 2; i <= iterate; ++i) {
            if (hasRepeatedPattern (linespaces, i)) { repeated = true; break; }
        }
        //
        }
        
        return repeated;
    }
    
    static bool hasRepeatedPattern (const Array<int>& linespaces, int size)
    {
        bool repeated = false;
        
        if ((linespaces.size() % size) == 0) {
        //
        repeated = true;
        
        std::string t;
        
        for (int i = 0; i < (linespaces.size() / size); ++i) {
        //
        std::ostringstream s;
        for (int j = 0; j < size; ++j) { s << linespaces[(i * size) + j]; }
        if (t.length() && (t != s.str())) { repeated = false; break; }
        t = s.str();
        //
        }
        //
        }
        
        return repeated;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    static int getLinespace (Music& music, NodePtr chord, mica::Concept direction)
    {
        Array<NodePtr> notes = music.getAllTokensNext (chord);
        
        int nearest = 0;
        
        if (notes.size()) {
        //
        Array<int> linespaces;
        
        for (int i = 0; i < notes.size(); ++i) {
            mica::Concept chromatic = notes[i]->getObject().getAttribute (mica::Value);
            mica::Concept linespace = music.getClefState().getLinespace (chromatic);
            linespaces.add (linespace.getNumerator());
        }
        
        linespaces.sort();

        if (direction == mica::Down) { nearest = linespaces.getFirst(); }
        else {
            nearest = linespaces.getLast();
        }
        //
        }
        
        return nearest;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
