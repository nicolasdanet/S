
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameters::Header::paint (juce::Graphics& g)
{
    juce::Rectangle<int> b (getLocalBounds().reduced (2, 0).withTrimmedBottom (1));
    
    g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    
    const juce::Rectangle<int> arrow (b.removeFromLeft (b.getCentreY()));
    
    g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderArrow));

    if (owner_->isExpanded (index_)) { LookAndFeel::drawArrowOpened (g, arrow); }
    else {
        LookAndFeel::drawArrowClosed (g, arrow);
    }
    
    g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderText));
    g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
    g.drawText (getName(), b.reduced (4, 0), juce::Justification::centredLeft, true);
}

void Parameters::Header::mouseUp (const juce::MouseEvent& e)
{
    if (!e.mouseWasDraggedSinceMouseDown()) { owner_->expandPanel (index_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
