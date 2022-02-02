
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter addParameterWithType (juce::ValueTree& group,
    const juce::String& key,
    const juce::String& text,
    const juce::String& info,
    const juce::String& type,
    juce::var v)
{
    juce::ValueTree parameter (Ids::PARAMETER);
    
    parameter.setProperty (Ids::key,   key,  nullptr);
    parameter.setProperty (Ids::text,  text, nullptr);
    parameter.setProperty (Ids::info,  info, nullptr);
    parameter.setProperty (Ids::type,  type, nullptr);
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

Parameter Group::addParameter (const juce::String& key, const juce::String& text, const juce::String& info, bool b)
{
    return addParameterWithType (group_, key, text, info, "boolean", b);
}

Parameter Group::addParameter (const juce::String& key, const juce::String& text, const juce::String& info, juce::Colour c)
{
    return addParameterWithType (group_, key, text, info, "color", Colours::getColourAsString (c));
}

Parameter Group::addParameter (const juce::String& key, const juce::String& text, const juce::String& info, int n)
{
    return addParameterWithType (group_, key, text, info, "integer", n);
}

Parameter Group::addParameter (const juce::String& key, const juce::String& text, const juce::String& info, double f)
{
    return addParameterWithType (group_, key, text, info, "float", f);
}

Parameter Group::addParameter (const juce::String& key, const juce::String& text, const juce::String& info, const juce::String& s)
{
    return addParameterWithType (group_, key, text, info, "text", s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
