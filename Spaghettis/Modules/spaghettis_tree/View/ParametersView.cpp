
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font ParameterView::getFont() const
{
    return base_.getFont();
}

int ParameterView::getNumberOfPanels() const
{
    return panel_.getNumPanels();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterView::requireExpandPanel (const juce::String& group)
{
    const int primeInterval = 307; requiredPanel_ = group; startTimer (primeInterval);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ConcertinaPanel& ParameterView::getPanel()
{
    return panel_;
}

void ParameterView::resizePanel (const juce::Rectangle<int>& bounds)
{
    panel_.setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterView::expandPanel (int index)
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

bool ParameterView::isExpanded (int i)
{
    return (i == expanded_);
}

void ParameterView::timerCallback()
{
    stopTimer(); expandPanel (data_.getGroupIndex (requiredPanel_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterView::addPanel (juce::PropertyPanel* p)
{
    const int headerSize = base_.getRequiredHeight() + 6;
    const int i = getNumberOfPanels();
    auto h = std::make_unique<ParameterHeader> (p->getName(), i, this);
    
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

std::unique_ptr<juce::PropertyComponent> createPropertyComponent (const core::Parameter& p,
    const ParameterBase& base)
{
    // return std::make_unique<ParameterSlider> (p, base);
    
    if (p.isBoolean())      { return std::make_unique<ParameterBoolean> (p, base); }
    else if (p.isColour())  { return std::make_unique<ParameterColour> (p, base);  }
    else if (p.isInteger()) { return std::make_unique<ParameterInteger> (p, base); }
    else if (p.isFloat())   { return std::make_unique<ParameterFloat> (p, base);   }
    else {
        return std::make_unique<ParameterText> (p, base);
    }
}

void buildConcertinaPanelParameter (const core::Parameter& p,
    const ParameterBase& base,
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

void ParameterView::buildConcertinaPanel (const core::Data& data, const ParameterBase& base, ParameterView& v)
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