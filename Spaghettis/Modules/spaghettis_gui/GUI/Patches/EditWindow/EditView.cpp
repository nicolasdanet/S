
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditView::valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&)
{
    ;
}

void EditView::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (Ids::OBJECT)) {
        DBG (juce::String ("Added / ") + juce::String (core::Object::getIdentifier (child)));
    }
}

void EditView::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (Ids::OBJECT)) {
        DBG (juce::String ("Removed / ") + juce::String (core::Object::getIdentifier (child)));
    }
}
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
