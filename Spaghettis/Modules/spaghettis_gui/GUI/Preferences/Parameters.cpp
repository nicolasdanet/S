
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
    juce::var t (v);
        
    return t;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
