
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Invariant {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void setProperties (juce::ValueTree& t, const Invariant& i)
    {
        t.setProperty (Id::key,   juce::String (i.key),   nullptr);
        t.setProperty (Id::type,  juce::String (i.type),  nullptr);
        t.setProperty (Id::label, juce::String (i.label), nullptr);
        t.setProperty (Id::tips,  juce::String (i.tips),  nullptr);
    }
    
public:
    juce::StringRef key;
    juce::StringRef type;
    juce::StringRef label;
    juce::StringRef tips;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
