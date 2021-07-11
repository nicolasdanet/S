
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::buildConcertinaPanel (PreferencesComponent& c)
{
    for (const auto& s : tree_) {
    //
    auto panel = std::make_unique<juce::PropertyPanel> (s.getProperty (Ids::name).toString());
    
    //for (const auto& p : s) { }
    
    c.addPanel (panel.release());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Preferences::getDefault()
{
    juce::ValueTree tree { Ids::PREFERENCES, {}, {
    //
    { Ids::GROUP, {{ Ids::name, "Editing" }},
        {
            { Ids::PARAMETER, {
                { Ids::item,  "snapToGrid" },
                { Ids::text,  NEEDS_TRANS ("Snap to grid") },
                { Ids::type,  "boolean" },
                { Ids::value, true }
            }},
            { Ids::PARAMETER, {
                { Ids::item, "gridSize" },
                { Ids::text,  NEEDS_TRANS ("Grid size") },
                { Ids::type, "integer" },
                { Ids::value, 12 }
            }}
        }
    },
    { Ids::GROUP, {{ Ids::name, "Fonts" }},
        {
            { Ids::PARAMETER, {
                { Ids::item, "defaultFontSize" },
                { Ids::text,  NEEDS_TRANS ("Default font size") },
                { Ids::type, "integer" },
                { Ids::value, 12 }
            }}
        }
    }
    //
    }};

    return tree;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Preferences::isValidSection (const juce::ValueTree& tree)
{
    return (tree.hasType (Ids::GROUP) && tree.hasProperty (Ids::name));
}

bool Preferences::isValidParameter (const juce::ValueTree& tree)
{
    return (tree.hasType (Ids::PARAMETER)
                && tree.hasProperty (Ids::item)
                && tree.hasProperty (Ids::text)
                && tree.hasProperty (Ids::type)
                && tree.hasProperty (Ids::value));
}

bool Preferences::isValidTree (const juce::ValueTree& tree)
{
    if (tree.isValid() && tree.hasType (Ids::PREFERENCES)) {
    //
    for (const auto& section : tree) {
    //
    if (isValidSection (section)) {
        for (const auto& parameter : section)  {
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

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
