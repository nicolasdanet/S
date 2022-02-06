
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::changeValue (const juce::var& v)
{
    if (!getValue().equals (v)) { setValue (constrained (v)); }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String Parameter::getKey() const
{
    return parameter_.getProperty (Ids::key).toString();
}

juce::String Parameter::getType() const
{
    return parameter_.getProperty (Ids::type).toString();
}

juce::String Parameter::getText() const
{
    return parameter_.getProperty (Ids::text).toString();
}

juce::String Parameter::getInfo() const
{
    return parameter_.getProperty (Ids::info).toString();
}

juce::var Parameter::getValue() const
{
    return parameter_.getProperty (Ids::value);
}

juce::Value Parameter::getValueAsValue()
{
    return parameter_.getPropertyAsValue (Ids::value, nullptr);
}
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter& Parameter::setText (const juce::String& s)
{
    parameter_.setProperty (Ids::text, s, nullptr);
    
    return *this;
}

Parameter& Parameter::setInfo (const juce::String& s)
{
    parameter_.setProperty (Ids::info, s, nullptr);
    
    return *this;
}

Parameter& Parameter::setValue (juce::var v)
{
    parameter_.setProperty (Ids::value, v, nullptr);
    
    return *this;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Parameter::hasRange() const
{
    return (parameter_.hasProperty (Ids::minimum) && parameter_.hasProperty (Ids::maximum));
}

double Parameter::getMinimumAsDouble() const
{
    double m = static_cast<double> (parameter_.getProperty (Ids::minimum));
    double n = static_cast<double> (parameter_.getProperty (Ids::maximum));
    
    return juce::jmin (m, n);
}

double Parameter::getMaximumAsDouble() const
{
    double m = static_cast<double> (parameter_.getProperty (Ids::minimum));
    double n = static_cast<double> (parameter_.getProperty (Ids::maximum));
    
    return juce::jmax (m, n);
}

double Parameter::getStep() const
{
    return 0.001;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::var Parameter::constrained (const juce::var& v) const
{
    const juce::String type = getType();
    
    if (type == "boolean") {
        bool b = static_cast<bool> (v);
        return juce::var (b);
    } else if (type == "color") {
        return juce::var (core::Colours::getColourFromString (v.toString()).toString());
    } else if (type == "integer" && hasRange()) {
        int i = juce::Range<int> (*this).clipValue (static_cast<int> (v));
        return juce::var (juce::String (i));
    } else if (type == "float" && hasRange()) {
        double f = juce::Range<double> (*this).clipValue (static_cast<double> (v));
        return juce::var (juce::String (f));
    }
    
    return juce::var (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Parameter::isValid() const
{
    if (!parameter_.isValid()) { return false; }
    
    return (parameter_.hasType (Ids::PARAMETER)
                && parameter_.getProperty (Ids::key).isString()
                && parameter_.getProperty (Ids::text).isString()
                && parameter_.getProperty (Ids::info).isString()
                && parameter_.getProperty (Ids::type).isString()
                && parameter_.hasProperty (Ids::value));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
