
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
    return get (Id::key).toString();
}

juce::String Parameter::getType() const
{
    return get (Id::type).toString();
}

juce::String Parameter::getLabel() const
{
    return get (Id::label).toString();
}

juce::String Parameter::getInfo() const
{
    return get (Id::info).toString();
}

juce::var Parameter::getValue() const
{
    return get (Id::value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Value Parameter::getValueAsValue (bool updateSynchronously) const
{
    return filtered (getSource (Id::value, updateSynchronously));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parameter::changeValue (const juce::var& v)
{
    change (Id::value, forceRange (forceType (v)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter& Parameter::setHidden (bool isHidden, ParameterScope scope)
{
    set (Id::hidden, isHidden, scope); return *this;
}

Parameter& Parameter::setEditable (bool isEditable, ParameterScope scope)
{
    set (Id::editable, isEditable, scope); return *this;
}

bool Parameter::isHidden() const
{
    const juce::var v (get (Id::hidden));
    
    if (v.isBool()) { return static_cast<bool> (v); }
    
    return false;   /* Default is false. */
}

bool Parameter::isEditable() const
{
    const juce::var v (get (Id::editable));
    
    if (v.isBool()) { return static_cast<bool> (v); }
    
    return true;    /* Default is true. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Parameter::hasRange() const
{
    return (!get (Id::minimum).isVoid() && !get (Id::maximum).isVoid());
}

double Parameter::getMinimumAsDouble() const
{
    double m = static_cast<double> (get (Id::minimum));
    double n = static_cast<double> (get (Id::maximum));
    
    return juce::jmin (m, n);
}

double Parameter::getMaximumAsDouble() const
{
    double m = static_cast<double> (get (Id::minimum));
    double n = static_cast<double> (get (Id::maximum));
    
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

juce::ValueTree baseDelegate (const juce::ValueTree& tree)
{
    auto p = dynamic_cast<DelegateShared*> (tree.getProperty (Id::DELEGATE).getObject());
    
    jassert (p);
    
    return p->getValueTree();
}

juce::ValueTree baseForGetters (const juce::ValueTree& tree, const juce::Identifier& i)
{
    /* First search locally then in delegate/prototype. */
    
    if (!tree.hasProperty (i)) { return baseDelegate (tree); }
    else {
        return tree;
    }
}

juce::ValueTree baseForSetters (const juce::ValueTree& tree, const juce::Identifier& i, ParameterScope scope)
{
    if (scope == ParameterScope::delegate) { return baseDelegate (tree); }
    else {
        return tree;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Value Parameter::getSource (const juce::Identifier& identifier, bool updateSynchronously) const
{
    juce::ValueTree t = baseForGetters (parameter_, identifier);
    
    return t.getPropertyAsValue (identifier, nullptr, updateSynchronously);
}

const juce::var& Parameter::get (const juce::Identifier& identifier) const
{
    juce::ValueTree t = baseForGetters (parameter_, identifier);
    
    return t.getProperty (identifier);
}

void Parameter::change (const juce::Identifier& identifier, const juce::var& v)
{
    juce::ValueTree t = baseForGetters (parameter_, identifier);
    
    if (t.hasProperty (identifier) && !t.getProperty (identifier).equals (v)) {
    //
    jassert (t.getProperty (identifier).hasSameTypeAs (v));
    
    t.setProperty (identifier, v, nullptr);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void Parameter::set (const juce::Identifier& identifier, const juce::var& v, ParameterScope scope)
{
    juce::ValueTree t = baseForSetters (parameter_, identifier, scope);
    
    t.setProperty (identifier, v, nullptr);
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
