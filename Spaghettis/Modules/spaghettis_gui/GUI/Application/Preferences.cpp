
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::PropertyComponent* Parameters::Base::createPropertyComponent() const
{
    if (type_ == "boolean")      { return new Parameters::Boolean (p_); }
    if (type_ == "color")        { return new Parameters::Colour (p_);  }
    else if (type_ == "integer") { return new Parameters::Integer (p_, *this); }
    else if (type_ == "float")   {
        if (hasRange()) {
            return new Parameters::Slider (p_, *this);
        } else {
            return new Parameters::Float (p_, *this);
        }
    } else {
        return new Parameters::Text (p_);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::PropertyComponent* buildConcertinaPanelParametersGet (juce::ValueTree parameter)
{
    return Parameters::Base (parameter).createPropertyComponent();
}

void buildConcertinaPanelParameters (juce::ValueTree parameter, juce::Array<juce::PropertyComponent*>& c)
{
    std::unique_ptr<juce::PropertyComponent> p (buildConcertinaPanelParametersGet (parameter));
    
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
    
        for (const auto& parameter : group) { buildConcertinaPanelParameters (parameter, components); }
    
        panel->addProperties (components);
    }
    
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
    return (tree.hasType (Ids::GROUP) && tree.getProperty (Ids::name).isString());
}

bool isValidParameter (const juce::ValueTree& tree)
{
    return (tree.hasType (Ids::PARAMETER)
                && tree.getProperty (Ids::item).isString()
                && tree.getProperty (Ids::text).isString()
                && tree.getProperty (Ids::info).isString()
                && tree.getProperty (Ids::type).isString()
                && tree.hasProperty (Ids::value));
}

bool isValidTree (const juce::ValueTree& tree)
{
    if (tree.isValid() && tree.hasType (Ids::PREFERENCES)) {
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::valueTreePropertyChanged (juce::ValueTree& tree, const juce::Identifier& identifier)
{
    if (isValidParameter (tree)) {
    //
    const juce::String key (tree.getProperty (Ids::item).toString());
    const juce::String value (tree.getProperty (Ids::value).toString());
    
    DBG (key + " / " + value);
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
                { Ids::item,    "Bar" },
                { Ids::text,    NEEDS_TRANS ("Bar") },
                { Ids::info,    NEEDS_TRANS ("Set bar color") },
                { Ids::type,    "color" },
                { Ids::value,   "ffffffff" }
            } },
            { Ids::PARAMETER, {
                { Ids::item,    "Foo" },
                { Ids::text,    NEEDS_TRANS ("Foo") },
                { Ids::info,     NEEDS_TRANS ("Set foo color") },
                { Ids::type,    "color" },
                { Ids::value,   "ffffffff" }
            } }
        }
    }
    //
    } };

    jassert (isValidTree (tree));
    
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
    const juce::var& v (parameter.getProperty (Ids::value));
    
    if (t.getProperty (Ids::value).hasSameTypeAs (v)) {
        t.setProperty (Ids::value, Parameters::Base (t).constrained (v), nullptr);
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
            if (isValidTree (t)) {
                setPropertiesFrom (tree_, t); return;
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
