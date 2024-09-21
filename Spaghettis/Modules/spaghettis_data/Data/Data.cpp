
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Data::hasParameter (const juce::String& group, const juce::String& key) const
{
    if (getGroup (group).hasParameter (key)) { return true; }
    
    return false;
}

Parameter Data::getParameter (const juce::String& group, const juce::String& key) const
{
    jassert (hasParameter (group, key)); return getGroup (group).getParameter (key);
}

void Data::changeValue (const juce::String& group, const juce::String& key, const juce::var& v)
{
    if (hasParameter (group, key)) { getParameter (group, key).changeValue (v); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Data::addGroup (const juce::String& name, bool isHidden)
{
    jassert (!hasGroup (name));
    
    juce::ValueTree group (Id::GROUP);
    
    group.setProperty (Id::name, name, nullptr);
    group.setProperty (Id::hidden, isHidden, nullptr);
    
    tree_.appendChild (group, nullptr);
        
    return Group (group);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Data::hasGroup (const juce::String& name) const
{
    for (const auto& group : *this) { if (group.getName() == name) { return true; } }
    
    return false;
}

Group Data::getGroup (const juce::String& name) const
{
    for (const auto& group : *this) { if (group.getName() == name) { return group; } }
    
    return Group();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
