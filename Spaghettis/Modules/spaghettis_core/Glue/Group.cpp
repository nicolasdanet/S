
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
    const juce::String& type,
    juce::var v)
{
    juce::ValueTree parameter (Ids::PARAMETER);
    
    parameter.setProperty (Ids::key,   key,  nullptr);
    // parameter.setProperty (Ids::text,  text, nullptr);
    // parameter.setProperty (Ids::info,  info, nullptr);
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

Parameter Group::addParameter (const juce::String& key, bool b)
{
    return addParameterWithType (group_, key, "boolean", b);
}

Parameter Group::addParameter (const juce::String& key, juce::Colour c)
{
    return addParameterWithType (group_, key, "color", Colours::getColourAsString (c));
}

Parameter Group::addParameter (const juce::String& key, int n)
{
    return addParameterWithType (group_, key, "integer", n);
}

Parameter Group::addParameter (const juce::String& key, double f)
{
    return addParameterWithType (group_, key, "float", f);
}

Parameter Group::addParameter (const juce::String& key, const juce::String& s)
{
    return addParameterWithType (group_, key, "text", s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool Group::isValid() const
{
    if (!group_.isValid()) { return false; }
    else if (!group_.hasType (Ids::GROUP)) { return false; }
    else if (!group_.getProperty (Ids::name).isString()) { return false; }
    
    for (const auto& parameter : group_) {
        if (!Parameter (parameter).isValid()) { return false; }
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
