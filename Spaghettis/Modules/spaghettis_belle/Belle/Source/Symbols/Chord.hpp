
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Chord_%28music%29 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* No support for bitonal chord and tonal cluster. */

/* < http://musescore.org/en/node/14449 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* No support for Maxima, Longa, Breve (and relative rests). */

/* < https://en.wikipedia.org/wiki/Double_whole_note > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Chord {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Note {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Note() : linespace_ (0), isTied_ (false)
    {
    }
    
    Note (int i, mica::Concept accidental, mica::Concept chromatic, bool tied)
    {
        linespace_  = i;
        accidental_ = accidental;
        chromatic_  = chromatic;
        isTied_     = tied;
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
    bool operator < (const Note& o) const
    {
        mica::Concept diff = mica::index (mica::Accidentals, o.accidental_, accidental_);
        return (linespace_ < o.linespace_) || ((linespace_ == o.linespace_) && (diff.getNumerator() < 0));
    }
  
    bool operator > (const Note& o) const
    {
        mica::Concept diff = mica::index (mica::Accidentals, o.accidental_, accidental_);
        return (linespace_ > o.linespace_) || ((linespace_ == o.linespace_) && (diff.getNumerator() > 0));
    }

    bool operator == (const Note& o) const
    {
        return (linespace_ == o.linespace_) && (accidental_ == o.accidental_);
    }
    
    bool operator != (const Note& o) const
    {
        return !(*this == o);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getLinespace() const
    {
        return linespace_;
    }

    mica::Concept getAccidental() const
    {
        return accidental_;
    }
    
    mica::Concept getChromatic() const
    {
        return chromatic_;
    }
    
    bool isTied() const
    {
        return isTied_;
    }
    
private:
    int linespace_;
    mica::Concept accidental_;
    mica::Concept chromatic_;
    bool isTied_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord() : duration_ (Ratio (1, 4)), isStemUp_ (true)
    {
    }

public:
    Chord (const Chord&) = default;
    Chord (Chord&&) = default;
    Chord& operator = (const Chord&) = default;
    Chord& operator = (Chord&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Chord& setDuration (mica::Concept duration)
    {
        duration_ = duration.toRatio(); return *this;
    }
    
    Chord& setBeam (mica::Concept beam)
    {
        beam_ = beam; return *this;
    }

    Chord& setDirection (mica::Concept direction)
    {
        direction_ = direction; return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addNote (mica::Concept linespace, mica::Concept accidental, mica::Concept chromatic, bool tied)
    {
        notes_.add (Chord::Note (linespace.getNumerator(), accidental, chromatic, tied));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Due to path filling artifacts flags are engraved later. */

