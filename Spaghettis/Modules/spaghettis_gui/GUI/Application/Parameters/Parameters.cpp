
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::unique_ptr<juce::PropertyComponent> Parameters::Base::createPropertyComponent()
{
    const juce::String type = p_.getType();
    
    if (type == "boolean")      { return std::make_unique<Parameters::Boolean> (p_); }
    if (type == "color")        { return std::make_unique<Parameters::Colour> (p_);  }
    else if (type == "integer") { return std::make_unique<Parameters::Integer> (p_); }
    else if (type == "float")   {
        if (p_.hasRange()) {
            return std::make_unique<Parameters::Slider> (p_);
        } else {
            return std::make_unique<Parameters::Float> (p_);
        }
    } else {
        return std::make_unique<Parameters::Text> (p_);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
