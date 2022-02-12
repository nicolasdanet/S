
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

juce::String Group::getName() const
{
    return group_.getProperty (Ids::name).toString();
}

bool Group::isHidden() const
{
    return static_cast<bool> (group_.getProperty (Ids::hidden));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter Group::add (const juce::String& key,
    const juce::String& type,
    const juce::String& label,
    const juce::String& info,
    juce::var v)
{
    juce::ValueTree parameter (Ids::PARAMETER);
    
    parameter.setProperty (Ids::key, key, nullptr);
    parameter.setProperty (Ids::type, type, nullptr);
    parameter.setProperty (Ids::label, label, nullptr);
    parameter.setProperty (Ids::info, info, nullptr);
    parameter.setProperty (Ids::value, v, nullptr);
    
    jassert (!hasParameter (key));
    
    group_.appendChild (parameter, nullptr);
    
    return Parameter (parameter);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/*
Parameter Group::addBoolean (const juce::String& key,
    const juce::String& label,
    const juce::String& info,
    bool b)
{
    return addParameter (key, "boolean", label, info, b);
}

Parameter Group::addColour (const juce::String& key,
    const juce::String& label,
    const juce::String& info,
    juce::Colour c)
{
    return addParameter (key, "color", label, info, Colours::getColourAsString (c));
}

Parameter Group::addInteger (const juce::String& key,
    const juce::String& label,
    const juce::String& info,
    int n)
{
    return addParameter (key, "integer", label, info, n);
}

Parameter Group::addFloat (const juce::String& key,
    const juce::String& label,
    const juce::String& info,
    double f)
{
    return addParameter (key, "float", label, info, f);
}

Parameter Group::addText (const juce::String& key,
    const juce::String& label,
    const juce::String& info,
    const juce::String& s)
{
    return addParameter (key, "text", label, info, s);
}
*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Group::hasParameter (const juce::String& key) const
{
    return getParameter (key).isValid();
}

Parameter Group::getParameter (const juce::String& key) const
{
    for (const auto& parameter : *this) { if (parameter.getKey() == key) { return parameter; } }
    
    return Parameter();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Group::isValid() const
{
    if (!group_.isValid()) { return false; }
    else if (!group_.hasType (Ids::GROUP)) { return false; }
    else if (!group_.getProperty (Ids::name).isString()) { return false; }
    
    for (const auto& parameter : *this) {
        if (!parameter.isValid()) { return false; }
    }
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
