
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const juce::var Preferences::getValue (const juce::String& group, const juce::String& item) const
{
    return juce::var (false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::valueTreePropertyChanged (juce::ValueTree& parameter, const juce::Identifier& identifier)
{
    /*
    const juce::String key (core::Parameter::getItem (parameter));
    
    if (key == "SnapToGrid") {
        Spaghettis()->handle (Inputs::setSnapToGrid (core::Parameter::getValue (parameter)));
    } else if (key == "GridSize") {
        Spaghettis()->handle (Inputs::setSnapToGridSize (core::Parameter::getValue (parameter)));
    }
        
    if (!isReading_) { const int primeInterval = 293; startTimer (primeInterval); }
    */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Preferences::getDefault()
{
    juce::ValueTree tree (Ids::PREFERENCES);
    
    {
        juce::ValueTree group = core::Tree::addGroup (tree, "General");

        core::Tree::addParameter (group,
            "AskBeforeQuit",
            NEEDS_TRANS ("Ask Before Quitting"),
            NEEDS_TRANS ("Show dialog box to confirm quitting"),
            true);
        core::Tree::addParameter (group,
            "DefaultIsRunView",
            NEEDS_TRANS ("Default Is Run View"),
            NEEDS_TRANS ("Open a patch into a run view"),
            false);
    }
    
    {
        juce::ValueTree group = core::Tree::addGroup (tree, "Editing");

        core::Tree::addParameter (group,
            "SnapToGrid",
            NEEDS_TRANS ("Snap To Grid"),
            NEEDS_TRANS ("Enable magnetic grid"),
            true);
        core::Tree::addParameter (group,
            "GridSize",
            NEEDS_TRANS ("Grid Size"),
            NEEDS_TRANS ("Set magnetic grid spacing"),
            12);
            // { Ids::minimum, 1 }
            // { Ids::maximum, 64 }
    }
    
    {
        juce::ValueTree group = core::Tree::addGroup (tree, "Nuclear");

        core::Tree::addParameter (group,
            "Engine",
            NEEDS_TRANS ("Kind Of Engine"),
            NEEDS_TRANS ("Set kind of stuff"),
            "Submarine");
        core::Tree::addParameter (group,
            "Power",
            NEEDS_TRANS ("Power Of Engine"),
            NEEDS_TRANS ("Set power of engine"),
            99.5);
        core::Tree::addParameter (group,
            "Launcher",
            NEEDS_TRANS ("Launcher Efficiency"),
            NEEDS_TRANS ("Set tenderness of button"),
            99.5);
        core::Tree::addParameter (group,
            "Random",
            NEEDS_TRANS ("Random Rate"),
            NEEDS_TRANS ("Set entropy in life"),
            0.5);
            // { Ids::minimum, 0 }
            // { Ids::maximum, 1 }
    }
    
    {
        juce::ValueTree group = core::Tree::addGroup (tree, "Colors");

        core::Tree::addParameter (group,
            "PatchBackground",
            NEEDS_TRANS ("Patch Background"),
            NEEDS_TRANS ("Set background color of patch"),
            juce::Colour (0xff1f2029));
    }
    
    jassert (core::Tree::isValid (tree, Ids::PREFERENCES));
    
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
        t.setProperty (Ids::value, Parameters::Base (t).constrained (newValue), nullptr);
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
            if (core::Tree::isValid (t, Ids::PREFERENCES)) {
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
