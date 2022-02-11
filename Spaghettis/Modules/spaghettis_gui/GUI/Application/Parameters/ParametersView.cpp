
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ConcertinaPanel& Parameters::View::getPanel()
{
    return panel_;
}

void Parameters::View::resizePanel (const juce::Rectangle<int>& bounds)
{
    panel_.setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameters::View::expandPanel (int i)
{
    if (panel_.getNumPanels() > 1) {
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

bool Parameters::View::isExpanded (int i)
{
    return (i == expanded_);
}

void Parameters::View::timerCallback()
{
    stopTimer(); expandPanel (0);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameters::View::addPanel (juce::PropertyPanel* p)
{
    const int headerSize = Spaghettis()->getLookAndFeel().getPropertyPanelHeight() + 6;
    const int i = panel_.getNumPanels();
    auto h = std::make_unique<Header> (p->getName(), i, this);
    
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

std::unique_ptr<juce::PropertyComponent> createPropertyComponent (const core::Parameter& p)
{
    if (p.isBoolean())      { return std::make_unique<Parameters::Boolean> (p); }
    if (p.isColour())       { return std::make_unique<Parameters::Colour> (p);  }
    else if (p.isInteger()) { return std::make_unique<Parameters::Integer> (p); }
    else if (p.isFloat())   {
        if (p.hasRange())   {
            return std::make_unique<Parameters::Slider> (p);
        } else {
            return std::make_unique<Parameters::Float> (p);
        }
    } else {
        return std::make_unique<Parameters::Text> (p);
    }
}

void buildConcertinaPanelParameter (const core::Parameter& p, juce::Array<juce::PropertyComponent*>& c)
{
    std::unique_ptr<juce::PropertyComponent> t (createPropertyComponent (p));
    
    t->setPreferredHeight (Spaghettis()->getLookAndFeel().getPropertyPanelHeight());
    t->setTooltip (p.getInfo());
    
    c.add (t.release());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameters::View::buildConcertinaPanel (const core::Tree& tree, View& v)
{
    for (const auto& group : tree) {
    //
    if (group.isHidden() == false) {
    //
    auto panel = std::make_unique<juce::PropertyPanel> (group.getName());
    
    juce::Array<juce::PropertyComponent*> components;
    
    for (const auto& parameter : group) {
        buildConcertinaPanelParameter (parameter, components);
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

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
