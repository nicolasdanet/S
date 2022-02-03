
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const juce::var Preferences::getValue (const juce::String& group, const juce::String& key) const
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

core::Tree Preferences::getDefault()
{
    core::Tree t (Ids::PREFERENCES);
    
    core::Group general (t.addGroup ("General"));
    core::Group editing (t.addGroup ("Editing"));
    core::Group nuclear (t.addGroup ("Editing"));
    core::Group colors  (t.addGroup ("Colors"));
        
    general.addParameter ("AskBeforeQuit", true)
                .setText (NEEDS_TRANS ("Ask Before Quitting"))
                .setInfo (NEEDS_TRANS ("Show dialog box to confirm quitting"));
        
    general.addParameter ("DefaultIsRunView", false)
                .setText (NEEDS_TRANS ("Default Is Run View"))
                .setInfo (NEEDS_TRANS ("Open a patch into a run view"));
    
    editing.addParameter ("SnapToGrid", true)
                .setText (NEEDS_TRANS ("Snap To Grid"))
                .setInfo (NEEDS_TRANS ("Enable magnetic grid"));

    editing.addParameter ("GridSize", 12)
                .setText (NEEDS_TRANS ("Grid Size"))
                .setInfo (NEEDS_TRANS ("Set magnetic grid spacing"))
                .setRange (juce::Range<int> (1, 64));
    
    nuclear.addParameter ("Engine", "Yellow Submarine")
                .setText (NEEDS_TRANS ("Kind Of Engine"))
                .setInfo (NEEDS_TRANS ("Set kind of stuff"));
                
    nuclear.addParameter ("Power", 99.5)
                .setText (NEEDS_TRANS ("Power Of Engine"))
                .setInfo (NEEDS_TRANS ("Set power of engine"));

    nuclear.addParameter ("Launcher", 99.5)
                .setText (NEEDS_TRANS ("Launcher Efficiency"))
                .setInfo (NEEDS_TRANS ("Set tenderness of button"));

    nuclear.addParameter ("Random", 0.5)
                .setText (NEEDS_TRANS ("Random Rate"))
                .setInfo (NEEDS_TRANS ("Set entropy in life"))
                .setRange (juce::Range<double> (0, 1));
    
    colors.addParameter ("PatchBackground", juce::Colour (0xff1f2029))
                .setText (NEEDS_TRANS ("Patch Background"))
                .setInfo (NEEDS_TRANS ("Set background color of patch"));
    
    jassert (t.isValid (Ids::PREFERENCES));
    
    return t;
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
            juce::ScopedValueSetter<bool> scoped (isReading_, true, false);
            tree_.setPropertiesFrom (t);
            return;
        }
    }
    
    write();
}

void Preferences::write()
{
    /*
    std::unique_ptr<juce::XmlElement> xml (tree_.createXml());
    
    if (xml) { xml->writeTo (file_); }
    */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
