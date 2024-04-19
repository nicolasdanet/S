
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String Group::getName() const
{
    return group_.getProperty (Id::name).toString();
}

bool Group::isHidden() const
{
    return static_cast<bool> (group_.getProperty (Id::hidden));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parameter Group::add (DelegateManager* p, const Invariant& i, juce::var v)
{
    jassert (!hasParameter (i.key));
    
    juce::ValueTree parameter (Id::PARAMETER);
    
    parameter.setProperty (Id::DELEGATE, (p ? p->getOrCreate (i) : new Delegate (i)), nullptr);
    parameter.setProperty (Id::value, v, nullptr);
    
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

Group Group::getFromParameter (const Parameter& parameter)
{
    const juce::ValueTree t (parameter.parameter_.getParent());
    
    jassert (t.isValid() && t.hasType (Id::GROUP));
    
    return Group (t);
}

Group Group::makeCopy (const Group& group)
{
    return Group (group.group_.createCopy());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
