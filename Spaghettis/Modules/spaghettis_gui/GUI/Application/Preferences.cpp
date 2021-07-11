
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::PropertyComponent* buildConcertinaPanelParametersGet (const juce::ValueTree& parameter)
{
    juce::String text = parameter.getProperty (Ids::text).toString();
    juce::String type = parameter.getProperty (Ids::type).toString();
    
    if (type == "boolean") {
        return new juce::BooleanPropertyComponent (juce::Value (true), text, "");
    } else {
        return new juce::TextPropertyComponent (juce::Value (juce::var ("Toto")), text, 200, false);
    }
}

void buildConcertinaPanelParameters (const juce::ValueTree& parameter,
    juce::Array<juce::PropertyComponent*>& components)
{
    std::unique_ptr<juce::PropertyComponent> p (buildConcertinaPanelParametersGet (parameter));
    
    components.add (p.release());
}

void buildConcertinaPanelGroup (const juce::ValueTree& group, juce::PropertyPanel& panel)
{
    juce::Array<juce::PropertyComponent*> components;
    
    for (const auto& parameter : group) { buildConcertinaPanelParameters (parameter, components); }
    
    panel.addProperties (components);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::buildConcertinaPanel (PreferencesComponent& c)
{
    for (const auto& group : tree_) {
    //
    auto panel = std::make_unique<juce::PropertyPanel> (group.getProperty (Ids::name).toString());
    
    buildConcertinaPanelGroup (group, *panel);
    
    c.addPanel (panel.release());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool isValidSection (const juce::ValueTree& tree)
{
    return (tree.hasType (Ids::GROUP) && tree.hasProperty (Ids::name));
}

bool isValidParameter (const juce::ValueTree& tree)
{
    return (tree.hasType (Ids::PARAMETER)
                && tree.hasProperty (Ids::item)
                && tree.hasProperty (Ids::text)
                && tree.hasProperty (Ids::type)
                && tree.hasProperty (Ids::value));
}

bool isValidTree (const juce::ValueTree& tree)
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
                { Ids::item,  "SnapToGrid" },
                { Ids::text,  NEEDS_TRANS ("Snap to Grid") },
                { Ids::type,  "boolean" },
                { Ids::value, true }
            }},
            { Ids::PARAMETER, {
                { Ids::item, "GridSize" },
                { Ids::text,  NEEDS_TRANS ("Grid Size") },
                { Ids::type, "integer" },
                { Ids::value, 12 }
            }}
        }
    },
    { Ids::GROUP, {{ Ids::name, "Fonts" }},
        {
            { Ids::PARAMETER, {
                { Ids::item, "DefaultFontSize" },
                { Ids::text,  NEEDS_TRANS ("Default Font Size") },
                { Ids::type, "integer" },
                { Ids::value, 12 }
            }}
        }
    }
    //
    }};

    jassert (isValidTree (tree));
    
    return tree;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
