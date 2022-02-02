
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
    const juce::String& item,
    const juce::String& text,
    const juce::String& info,
    const juce::String& type,
    juce::var v)
{
    juce::ValueTree parameter (Ids::PARAMETER);
    
    parameter.setProperty (Ids::item,  item, nullptr);
    parameter.setProperty (Ids::text,  text, nullptr);
    parameter.setProperty (Ids::info,  info, nullptr);
    parameter.setProperty (Ids::type,  type, nullptr);
    parameter.setProperty (Ids::value, v, nullptr);
    
    group.appendChild (parameter, nullptr);
    
    return Parameter (parameter);
}

/*

juce::ValueTree getParameter (const juce::ValueTree& group, const juce::String& item)
{
    for (const auto& parameter : group) {
        if (parameter.getProperty (Ids::item).equalsWithSameType (item)) { return parameter; }
    }
    
    return juce::ValueTree();
}

juce::ValueTree getGroup (const juce::ValueTree& tree, const juce::String& name)
{
    for (const auto& group : tree) {
        if (group.getProperty (Ids::name).equalsWithSameType (name)) { return group; }
    }
        
    return juce::ValueTree();
}
    
*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter Tree::addParameter (juce::ValueTree& group,
    const juce::String& item,
    const juce::String& text,
    const juce::String& info,
    bool b)
{
    return addParameterWithType (group, item, text, info, "boolean", b);
}

Parameter Tree::addParameter (juce::ValueTree& group,
    const juce::String& item,
    const juce::String& text,
    const juce::String& info,
    juce::Colour c)
{
    return addParameterWithType (group, item, text, info, "color", Colours::getColourAsString (c));
}

Parameter Tree::addParameter (juce::ValueTree& group,
    const juce::String& item,
    const juce::String& text,
    const juce::String& info,
    int n)
{
    return addParameterWithType (group, item, text, info, "integer", n);
}

Parameter Tree::addParameter (juce::ValueTree& group,
    const juce::String& item,
    const juce::String& text,
    const juce::String& info,
    double f)
{
    return addParameterWithType (group, item, text, info, "float", f);
}

Parameter Tree::addParameter (juce::ValueTree& group,
    const juce::String& item,
    const juce::String& text,
    const juce::String& info,
    const juce::String& s)
{
    return addParameterWithType (group, item, text, info, "text", s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Tree::addGroup (juce::ValueTree& tree, const juce::String& name)
{
    juce::ValueTree group (Ids::GROUP); group.setProperty (Ids::name, name, nullptr);
        
    tree.appendChild (group, nullptr);
        
    return group;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool isValidParameter (const juce::ValueTree& parameter)
{
    return (parameter.hasType (Ids::PARAMETER)
                && parameter.getProperty (Ids::item).isString()
                && parameter.getProperty (Ids::text).isString()
                && parameter.getProperty (Ids::info).isString()
                && parameter.getProperty (Ids::type).isString()
                && parameter.hasProperty (Ids::value));
}

bool isValidGroup (const juce::ValueTree& group)
{
    return (group.hasType (Ids::GROUP) && group.getProperty (Ids::name).isString());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Tree::isValid (const juce::ValueTree& tree, const juce::Identifier& identifier)
{
    if (tree.isValid() && tree.hasType (identifier)) {
    //
    for (const auto& group : tree) {
    //
    if (isValidGroup (group)) {
        for (const auto& parameter : group)  {
            if (!isValidParameter (parameter)) { return false; }
        }
    } else { return false; }
    //
    }
    //
    } else { return false; }
    
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
