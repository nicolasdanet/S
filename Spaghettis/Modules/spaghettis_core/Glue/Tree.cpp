
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Tree::getParentIfPropertyChangedEquals (const juce::ValueTree& t, juce::String key)
{
    if (t.hasType (Ids::PARAMETER) && (core::Parameter (t).getKey() == key)) {
    //
    return t.getParent().getParent().getParent();
    //
    }
    
    return {};
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------