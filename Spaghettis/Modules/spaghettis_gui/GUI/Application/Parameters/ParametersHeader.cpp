
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterHeader::paintArrow (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    const juce::Colour c (Spaghettis()->getColour (Colours::parametersHeaderArrow));
    
    g.setColour ((owner_->getNumberOfPanels() > 1) ? c : c.withAlpha (0.25f));

    if (owner_->isExpanded (index_)) { LookAndFeel::drawArrowOpened (g, r); }
    else {
        LookAndFeel::drawArrowClosed (g, r);
    }
}

void ParameterHeader::paint (juce::Graphics& g)
{
    juce::Rectangle<int> b (getLocalBounds().reduced (1, 0).withTrimmedBottom (1));
    
    g.setColour (Spaghettis()->getColour (Colours::parametersHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    
    paintArrow (g, b.removeFromLeft (b.getCentreY()));
    
    g.setColour (Spaghettis()->getColour (Colours::parametersHeaderText));
    g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
    g.drawText (getName(), b.reduced (4, 0), juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterHeader::mouseUp (const juce::MouseEvent& e)
{
    if (!e.mouseWasDraggedSinceMouseDown()) { owner_->expandPanel (index_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
