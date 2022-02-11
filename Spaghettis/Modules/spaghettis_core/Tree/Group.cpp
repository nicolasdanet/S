
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter addParameter (juce::ValueTree& group,
    const juce::String& key,
    const juce::String& type,
    const juce::String& text,
    const juce::String& info,
    juce::var v)
{
    juce::ValueTree parameter (Ids::PARAMETER);
    
    Prototype::setProperties (parameter, key, type, text, info);
    
    parameter.setProperty (Ids::value, v, nullptr);
    
    group.appendChild (parameter, nullptr);
    
    return Parameter (parameter);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter Group::addBoolean (const juce::String& key,
    bool b,
    const juce::String& text,
    const juce::String& info)
{
    jassert (!hasParameter (key));
    
    return addParameter (group_, key, "boolean", text, info, b);
}

Parameter Group::addColour (const juce::String& key,
    juce::Colour c,
    const juce::String& text,
    const juce::String& info)
{
    jassert (!hasParameter (key));
        
    return addParameter (group_, key, "color", text, info, Colours::getColourAsString (c));
}

Parameter Group::addInteger (const juce::String& key,
    int n,
    const juce::String& text,
    const juce::String& info)
{
    jassert (!hasParameter (key));
    
    return addParameter (group_, key, "integer", text, info, n);
}

Parameter Group::addFloat (const juce::String& key,
    double f,
    const juce::String& text,
    const juce::String& info)
{
    jassert (!hasParameter (key));
    
    return addParameter (group_, key, "float", text, info, f);
}

Parameter Group::addText (const juce::String& key,
    const juce::String& s,
    const juce::String& text,
    const juce::String& info)
{
    jassert (!hasParameter (key));
    
    return addParameter (group_, key, "text", text, info, s);
}

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
