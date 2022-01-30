
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const juce::var Preferences::getValue (const juce::String& item) const
{
    return core::Parameters::getValue (tree_, item);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::unique_ptr<juce::PropertyComponent> buildConcertinaPanelParameterGet (juce::ValueTree parameter)
{
    return Parameter::Base (parameter).createPropertyComponent();
}

void buildConcertinaPanelParameter (juce::ValueTree parameter, juce::Array<juce::PropertyComponent*>& c)
{
    std::unique_ptr<juce::PropertyComponent> p (buildConcertinaPanelParameterGet (parameter));
    
    p->setPreferredHeight (Spaghettis()->getLookAndFeel().getPropertyPanelHeight());
    p->setTooltip (parameter.getProperty (Ids::info).toString());
    
    c.add (p.release());
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
    
    {
        juce::Array<juce::PropertyComponent*> components;
    
        for (const auto& parameter : group) { buildConcertinaPanelParameter (parameter, components); }
    
        panel->addProperties (components);
    }
    
    c.addPanel (panel.release());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::valueTreePropertyChanged (juce::ValueTree& tree, const juce::Identifier& identifier)
{
    if (core::Parameters::isValidParameter (tree)) {
    //
    const juce::String key (tree.getProperty (Ids::item).toString());
    
    if (key == "SnapToGrid") {
        Spaghettis()->handle (Inputs::setSnapToGrid (tree.getProperty (Ids::value)));
    } else if (key == "GridSize") {
        Spaghettis()->handle (Inputs::setSnapToGridSize (tree.getProperty (Ids::value)));
    }
        
    if (!isReading_) { const int primeInterval = 293; startTimer (primeInterval); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Preferences::getDefault()
{
    juce::ValueTree tree { Ids::PREFERENCES, { }, {
    //
    { Ids::GROUP, { { Ids::name, "General" } },
        {
            { Ids::PARAMETER, {
                { Ids::item,    "AskBeforeQuit" },
                { Ids::text,    NEEDS_TRANS ("Ask Before Quitting") },
                { Ids::info,    NEEDS_TRANS ("Show dialog box to confirm quitting") },
                { Ids::type,    "boolean" },
                { Ids::value,   true }
            } },
            { Ids::PARAMETER, {
                { Ids::item,    "DefaultIsRunView" },
                { Ids::text,    NEEDS_TRANS ("Default Is Run View") },
                { Ids::info,    NEEDS_TRANS ("Open a patch into a run view") },
                { Ids::type,    "boolean" },
                { Ids::value,   false }
            } }
        }
    },
    { Ids::GROUP, { { Ids::name, "Editing" } },
        {
            { Ids::PARAMETER, {
                { Ids::item,    "SnapToGrid" },
                { Ids::text,    NEEDS_TRANS ("Snap To Grid") },
                { Ids::info,    NEEDS_TRANS ("Enable magnetic grid") },
                { Ids::type,    "boolean" },
                { Ids::value,   true }
            } },
            { Ids::PARAMETER, {
                { Ids::item,    "GridSize" },
                { Ids::text,    NEEDS_TRANS ("Grid Size") },
                { Ids::info,    NEEDS_TRANS ("Set magnetic grid spacing") },
                { Ids::type,    "integer" },
                { Ids::minimum, 1 },
                { Ids::maximum, 64 },
                { Ids::value,   "12" }
            } }
        }
    },
    { Ids::GROUP, { { Ids::name, "Nuclear" } },
        {
            { Ids::PARAMETER, {
                { Ids::item,    "Engine" },
                { Ids::text,    NEEDS_TRANS ("Kind Of Engine") },
                { Ids::info,    NEEDS_TRANS ("Set kind of stuff") },
                { Ids::type,    "text" },
                { Ids::value,   "Submarine" }
            } },
            { Ids::PARAMETER, {
                { Ids::item,    "Power" },
                { Ids::text,    NEEDS_TRANS ("Power Of Engine") },
                { Ids::info,    NEEDS_TRANS ("Set power of engine") },
                { Ids::type,    "float" },
                { Ids::value,   "99.5" }
            } },
            { Ids::PARAMETER, {
                { Ids::item,    "Launcher" },
                { Ids::text,    NEEDS_TRANS ("Launcher Efficiency") },
                { Ids::info,    NEEDS_TRANS ("Set tenderness of button") },
                { Ids::type,    "float" },
                { Ids::value,   "99.5" }
            } },
            { Ids::PARAMETER, {
                { Ids::item,    "Random" },
                { Ids::text,    NEEDS_TRANS ("Random Rate") },
                { Ids::info,    NEEDS_TRANS ("Set entropy in life") },
                { Ids::type,    "float" },
                { Ids::minimum, 0 },
                { Ids::maximum, 1 },
                { Ids::value,   "0.5" }
            } }
        }
    },
    { Ids::GROUP, { { Ids::name, "Colors" } },
        {
            { Ids::PARAMETER, {
                { Ids::item,    "PatchBackground" },
                { Ids::text,    NEEDS_TRANS ("Patch Background") },
                { Ids::info,    NEEDS_TRANS ("Set background color of patch") },
                { Ids::type,    "color" },
                { Ids::value,   "ff1f2029" }
            } }
        }
    }
    //
    } };

    jassert (core::Parameters::isValidPreferences (tree));
    
    return tree;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void setPropertyFrom (juce::ValueTree& tree, const juce::ValueTree& group, const juce::ValueTree& parameter)
{
    juce::ValueTree t = tree.getChildWithProperty (Ids::name, group.getProperty (Ids::name))
                            .getChildWithProperty (Ids::item, parameter.getProperty (Ids::item));
    
    if (t.isValid()) {
    //
    const juce::var& newValue (parameter.getProperty (Ids::value));
    const juce::var& oldValue (t.getProperty (Ids::value));
    
    if (!oldValue.equals (newValue)) {
        t.setProperty (Ids::value, Parameter::Base (t).constrained (newValue), nullptr);
    }
    //
    }
}

void setPropertiesFrom (juce::ValueTree& tree, const juce::ValueTree& other)
{
    for (const auto& group : other) {
    for (const auto& parameter : group) {
        setPropertyFrom (tree, group, parameter);
    }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::read()
{
    if (file_.existsAsFile() && file_.hasFileExtension (".xml")) {
        std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file_));
        if (xml) {
            juce::ValueTree t (juce::ValueTree::fromXml (*xml));
            if (core::Parameters::isValidPreferences (t)) {
                juce::ScopedValueSetter<bool> scoped (isReading_, true, false);
                setPropertiesFrom (tree_, t);
                return;
            }
        }
    }
    
    write();
}

void Preferences::write()
{
    std::unique_ptr<juce::XmlElement> xml (tree_.createXml());
    
    if (xml) { xml->writeTo (file_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
