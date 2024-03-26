
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
    const ParametersBase& base,
    const juce::String& label,
    bool isEditable)
{
    // return std::make_unique<ParameterSlider> (p, base, label, isEditable);
    
    if (p.isBoolean())      { return std::make_unique<ParameterBoolean> (p, base, label, isEditable); }
    else if (p.isColour())  { return std::make_unique<ParameterColour> (p, base, label, isEditable);  }
    else if (p.isInteger()) { return std::make_unique<ParameterInteger> (p, base, label, isEditable); }
    else if (p.isFloat())   { return std::make_unique<ParameterFloat> (p, base, label, isEditable);   }
    else {
        return std::make_unique<ParameterText> (p, base, label, isEditable);
    }
}

void addPropertyComponent (const data::Parameter& p,
    const ParametersBase& base,
    const juce::String& label,
    bool isEditable,
    juce::Array<juce::PropertyComponent*>& c)
{
    std::unique_ptr<juce::PropertyComponent> t (createPropertyComponent (p, base, label, isEditable));
    
    t->setPreferredHeight (base.getRequiredHeight());
    t->setTooltip (p.getInfo());
    
    c.add (t.release());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ViewCommon::buildPanel (const data::Data& data,
    const ParametersBase& base,
    ParametersView& v)
{
    for (const auto& group : data) {
    //
    if (group.isHidden() == false) {
    //
    auto panel = std::make_unique<juce::PropertyPanel> (group.getName());
    
    juce::Array<juce::PropertyComponent*> components;
    
    for (const auto& p : group) {
        if (!p.isHidden()) { addPropertyComponent (p, base, p.getLabel(), p.isEditable(), components); }
    }
    
    panel->addProperties (components);
    
    v.addPanel (panel.release());
    //
    }
    //
    }
}

void ViewCommon::buildPanel (const std::vector<PresetElement>& elements,
    const ParametersBase& base,
    PresetsView& v)
{
    auto panel = std::make_unique<juce::PropertyPanel> (v.getName());
    
    juce::Array<juce::PropertyComponent*> components;
    
    for (const auto& e : elements) {
    //
    const data::Parameter p (e.getParameter());
    
    if (!p.isHidden()) { addPropertyComponent (p, base, e.getLabel(), false, components); }
    //
    }
    
    panel->addProperties (components);
    
    v.addPanel (panel.release());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
