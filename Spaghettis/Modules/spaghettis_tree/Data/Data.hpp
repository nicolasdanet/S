
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Iter = core::Iterator<Group>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Data (const juce::Identifier& type) : tree_ (type)
    {
    }
    
    explicit Data (const juce::ValueTree& tree) : tree_ (tree)
    {
    }
    
    ~Data() = default;
        
public:
    Data (const Data&) = default;
    Data (Data&&) = default;
    Data& operator = (const Data&) = default;
    Data& operator = (Data&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::ValueTree asValueTree() const
    {
        return tree_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Iter begin() const { return Iter (tree_.begin()); }
    Iter end() const   { return Iter (tree_.end());   }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addObserver (Observer* observer)
    {
        tree_.addListener (observer);
    }
    
    void removeObserver (Observer* observer)
    {
        tree_.removeListener (observer);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool      hasParameter (const juce::String&, const juce::String&) const;
    Parameter getParameter (const juce::String&, const juce::String&) const;

public:
    void changeValue (const juce::String&, const juce::String&, const juce::var&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Group addGroup (const juce::String&, bool isHidden = false);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool  hasGroup (const juce::String&) const;
    Group getGroup (const juce::String&) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getGroupIndex (const juce::String&) const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void apply (const juce::ValueTree&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addParametersFromXmlDocumentation (const juce::String&);
    
public:
    bool readValuesFromFile (const juce::File&);
    void writeValuesToFile  (const juce::File&) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Data makeCopy (const Data&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::ValueTree tree_;

private:
    JUCE_LEAK_DETECTOR (Data)
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------