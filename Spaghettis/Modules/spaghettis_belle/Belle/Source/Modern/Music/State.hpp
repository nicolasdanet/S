
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* State while traversing the graph. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace State {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Clef {    /* Get accidental and line depending on clef and key. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Clef()
    {
        setKeyAccidentals (mica::NoAccidentals);
    }

public:
    Clef (const Clef&) = delete;
    Clef& operator = (const Clef&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void reset()
    {
        attributes_.clear();
        actives_.clear();
        keys_.clear();
        
        setKeyAccidentals (mica::NoAccidentals);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void parse (NodePtr token)
    {
        mica::Concept kind  = token->getObject().getAttribute (mica::Kind);
        mica::Concept value = token->getObject().getAttribute (mica::Value);
        
        if (kind == mica::Clef)              { attributes_[kind] = value; }
        else if (kind == mica::KeySignature) { actives_.clear(); setKeyAccidentals (value); }
        else if (kind == mica::Barline)      { actives_.clear(); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    mica::Concept getAttribute (mica::Concept concept) const
    {
        return attributes_[concept];
    }

    mica::Concept getLinespace (mica::Concept chromatic) const
    {
        mica::Concept diatonic = mica::map (chromatic, mica::DiatonicPitch);
        mica::Concept linespace = mica::map (getAttribute (mica::Clef), diatonic);
        
        return linespace;
    }
    
    mica::Concept getAccidental (mica::Concept chromatic)
    {
        mica::Concept accidental = mica::map (chromatic, mica::Accidental);
        mica::Concept diatonic = mica::map (chromatic, mica::DiatonicPitch);
        mica::Concept letter = mica::map (diatonic, mica::Letter);
        
        mica::Concept current = actives_[diatonic];
        
        if (current != mica::Undefined) { if (accidental == current) { return mica::Undefined; } }
        else if (keys_[letter] == accidental) { return mica::Undefined;  } 

        actives_[diatonic] = accidental; 
        
        return accidental;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void setKeyAccidentals (mica::Concept keySignature)
    {
        keys_ = mica::MIR::Utils::getAccidentalsByLetters (keySignature);
    }

private:
    Table<mica::Concept> attributes_;
    Table<mica::Concept> actives_;           /* Active accidentals. */
    Table<mica::Concept> keys_;              /* Key accidentals. */

private:
    PRIM_LEAK_DETECTOR (Clef)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Chord {   /* Buffering chord values to define ties. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord()
    {
    }

public:
    Chord (const Chord&) = delete;
    Chord& operator = (const Chord&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void reset()
    {
        attributes_.clear();
        stamps_.clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void parse (NodePtr island, NodePtr token, const Pointer<Stamp>& stamp)
    {
        mica::Concept kind  = token->getObject().getAttribute (mica::Kind);
        mica::Concept value = token->getObject().getAttribute (mica::Value);
        
        if (kind == mica::Chord) { 
            attributes_[mica::Stem]   = token->getObject().getAttribute (mica::Stem);
            attributes_[mica::Size]   = token->getObject().getAttribute (mica::Size);
            attributes_[mica::Status] = island->getObject().getAttribute (mica::Status);
            
        } else if (kind == mica::Note) { 
            mica::Concept tie = token->getObject().getAttribute (mica::Tie);
            if (tie == mica::End) { stamps_[value] = nullptr; }
            else if (tie == mica::Beginning || tie == mica::Middle) {
                stamps_[value] = stamp;
            }
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    mica::Concept getAttribute (mica::Concept concept) const
    {
        return attributes_[concept];
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that values are returned in context. */

public:
    Path getPath (mica::Concept chromatic) const
    {
        if (stamps_[chromatic] == nullptr) { return Path(); }
        else { 
            return stamps_[chromatic]->getPath (stamps_[chromatic]->getContext());
        }
    }
    
    Box getBox (mica::Concept chromatic) const
    {
        if (stamps_[chromatic] == nullptr) { return Box::empty(); }
        else {
            return stamps_[chromatic]->getBox (chromatic, stamps_[chromatic]->getContext());
        }
    }
    
private:
    Table<mica::Concept> attributes_; 
    Table<mica::Concept, Pointer<Stamp>> stamps_;     /* Stamps of active notes to determine ties. */

private:
    PRIM_LEAK_DETECTOR (Chord)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Group {   /* Collecting chords to define tuplets and beams. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Group()
    {
    }

public:
    Group (const Group&) = delete;
    Group& operator = (const Group&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void reset()
    {
        group_.clear();
        boxes_.clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setBox (mica::Concept concept, const Box& box)
    {
        if (concept != mica::Undefined) { boxes_[concept] = box; }
    }

    Box getBox (mica::Concept concept) const
    {
        return boxes_[concept];
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void add (NodePtr token)
    {
        group_.add (token);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const Array<NodePtr>& getTokens() const
    {
        return group_;
    }

    Array<NodePtr> reclaimTokens()
    {
        Array<NodePtr> scoped;
        scoped.swapWith (group_);
        return scoped;
    }
    
private:
    Array<NodePtr> group_;
    Table<mica::Concept, Box> boxes_;

private:
    PRIM_LEAK_DETECTOR (Group)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace State

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