public:
    void addToPath (Path& path) const
    {
        if (!notes_.size()) { path.addShape (Shapes::Rest (duration_)); }
        else {
        //
        prepare (path);         /* Note that in functions below only the sorted notes are used. */
        
        path.addPath (engraveLedgerLines());
        
        Path notes (engraveNotes());
        path.addPath (engraveDots (notes));
        path.addPath (engraveAccidentals (notes));
        path.addPath (notes);
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    mica::Concept getDirection() const
    {
        return direction_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void prepare (Path& path) const
    {
        /* In case. */
        
        sorted_.clear();
        clustered_.clear();
        
        /* Sort and remove duplicated notes if any. */
        
        Array<Note> temp (notes_);
        temp.sort();
        
        sorted_.add (temp.getFirst());
        for (int i = 1; i < temp.size(); ++i) { if (temp[i] != temp[i - 1]) { sorted_.add (temp[i]); } }
        
        /* Determine the ties direction. */
        
        temp.clear();
        for (int i = 0; i < sorted_.size(); ++i) {
            if (sorted_[i].isTied()) { temp.add (sorted_[i]); } 
        }
        
        if (temp.size()) {
        //
        path.setProperty (temp.getFirst().getChromatic(), mica::Below);
        path.setProperty (temp.getLast().getChromatic(), mica::Above);
        
        for (int i = 1; i < temp.size() - 1; ++i) {
            if (temp[i].getLinespace() >= 0) { path.setProperty (temp[i].getChromatic(), mica::Above); }
            else { 
                path.setProperty (temp[i].getChromatic(), mica::Below);
            }
        }
        //
        }
        
        /* Determine the stem direction. */
        
        if (direction_ == mica::Undefined) {
            Array<int> linespaces;
            for (int i = 0; i < sorted_.size(); ++i) { linespaces.add (sorted_[i].getLinespace()); }
            direction_ = Utils::getDirection (linespaces);
        }
        
        isStemUp_ = (direction_ == mica::Up);
        
        /* Base of the chord always comes first. */
                        
        if (isStemUp_ == false) { sorted_.reverse(); }
        
        /* Group notes by clusters (consecutive staff lines and staff spaces). */
        
        Array<Note> cluster;
        cluster.add (sorted_.getFirst());
        clustered_.add (cluster);
      
        for (int i = 1; i < sorted_.size(); ++i) {
            if ((Math::abs ((sorted_[i].getLinespace() - sorted_[i - 1].getLinespace())) < 2)) {
                clustered_.getLast().add (sorted_[i]);
            } else {
                Array<Note> newCluster;
                newCluster.add (sorted_[i]);
                clustered_.add (newCluster);
            }
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Path engraveLedgerLines() const
    {
        Path path;
                
        int highNote    = getHighestLinespace();
        int lowNote     = getLowestLinespace();
        int highCluster = getHighestLinespaceInClusters();
        int lowCluster  = getLowestLinespaceInClusters();
                
        for (int i = Utils::getTopLine() + 2; i <= highNote; i += 2) {
            Affine affine = Affine::translation (Vector (0.0, Utils::getPosition (i)));
            if (i < highCluster) { path.addShape (Shapes::LedgerLineTwoColumn (isStemUp_), affine); } 
            else {
                path.addShape (Shapes::LedgerLine(), affine);
            }
        }
      
        for (int i = Utils::getBottomLine() - 2; i >= lowNote; i -= 2) {
            Affine affine = Affine::translation (Vector (0.0, Utils::getPosition (i)));
            if (i > lowCluster) { path.addShape (Shapes::LedgerLineTwoColumn (isStemUp_), affine); } 
            else {
                path.addShape (Shapes::LedgerLine(), affine);
            }
        }
        
        return path;
    }

    Path engraveNotes() const
    {
        Path path;
        
        for (int i = 0; i < sorted_.size(); ++i) {
        //
        Ratio base = Utils::getUndottedValue (duration_);
        double y = Utils::getPosition (sorted_[i].getLinespace());
        double x = getColumn (i);
        
        Vector v;
        Affine origin;
        
        if (x) { v = Vector (x * (House::kNoteHeadWidthPrecise - (House::kNoteStemThickness / 2.0)), y); } 
        else {
            v = Vector (0.0, y);
        }
        
        origin = Affine::translation (v);
        
        path.getBox (sorted_[i].getChromatic()) = Box (v);       /* Anchors for ties. */
        
        if (i || (duration_ >= Ratio (1, 1))) {
        //
        if (base <= Ratio (1, 4))      { path.addShape (Shapes::QuarterNote().setHeight (0.0), origin); }
        else if (base == Ratio (1, 2)) { path.addShape (Shapes::HalfNote().setHeight (0.0), origin); }
        else {
            path.addShape (Shapes::WholeNote(), origin);
        }
        //
        } else {
        //
        double h = getHeight();
        
        if (base == Ratio (1, 2)) { path.addShape (Shapes::HalfNote().setHeight (h), origin); }
        else {
            if (beam_ == mica::Undefined) { path.addShape (Shapes::QuarterNote().setHeight (h), origin); } 
            else {
                path.addShape (Shapes::QuarterNote().hasBeam (true).setHeight (h), origin);
            }
        }
        //
        }
        //
        }
        
        /* Rearrange anchors to avoid collision with note heads. */
        
        double right = path.getBounds().getRight();
        for (int i = 0; i < sorted_.size(); ++i) { 
            path.getBox (sorted_[i].getChromatic()).setRight (right); 
        }
        
        return path;
    }
    
    Path engraveDots (Path& notes) const
    {
        Path path;
        
        int count = Utils::countDots (duration_);
        
        if (count) {
        //
        Path collision;
        
        /* Reserve the place for the flags. */
        
        if (beam_ == mica::Undefined && Utils::countFlags (duration_)) {
            Path flag ((Shapes::FlagStemUp()));                                 /* Avoid most vexing parse. */
            Affine affine = Affine::translation (flag.getBounds().getWidth());
            collision.addRectangle (notes.getBounds() + affine.appliedTo (notes.getBox (mica::Stem)));
            
        } else {
            collision.addRectangle (notes.getBounds());
        }
        
        double x = collision.getBounds().getRight() + House::kRhythmicDotDistance;
        
        Array<int> y;
        Array<int> remains;
        
        int previous = 1234;   /* Dummy. */
      
        for (int i = 0; i < sorted_.size(); ++i) {
        //
        int linespace = sorted_[i].getLinespace();
        int current = (linespace % 2) ? linespace : linespace + 1;
        if (current != previous) { y.add (current); previous = current; }
        else {
            remains.add (current);
        }        
        //
        }
      
        for (int i = 0; i < remains.size(); ++i) {
        //
        int j = 0;
        bool placed = false;
        
        while (!placed) {
        //
        int below = remains[i] - (2 * j);
        int above = remains[i] + (2 * j);
        
        if (y.contains (below) == false)      { y.add (below); placed = true; }
        else if (y.contains (above) == false) { y.add (above); placed = true; }
        
        j++;
        //
        }
        //
        }
        
        for (int i = 0; i < y.size(); ++i) {
        //
        for (int j = 0; j < count; ++j) {
        //
        Vector v (x + (j * House::kRhythmicDotExtra), Utils::getPosition (y[i]));
        Affine origin = Affine::translation (v);
        path.addShape (Shapes::RhythmicDot(), origin);
        collision.addShape (Shapes::RhythmicDot(), origin);
        //
        }
        //
        }
        
        /* Rearrange anchors to avoid collision with dots. */
        
        double right = collision.getBounds().getRight();
        for (int i = 0; i < sorted_.size(); ++i) { 
            notes.getBox (sorted_[i].getChromatic()).setRight (right); 
        }
        //
        }
        
        return path;
    }
    
    Path engraveAccidentals (Path& notes) const
    {
        Path path;
        
        Array<Path> shape;
        Array<int> order;
        Array<Vector> place;
      
        shape.resize (sorted_.size());
        order.resize (sorted_.size());
        place.resize (sorted_.size());
      
        for (int i = 0; i < sorted_.size(); ++i) {
        //
        mica::Concept accidental (sorted_[i].getAccidental());
        
        if (accidental == mica::DoubleFlat)       { shape[i] = Path (Shapes::AccidentalDoubleFlat());  }
        else if (accidental == mica::Flat)        { shape[i] = Path (Shapes::AccidentalFlat());        }
        else if (accidental == mica::Natural)     { shape[i] = Path (Shapes::AccidentalNatural());     }
        else if (accidental == mica::Sharp)       { shape[i] = Path (Shapes::AccidentalSharp());       }
        else if (accidental == mica::DoubleSharp) { shape[i] = Path (Shapes::AccidentalDoubleSharp()); }
        //
        }
      
        for (int i = 0; i < sorted_.size(); ++i)  {         /* Alterne from up and down to the middle. */
        //
        if (isStemUp_) {
            if (i % 2 == 0) { order[i] = (sorted_.size() - 1) - (i / 2); }
            else {
                order[i] = (i - 1) / 2;
            }

        } else {
            if (i % 2 == 0) { order[i] = i / 2; }
            else {
                order[i] = (sorted_.size() - 1) - (i - 1) / 2;
            }
        }
        //
        }
      
        Path collision;
        collision.addRectangle (notes.getBounds());
        
        for (int i = 0; i < sorted_.size(); ++i) {
        // 
        if (!shape[order[i]].isEmpty()) {
        //
        double y = Utils::getPosition (sorted_[order[i]].getLinespace());
        double d = Optics::nonCollidingByOutlines (collision, shape[order[i]], Point (0.0, y), kPi);
        place[order[i]] = Vector (-(d + House::kAccidentalExtra), y);
        collision.addPath (shape[order[i]], Affine::translation (place[order[i]]));
        //
        }
        //
        }
      
        Vector v (-(House::kAccidentalDistance - House::kAccidentalExtra), 0.0);
        
        for (int i = 0; i < sorted_.size(); ++i) {
        //
        if (!shape[order[i]].isEmpty()) {
        //
        path.addPath (shape[order[i]], Affine::translation (place[order[i]] + v));
        //
        }
        //
        }
        
        /* Rearrange anchors to avoid collision with accidentals. */
        
        double left = collision.getBounds().getLeft() + v.getX();
        for (int i = 0; i < sorted_.size(); ++i) {
            notes.getBox (sorted_[i].getChromatic()).setLeft (left); 
        }
        
        return path;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    double getHeight() const
    {
        /* Basic. */
        
        int last  = sorted_.getLast().getLinespace();
        int first = sorted_.getFirst().getLinespace();
        double height = House::kNoteStemHeight + (Math::abs (last - first) * (House::kNoteHeadHeight / 2.0));
        
        /* Extra flags. */
        
        int flags = Utils::countFlags (duration_);
        if (flags > 2) { height += (flags - 2) * House::kStaffSpaceWidth; }
        
        /* Ledger lines case. */
        
        bool top = !isStemUp_ && (last > (Utils::getTopLine() + 2));
        bool bottom = isStemUp_ && (last < (Utils::getBottomLine() - 2));
        if (top || bottom) { height = Math::max (height, Math::abs (Utils::getPosition (first))); } 
        
        /* Stem up or stem down. */
        
        return height * (isStemUp_ ? 1.0 : -1.0);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getColumn (int i) const
    {
        for (int j = 0; j < clustered_.size(); ++j) {       /* Stagger heads according to clusters. */
        //
        int n = clustered_[j].size();
        
        if (i >= n) { i -= n; }
        else {
            break;
        }
        //
        }
        
        if (i % 2 == 0) { return 0; }           
        else if (isStemUp_) { return 1; }
        else {
            return -1;
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getLowestLinespace() const
    {
        int n = sorted_.getFirst().getLinespace();
        for (int i = 1; i < sorted_.size(); ++i) { n = Math::min (n, sorted_[i].getLinespace()); }
        return n;
    }

    int getLowestLinespaceInClusters() const
    {
        int n = 0;
        bool first = true;
        
        for (int i = 0; i < clustered_.size(); ++i) {
        //
        if (clustered_[i].size() > 1) {
        //
        for (int j = 0; j < clustered_[i].size(); ++j) {
            if (first) { n = clustered_[i][j].getLinespace(); first = false; } 
            else {
                n = Math::min (n, clustered_[i][j].getLinespace());
            }
        }
        //
        }
        //
        }
        
        return n;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getHighestLinespace() const
    {
        int n = sorted_.getFirst().getLinespace();
        for (int i = 1; i < sorted_.size(); ++i) { n = Math::max (n, sorted_[i].getLinespace()); }
        return n;
    }
    
    int getHighestLinespaceInClusters() const
    {
        int n = 0;
        bool first = true;
        
        for (int i = 0; i < clustered_.size(); ++i) {
        //
        if (clustered_[i].size() > 1) {
        //
        for (int j = 0; j < clustered_[i].size(); ++j) {
            if (first) { n = clustered_[i][j].getLinespace(); first = false; } 
            else {
                n = Math::max (n, clustered_[i][j].getLinespace());
            }
        }
        //
        }
        //
        }
        
        return n;
    }

private:
    Ratio duration_;
    mutable mica::Concept direction_;
    mica::Concept beam_;
    Array<Note> notes_;
        
private:
    mutable bool isStemUp_;
    mutable Array<Note> sorted_;
    mutable Array<Array<Note>> clustered_;

private:
    PRIM_LEAK_DETECTOR (Chord)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
