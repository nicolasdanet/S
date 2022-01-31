
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::unique_ptr<juce::PropertyComponent> Parameters::Base::createPropertyComponent() const
{
    if (type_ == "boolean")      { return std::make_unique<Parameters::Boolean> (p_); }
    if (type_ == "color")        { return std::make_unique<Parameters::Colour> (p_);  }
    else if (type_ == "integer") { return std::make_unique<Parameters::Integer> (p_, *this); }
    else if (type_ == "float")   {
        if (hasRange()) {
            return std::make_unique<Parameters::Slider> (p_, *this);
        } else {
            return std::make_unique<Parameters::Float> (p_, *this);
        }
    } else {
        return std::make_unique<Parameters::Text> (p_);
    }
}

juce::var Parameters::Base::constrained (const juce::var& v) const
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

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
