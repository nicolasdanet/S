
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

bool Parameter::isBoolean() const
{
    return (getType() == "boolean");
}

bool Parameter::isColour() const
{
    return (getType() == "color");
}

bool Parameter::isInteger() const
{
    return (getType() == "integer");
}

bool Parameter::isFloat() const
{
    return (getType() == "float");
}

bool Parameter::isText() const
{
    return (getType() == "text");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String Parameter::getKey() const
{
    return get (Ids::key).toString();
}

juce::String Parameter::getType() const
{
    return get (Ids::type).toString();
}

juce::String Parameter::getLabel() const
{
    return get (Ids::label).toString();
}

juce::String Parameter::getInfo() const
{
    return get (Ids::info).toString();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::setValue (const juce::var& v)
{
    if (!parameter_.getProperty (Ids::value).equals (v)) {
    //
    parameter_.setProperty (Ids::value, constrained (v), nullptr);
    //
    }
}

juce::var Parameter::getValue() const
{
    return parameter_.getProperty (Ids::value);
}

/* Make a temporary copy of the ValueTree as a workaround to get that method const. */

juce::Value Parameter::getSource() const
{
    return juce::ValueTree (parameter_).getPropertyAsValue (Ids::value, nullptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Parameter::hasRange() const
{
    return (!get (Ids::minimum).isVoid() && !get (Ids::maximum).isVoid());
}

double Parameter::getMinimumAsDouble() const
{
    double m = static_cast<double> (get (Ids::minimum));
    double n = static_cast<double> (get (Ids::maximum));
    
    return juce::jmin (m, n);
}

double Parameter::getMaximumAsDouble() const
{
    double m = static_cast<double> (get (Ids::minimum));
    double n = static_cast<double> (get (Ids::maximum));
    
    return juce::jmax (m, n);
}

double Parameter::getStep() const
{
    return 0.001;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const juce::var& Parameter::get (const juce::Identifier& identifier) const
{
    if (parameter_.hasProperty (identifier) == false) {
    //
    juce::ValueTree prototype (parameter_.getChildWithName (Ids::PROTOTYPE));
    
    if (prototype.isValid()) { return prototype.getProperty (identifier); }
    //
    }
    
    return parameter_.getProperty (identifier);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::var Parameter::constrained (const juce::var& v) const
{
    if (isBoolean()) {
        bool b = static_cast<bool> (v);
        return juce::var (b);
    } else if (isColour()) {
        return juce::var (core::Colours::getColourFromString (v.toString()).toString());
    } else if (isInteger() && hasRange()) {
        int i = juce::Range<int> (*this).clipValue (static_cast<int> (v));
        return juce::var (juce::String (i));
    } else if (isFloat() && hasRange()) {
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
    else if (!parameter_.hasType (Ids::PARAMETER)) { return false; }

    return (get (Ids::key).isString() && get (Ids::type).isString() && !getValue().isVoid());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
