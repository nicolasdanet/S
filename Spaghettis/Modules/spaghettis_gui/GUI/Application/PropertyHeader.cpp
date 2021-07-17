
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PropertyHeader::paint (juce::Graphics& g)
{
    juce::Rectangle<int> b (getLocalBounds().reduced (2, 0).withTrimmedBottom (1));
    
    g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderText));
    g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
    g.drawText (getName(), b.reduced (8, 0), juce::Justification::centredLeft, true);
}

void PropertyHeader::mouseUp (const juce::MouseEvent& e)
{
    if (!e.mouseWasDraggedSinceMouseDown()) { owner_->expandPanel (index_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
