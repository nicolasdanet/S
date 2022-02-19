
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
    return (getType() == ParameterType<bool>::get());
}

bool Parameter::isColour() const
{
    return (getType() == ParameterType<juce::Colour>::get());
}

bool Parameter::isInteger() const
{
    return (getType() == ParameterType<int>::get());
}

bool Parameter::isFloat() const
{
    return (getType() == ParameterType<double>::get());
}

bool Parameter::isText() const
{
    return (getType() == ParameterType<juce::String>::get());
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

juce::var Parameter::getValue() const
{
    return get (Ids::value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Value Parameter::getValueSource() const
{
    return getSource (Ids::value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::changeValue (const juce::var& v)
{
    change (Ids::value, v);
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::ValueTree getBase (const juce::ValueTree& tree, const juce::Identifier& identifier)
{
    if (!tree.hasProperty (identifier)) {
    //
    auto p = dynamic_cast<Delegate::Shared*> (tree.getProperty (Ids::DELEGATE).getObject());
    
    if (p) {
        return p->getTree();
    }
    //
    }
    
    return tree;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Value Parameter::getSource (const juce::Identifier& identifier) const
{
    return getBase (parameter_, identifier).getPropertyAsValue (identifier, nullptr);
}

const juce::var& Parameter::get (const juce::Identifier& identifier) const
{
    return getBase (parameter_, identifier).getProperty (identifier);
}

void Parameter::set (const juce::Identifier& identifier, const juce::var& v)
{
    getBase (parameter_, identifier).setProperty (identifier, v, nullptr);
}

void Parameter::change (const juce::Identifier& identifier, const juce::var& v)
{
    juce::ValueTree t (getBase (parameter_, identifier));
    
    if (t.hasProperty (identifier) && !t.getProperty (identifier).equals (v)) {
        t.setProperty (identifier, constrained (v), nullptr);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
