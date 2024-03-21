
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void paintArrow (juce::Graphics& g, const juce::Rectangle<int>& r, int numberOfPanels, bool expanded)
{
    const juce::Colour c (Colours::fetchColour (Colours::parametersHeaderArrow));
    
    g.setColour ((numberOfPanels > 1) ? c : c.withAlpha (0.25f));

    if (expanded) { LNF::drawArrowDown (g, r); }
    else {
        LNF::drawArrowRight (g, r);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ViewCommon::paintHeader (juce::Graphics& g,
    const juce::Rectangle<int>& bounds,
    const juce::Font& font,
    const juce::String& name,
    int numberOfPanels,
    bool expanded)
{
    juce::Rectangle<int> b (bounds.reduced (1, 0).withTrimmedBottom (1));
    
    g.setColour (Colours::fetchColour (Colours::parametersHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    
    paintArrow (g, b.removeFromLeft (b.getCentreY()), numberOfPanels, expanded);
    
    g.setColour (Colours::fetchColour (Colours::parametersHeaderText));
    g.setFont (font);
    g.drawText (name, b.reduced (4, 0), juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::unique_ptr<juce::PropertyComponent> createPropertyComponent (const data::Parameter& p,
    const ParametersBase& base)
{
    const juce::String label (p.getLabel());
    
    // return std::make_unique<ParameterSlider> (p, base, label);
    
    if (p.isBoolean())      { return std::make_unique<ParameterBoolean> (p, base, label); }
    else if (p.isColour())  { return std::make_unique<ParameterColour> (p, base, label);  }
    else if (p.isInteger()) { return std::make_unique<ParameterInteger> (p, base, label); }
    else if (p.isFloat())   { return std::make_unique<ParameterFloat> (p, base, label);   }
    else {
        return std::make_unique<ParameterText> (p, base, label);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ViewCommon::buildConcertinaPanel (const data::Parameter& p,
    const ParametersBase& base,
    juce::Array<juce::PropertyComponent*>& c)
{
    std::unique_ptr<juce::PropertyComponent> t (createPropertyComponent (p, base));
    
    t->setPreferredHeight (base.getRequiredHeight());
    t->setTooltip (p.getInfo());
    
    c.add (t.release());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
