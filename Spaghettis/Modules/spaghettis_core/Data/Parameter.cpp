
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

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

bool Parameter::isRectangle() const
{
    return (getType() == ParameterType<juce::Rectangle<int>>::get());
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

juce::Value Parameter::getValueAsValue (bool updateSynchronously) const
{
    return filtered (getSource (Ids::value, updateSynchronously));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::changeValue (const juce::var& v)
{
    change (Ids::value, forceRange (forceType (v)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter& Parameter::setHidden (bool isHidden)
{
    set (Ids::hidden, isHidden); return *this;
}
    
bool Parameter::isHidden() const
{
    const juce::var v (get (Ids::hidden));
    
    if (v.isBool()) { return static_cast<bool> (v); }
    
    return false;
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
    auto p = dynamic_cast<DelegateShared*> (tree.getProperty (Ids::DELEGATE).getObject());
    
    if (p) {
        return p->getValueTree();
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

juce::Value Parameter::getSource (const juce::Identifier& identifier, bool updateSynchronously) const
{
    return getBase (parameter_, identifier).getPropertyAsValue (identifier, nullptr, updateSynchronously);
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
    //
    jassert (t.getProperty (identifier).hasSameTypeAs (v));
    
    t.setProperty (identifier, v, nullptr);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::var Parameter::forceType (const juce::var& v) const
{
    if (isBoolean())        { return Cast::force<bool> (v);   }
    else if (isInteger())   { return Cast::force<int> (v);    }
    else if (isFloat())     { return Cast::force<double> (v); }
    else if (isColour())    { return Cast::force<juce::Colour> (v); }
    else if (isRectangle()) { return Cast::force<juce::Rectangle<int>> (v); }
    else {
        return Cast::force<juce::String> (v);
    }
}

juce::var Parameter::forceRange (const juce::var& v) const
{
    if (hasRange())  {
    //
    if (isInteger()) { return juce::var (juce::Range<int> (*this).clipValue (static_cast<int> (v))); }
    else if (isFloat()) {
        return juce::var (juce::Range<double> (*this).clipValue (static_cast<double> (v)));
    }
    //
    }
    
    return v;
}

juce::Value Parameter::filtered (const juce::Value& v) const
{
    if (isBoolean())        { return core::Filter<bool>::make (v);   }
    else if (isInteger())   { return core::Filter<int>::make (v);    }
    else if (isFloat())     { return core::Filter<double>::make (v); }
    else if (isRectangle()) { return core::Filter<juce::Rectangle<int>>::make (v); }
    else {
        return core::Filter<juce::String>::make (v);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
