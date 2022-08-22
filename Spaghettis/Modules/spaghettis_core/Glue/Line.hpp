
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Line {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Line (const juce::ValueTree& t) : tree_ (t)
    {
        Report::isLine (tree_);
    }

public:
    ~Line() = default;

public:
    Line (const Line&) = default;
    Line (Line&&) = default;
    Line& operator = (const Line&) = default;
    Line& operator = (Line&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    UniqueId getIdentifier() const
    {
        return Cast::fromVar<UniqueId> (tree_.getProperty (Ids::identifier));
    }

    UniqueId getIdentifierOfSource() const
    {
        return Cast::fromVar<UniqueId> (tree_.getProperty (Ids::source));
    }
    
    UniqueId getIdentifierOfDestination() const
    {
        return Cast::fromVar<UniqueId> (tree_.getProperty (Ids::destination));
    }
    
    int getInlet() const
    {
        return tree_.getProperty (Ids::inlet);
    }
    
    int getOutlet() const
    {
        return tree_.getProperty (Ids::outlet);
    }
    
private:
    juce::ValueTree tree_;
    
private:
    JUCE_LEAK_DETECTOR (Line)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
