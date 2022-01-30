
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct Parameters {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static const juce::var getValue (const juce::ValueTree& tree, const juce::String& item)
{
    for (const auto& group : tree) {
    for (const auto& parameter : group) {
        if (parameter.getProperty (Ids::item).equalsWithSameType (item)) {
            return parameter.getProperty (Ids::value);
        }
    }
    }
    
    return juce::var();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static bool isValidSection (const juce::ValueTree& group)
{
    return (group.hasType (Ids::GROUP) && group.getProperty (Ids::name).isString());
}

static bool isValidParameter (const juce::ValueTree& parameter)
{
    return (parameter.hasType (Ids::PARAMETER)
                && parameter.getProperty (Ids::item).isString()
                && parameter.getProperty (Ids::text).isString()
                && parameter.getProperty (Ids::info).isString()
                && parameter.getProperty (Ids::type).isString()
                && parameter.hasProperty (Ids::value));
}

static bool isValidTree (const juce::ValueTree& tree, const juce::Identifier& identifier)
{
    if (tree.isValid() && tree.hasType (identifier)) {
    //
    for (const auto& group : tree) {
    //
    if (isValidSection (group)) {
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

static bool isValidPreferences (const juce::ValueTree& tree)
{
    return isValidTree (tree, Ids::PREFERENCES);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
