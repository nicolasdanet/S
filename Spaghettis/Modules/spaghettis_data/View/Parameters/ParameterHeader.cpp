
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
    const juce::Colour c (Colours::fetchColour (Colours::parametersHeaderArrow));
    
    g.setColour ((owner_->getNumberOfPanels() > 1) ? c : c.withAlpha (0.25f));

    if (owner_->isExpanded (index_)) { LNF::drawArrowDown (g, r); }
    else {
        LNF::drawArrowRight (g, r);
    }
}

void ParameterHeader::paint (juce::Graphics& g)
{
    juce::Rectangle<int> b (getLocalBounds().reduced (1, 0).withTrimmedBottom (1));
    
    g.setColour (Colours::fetchColour (Colours::parametersHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    
    paintArrow (g, b.removeFromLeft (b.getCentreY()));
    
    g.setColour (Colours::fetchColour (Colours::parametersHeaderText));
    g.setFont (owner_->getFont());
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
