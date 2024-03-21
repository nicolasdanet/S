
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font ParametersView::getFont() const
{
    return base_.getFont();
}

int ParametersView::getNumberOfPanels() const
{
    return panel_.getNumPanels();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParametersView::requireExpandPanel (const juce::String& group)
{
    const int primeInterval = 307; requiredPanel_ = group; startTimer (primeInterval);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ConcertinaPanel& ParametersView::getPanel()
{
    return panel_;
}

void ParametersView::resizePanel (const juce::Rectangle<int>& bounds)
{
    panel_.setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParametersView::expandPanel (int index)
{
    const int i = juce::jmax (0, index);
    
    if (getNumberOfPanels() > 1) {
    //
    if (expanded_ == i) { jassert (expandedLast_ != i); expandPanel (expandedLast_); }
    else {
        expandedLast_   = expanded_;
        expanded_       = i;
        panel_.expandPanelFully (panel_.getPanel (i), true);
    }
    //
    }
}

bool ParametersView::isExpanded (int i)
{
    return (i == expanded_);
}

void ParametersView::timerCallback()
{
    stopTimer(); expandPanel (data_.getGroupIndex (requiredPanel_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParametersView::addPanel (juce::PropertyPanel* p)
{
    const int headerSize = base_.getRequiredHeight() + 6;
    const int i = getNumberOfPanels();
    auto h = std::make_unique<ParametersViewHeader> (p->getName(), i, this);
    
    panel_.addPanel (-1, p, true);
    panel_.setCustomPanelHeader (p, h.release(), true);
    panel_.setPanelHeaderSize (p, headerSize);
    
    expanded_ = i;
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

void buildConcertinaPanelParameter (const data::Parameter& p,
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
// MARK: -

void ParametersView::buildConcertinaPanel (const data::Data& data,
    const ParametersBase& base,
    ParametersView& v)
{
    for (const auto& group : data) {
    //
    if (group.isHidden() == false) {
    //
    auto panel = std::make_unique<juce::PropertyPanel> (group.getName());
    
    juce::Array<juce::PropertyComponent*> components;
    
    for (const auto& parameter : group) {
        if (!parameter.isHidden()) { buildConcertinaPanelParameter (parameter, base, components); }
    }
    
    panel->addProperties (components);
    
    v.addPanel (panel.release());
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
