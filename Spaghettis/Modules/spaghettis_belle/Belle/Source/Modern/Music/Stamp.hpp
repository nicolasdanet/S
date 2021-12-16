
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A group of graphics with an affine context. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Stamp {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Stamp() : part_ (-1), instant_ (-1), weight_ (0.0)
    {
    }

public:
    Stamp (const Stamp&) = delete;
    Stamp& operator = (const Stamp&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isEmpty() const
    {
        return (getPath().isEmpty());
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// mica::Extra   / Extra for spacing algorithm.
// mica::Beam    / Bounds of the beams.
// mica::Stem    / Bounds of the stems.
// mica::CSharp4 / Anchors for tied notes.

public:
    Box getBox (mica::Concept concept, const Affine& affine = Affine::identity()) const
    {
        Box box;
        
        for (int i = 0; i < graphics_.size(); ++i) { 
        //
        const Path& path (graphics_[i]->getPath()); 
        if (path.getBox (concept) != Box::empty()) { 
            box += affine.appliedTo (path.getBox (concept)); 
        }
        //    
        }
        
        return box;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// mica::Large   / Symbol over multiple staves (mica::True).
// mica::Beam    / Slope direction (mica::Up or mica::Down).
// mica::CSharp4 / Tie direction (mica::Above or mica::Below).

public:
    mica::Concept getProperty (mica::Concept key) const     /* Fetch the first matched. */
    {
        for (int i = 0; i < graphics_.size(); ++i) { 
        //
        const Path& path (graphics_[i]->getPath()); 
        if (path.getProperty (key) != mica::Undefined) { return path.getProperty (key); }
        //    
        }
        
        return mica::Undefined;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getPart()
    {
        return part_;
    }
    
    int getInstant()
    {
        return instant_;
    }

    double getWeight()
    {
        return weight_;
    }
    
    void setPart (int part) 
    {
        part_ = part;
    }

    void setInstant (int instant)
    {
        instant_ = instant;
    }

    void setWeight (double weight)
    {
        weight_ = weight;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const Affine& getContext() const
    {
        return context_;
    }
    
    void addContext (const Affine& affine)
    {
        context_ *= affine;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Path getPath (const Affine& affine = Affine::identity()) const
    {
        Path path;
        for (int i = 0; i < graphics_.size(); ++i) { path.addPath (graphics_[i]->getPath(), affine); }
        return path;
    }
    
    Box getBounds (const Affine& affine = Affine::identity()) const
    {
        Box box;
        for (int i = 0; i < graphics_.size(); ++i) { box += graphics_[i]->getBounds (affine); }
        return box;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void add (const Pointer<Graphic>& graphic)
    {
        graphics_.add (graphic);
    }

    void erase() 
    {
        graphics_.clear();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (Painter& painter)
    {
        painter.pushAffine (context_);
        
        for (int i = graphics_.size() - 1; i >= 0; --i) { graphics_[i]->paint (painter); }
        
        painter.popAffine (1);
    }
  
private:
    int part_;
    int instant_;
    double weight_;
    Array<Pointer<Graphic>> graphics_;
    Affine context_;

private:
    PRIM_LEAK_DETECTOR (Stamp)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
