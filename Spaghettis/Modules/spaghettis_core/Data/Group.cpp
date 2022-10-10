
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

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

Parameter Group::add (DelegateManager* p, const Invariant& i, juce::var v)
{
    jassert (!hasParameter (i.key));
    
    juce::ValueTree parameter (Ids::PARAMETER);
    
    parameter.setProperty (Ids::DELEGATE, (p ? p->getOrCreate (i) : new DelegateShared (i)), nullptr);
    parameter.setProperty (Ids::value, v, nullptr);
    
    group_.appendChild (parameter, nullptr);
    
    return Parameter (parameter);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Group::hasParameter (const juce::String& key) const
{
    for (const auto& parameter : *this) { if (parameter.getKey() == key) { return true; } }
    
    return false;
}

Parameter Group::getParameter (const juce::String& key) const
{
    for (const auto& parameter : *this) { if (parameter.getKey() == key) { return parameter; } }
    
    return Parameter();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Group::getFromParameter (const Parameter& p)
{
    return Group (p.parameter_.getParent());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
