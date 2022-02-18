
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
    if (tree.hasProperty (identifier) == false) {
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::set (const juce::Identifier& identifier, const juce::var& v)
{
    juce::ValueTree t (getBase (parameter_, identifier));
    
    if (!t.hasProperty (identifier)) { t.setProperty (identifier, v, nullptr); }
    else if (!t.getProperty (identifier).equals (v)) {
        parameter_.setProperty (identifier, constrained (v), nullptr);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::substitute (juce::ValueTree& tree)
{
    if (tree.hasType (Ids::PARAMETER) && tree.hasProperty (Ids::DELEGATE)) {
    //
    auto p = dynamic_cast<Delegate::Shared*> (tree.getProperty (Ids::DELEGATE).getObject());
    
    if (p) { tree.setProperty (Ids::key, p->getTree().getProperty (Ids::key), nullptr); }
    
    tree.removeProperty (Ids::DELEGATE, nullptr);
    //
    }
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

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
