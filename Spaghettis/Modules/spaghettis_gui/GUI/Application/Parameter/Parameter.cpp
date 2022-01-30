
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::unique_ptr<juce::PropertyComponent> Parameter::Base::createPropertyComponent() const
{
    if (type_ == "boolean")      { return std::make_unique<Parameter::Boolean> (p_); }
    if (type_ == "color")        { return std::make_unique<Parameter::Colour> (p_);  }
    else if (type_ == "integer") { return std::make_unique<Parameter::Integer> (p_, *this); }
    else if (type_ == "float")   {
        if (hasRange()) {
            return std::make_unique<Parameter::Slider> (p_, *this);
        } else {
            return std::make_unique<Parameter::Float> (p_, *this);
        }
    } else {
        return std::make_unique<Parameter::Text> (p_);
    }
}

juce::var Parameter::Base::constrained (const juce::var& v) const
{
    if (type_ == "boolean") {
        bool b = static_cast<bool> (v);
        return juce::var (b);
    } else if (type_ == "color") {
        return juce::var (LookAndFeel::getColorFromString (v.toString()).toString());
    } else if (type_ == "integer" && hasRange()) {
        int i = juce::Range<int> (*this).clipValue (static_cast<int> (v));
        return juce::var (juce::String (i));
    } else if (type_ == "float" && hasRange()) {
        double f = juce::Range<double> (*this).clipValue (static_cast<double> (v));
        return juce::var (juce::String (f));
    }
    
    return juce::var (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::unique_ptr<juce::PropertyComponent> buildConcertinaPanelParameterGet (juce::ValueTree parameter)
{
    return Parameter::Base (parameter).createPropertyComponent();
}

void buildConcertinaPanelParameter (juce::ValueTree parameter, juce::Array<juce::PropertyComponent*>& c)
{
    std::unique_ptr<juce::PropertyComponent> p (buildConcertinaPanelParameterGet (parameter));
    
    p->setPreferredHeight (Spaghettis()->getLookAndFeel().getPropertyPanelHeight());
    p->setTooltip (parameter.getProperty (Ids::info).toString());
    
    c.add (p.release());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::buildConcertinaPanel (const juce::ValueTree& tree, Model& m)
{
    for (const auto& group : tree) {
    //
    auto panel = std::make_unique<juce::PropertyPanel> (group.getProperty (Ids::name).toString());
    
    {
        juce::Array<juce::PropertyComponent*> components;
    
        for (const auto& parameter : group) { buildConcertinaPanelParameter (parameter, components); }
    
        panel->addProperties (components);
    }
    
    m.addPanel (panel.release());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
