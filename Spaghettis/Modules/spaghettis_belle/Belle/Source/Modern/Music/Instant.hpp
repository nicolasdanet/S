
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Collected stamps by instants. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Instant {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Instant() : unit_ (House::kDensityUnit), position_ (0.0)
    {
    }

public:
    Instant (const Instant&) = delete;
    Instant& operator = (const Instant&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    double getUnit() const              /* Spacing factor (default is House::kDensityUnit). */
    {
        return unit_;
    }
    
    void setUnit (double unit)
    {
        unit_ = unit;
    }

    double getPosition() const
    {
        return position_;
    }

    void setPosition (double position)
    {
        position_ = position;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Pointer<Stamp> getStampAtPart (int part) const
    {
        if (part < stamps_.size()) { return stamps_[part]; }
        else {
            return Pointer<Stamp> (nullptr);
        }
    }

    void addStamp (const Pointer<Stamp>& stamp)
    {
        int i = stamp->getPart();
        if (stamps_.size() <= i) { stamps_.resize (i + 1); } stamps_[i] = stamp;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isUnique() const       /* Just one part? */
    {
        int k = 0;
        for (int i = 0; i < stamps_.size(); ++i) { if (stamps_[i]) { k++; } }
        return (k == 1);
    }
    
    int getUnique() const       /* Get that part. */
    {
        for (int i = 0; i < stamps_.size(); ++i) { if (stamps_[i]) { return i; } }
        return -1;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    double getWeight (int part) const                           /* The weight for the spacing algorithm. */
    {   
        Pointer<Stamp> stamp = getStampAtPart (part);
        
        if (stamp) { return stamp->getWeight(); }
        else {
            return 0.0;
        }
    }
    
    Path getSpacing (int part) const                            /* The path plus some extra spaces. */
    {
        Path path;
        
        Pointer<Stamp> stamp = getStampAtPart (part);
        
        if (stamp) {
        //
        path.addPath (stamp->getPath (stamp->getContext()));
        Box box (stamp->getBox (mica::Extra, stamp->getContext()));
        if (!box.isEmpty()) { path.addRectangle (box); }
        //
        }
        
        return path;
    }

    double getWeight() const
    {   
        double w = 0.0;
        
        for (int i = 0; i < stamps_.size(); ++i) { 
            if (stamps_[i]) { w = Math::max (w, stamps_[i]->getWeight()); }
        }
        
        return w;
    }
    
    Path getSpacing() const
    {
        Path path;
        
        for (int i = 0; i < stamps_.size(); ++i) { 
        //
        if (stamps_[i]) { 
        //
        path.addPath (stamps_[i]->getPath (stamps_[i]->getContext())); 
        Box box (stamps_[i]->getBox (mica::Extra, stamps_[i]->getContext()));
        if (!box.isEmpty()) { path.addRectangle (box); }
        //
        } 
        //
        }
        
        return path;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Box getBounds() const
    {
        Box box;
        
        for (int i = 0; i < stamps_.size(); ++i) { 
            if (stamps_[i]) { box += stamps_[i]->getBounds (stamps_[i]->getContext()); } 
        }
        
        return box;
    }
    
private:
    double unit_;
    double position_;
    Array<Pointer<Stamp>> stamps_;

private:
    PRIM_LEAK_DETECTOR (Instant)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
