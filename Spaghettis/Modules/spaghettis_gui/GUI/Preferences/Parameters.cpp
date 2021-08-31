
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::PropertyComponent* Parameters::Base::createPropertyComponent() const
{
    if (type_ == "boolean")      { return new Parameters::Boolean (p_); }
    if (type_ == "color")        { return new Parameters::Colour (p_);  }
    else if (type_ == "integer") { return new Parameters::Integer (p_, *this); }
    else if (type_ == "float")   {
        if (hasRange()) {
            return new Parameters::Slider (p_, *this);
        } else {
            return new Parameters::Float (p_, *this);
        }
    } else {
        return new Parameters::Text (p_);
    }
}

juce::var Parameters::Base::constrained (const juce::var& v) const
{
    if (type_ == "integer" && hasRange()) {
        return juce::var (juce::Range<int> (*this).clipValue (static_cast<int> (v)));
    } else if (type_ == "float" && hasRange()) {
        return juce::var (juce::Range<double> (*this).clipValue (static_cast<double> (v)));
    } else if (type_ == "color") {
        return juce::var (LookAndFeel::getColorFromString (v.toString()).toString());
    }
    
    return juce::var (v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
