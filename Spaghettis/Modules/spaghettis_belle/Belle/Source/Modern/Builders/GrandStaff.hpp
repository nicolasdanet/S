
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Helper to build a music graph. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/File:Grand_staff.svg > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class GrandStaff {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    GrandStaff (mica::Concept key, mica::Concept time = mica::CommonTime) : key_ (key)
    {
        beats_ = (time == mica::CutTime) ? 2 : 4; 
        value_ = (time == mica::CutTime) ? 2 : 4;
        
        begin();
        
        brace (music_.addKey (key));
        brace (music_.addTime (time));
    }
    
    GrandStaff (mica::Concept key, int beats, int value) : key_ (key), beats_ (beats), value_ (value)
    {
        begin();
        
        brace (music_.addKey (key));
        brace (music_.addTime (beats, value));
    }
    
public:
    GrandStaff (const GrandStaff&) = delete;
    GrandStaff& operator = (const GrandStaff&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    mica::Concept getKey() const
    {
        return key_;
    }
    
    int getBeats() const
    {
        return beats_;
    }
    
    int getValue() const
    {
        return value_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    GrandStaff& lower (NodePtr island = NodePtr (nullptr), bool selected = false)
    {
        if (selected && (island != nullptr)) { 
            island->getObject().setAttribute (mica::Status, mica::Selected);
        }
        
        lowerClef_.add (island);
        
        return *this;
    }
    
    GrandStaff& upper (NodePtr island = NodePtr (nullptr), bool selected = false)
    {
        if (selected && (island != nullptr)) { 
            island->getObject().setAttribute (mica::Status, mica::Selected);
        }
        
        upperClef_.add (island);
        
        return *this;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    GrandStaff& brace (NodePtr island, bool selected = false)
    {
        PRIM_ASSERT (island != nullptr);
        
        NodePtr cloned (music_.cloneIsland (island));
        
        if (selected) { 
            island->getObject().setAttribute (mica::Status, mica::Selected); 
            cloned->getObject().setAttribute (mica::Status, mica::Selected);
        }
        
        upperClef_.add (island);
        lowerClef_.add (cloned);
        
        return *this;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void begin()
    {
        music_.clear();
        
        upper (music_.addBarline (mica::BeginBarline));
        upper (music_.addClef (mica::TrebleClef));
        lower (music_.addBarline (mica::BeginBarline));
        lower (music_.addClef (mica::BassClef));
    }
    
    MusicBase& close()
    {
        if (!upperClef_.size() || (upperClef_.size() != lowerClef_.size())) { music_.clear(); }
        else {
        //
        upper (music_.addBarline (mica::EndBarline));
        lower (music_.addBarline (mica::EndBarline));
        
        for (int i = 0; i < upperClef_.size(); ++i) { 
            if (upperClef_[i] && lowerClef_[i]) {
                music_.connect (upperClef_[i], lowerClef_[i], mica::Instantwise);
            }
        }

        removeEmpty (upperClef_);
        removeEmpty (lowerClef_);
        connectPartwise (upperClef_);
        connectPartwise (lowerClef_);
        //
        }
        
        upperClef_.clear();
        lowerClef_.clear();
        
        return music_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    MusicBase* operator->()
    {
        return &music_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void removeEmpty (Array<NodePtr>& part)
    {
        Array<NodePtr> scoped;
        for (int i = 0; i < part.size(); ++i) { if (part[i]) { scoped.add (part[i]); } }
        scoped.swapWith (part);
    }
    
    void connectPartwise (Array<NodePtr>& part)
    {
        if (part.size() > 1) {
            for (int i = 1; i < part.size(); ++i) { music_.connect (part[i - 1], part[i], mica::Partwise); }
        }
    }
    
private:
    MusicBase music_;
    Array<NodePtr> upperClef_;
    Array<NodePtr> lowerClef_;
    mica::Concept key_;
    int beats_;
    int value_;

private:
    PRIM_LEAK_DETECTOR (GrandStaff)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
