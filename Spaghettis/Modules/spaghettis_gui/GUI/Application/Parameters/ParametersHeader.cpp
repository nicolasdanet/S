
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterHeader::paint (juce::Graphics& g)
{
    juce::Rectangle<int> b (getLocalBounds().reduced (2, 0).withTrimmedBottom (1));
    
    g.setColour (Spaghettis()->getColour (Colours::parametersHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    
    const juce::Rectangle<int> arrow (b.removeFromLeft (b.getCentreY()));
    
    g.setColour (Spaghettis()->getColour (Colours::parametersHeaderArrow));

    if (owner_->isExpanded (index_)) { LookAndFeel::drawArrowOpened (g, arrow); }
    else {
        LookAndFeel::drawArrowClosed (g, arrow);
    }
    
    g.setColour (Spaghettis()->getColour (Colours::parametersHeaderText));
    g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
    g.drawText (getName(), b.reduced (4, 0), juce::Justification::centredLeft, true);
}

void ParameterHeader::mouseUp (const juce::MouseEvent& e)
{
    if (!e.mouseWasDraggedSinceMouseDown()) { owner_->expandPanel (index_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
