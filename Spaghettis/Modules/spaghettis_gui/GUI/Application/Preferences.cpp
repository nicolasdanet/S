
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::Tree getDefaultPreferences()
{
    core::Tree t (Ids::PREFERENCES);
    
    core::Group general (t.addGroup (Tags::General));
    core::Group editing (t.addGroup (Tags::Editing));
    core::Group colors  (t.addGroup (Tags::Colors));
        
    general.addBoolean (Tags::AskBeforeQuit,
        true,
        NEEDS_TRANS ("Ask Before Quitting"),
        NEEDS_TRANS ("Show dialog box to confirm quitting"));
        
    general.addBoolean (Tags::DefaultIsRunView,
        false,
        NEEDS_TRANS ("Default Is Run View"),
        NEEDS_TRANS ("Open a patch into a run view"));
    
    editing.addBoolean (Tags::SnapToGrid,
        true,
        NEEDS_TRANS ("Snap To Grid"),
        NEEDS_TRANS ("Enable magnetic grid"));

    editing.addInteger (Tags::GridSize,
        12,
        NEEDS_TRANS ("Grid Size"),
        NEEDS_TRANS ("Set magnetic grid spacing"))
        .setRange (juce::Range<int> (1, 64));
    
    colors.addColour (Tags::PatchBackground,
        juce::Colour (0xff1f2029),
        NEEDS_TRANS ("Patch Background"),
        NEEDS_TRANS ("Set background color of patch"));

    /* Just for testing. */
    
    core::Group nuclear (t.addGroup ("Nuclear"));
        
    nuclear.addText ("Engine",
        "Yellow Submarine",
        NEEDS_TRANS ("Kind Of Engine"),
        NEEDS_TRANS ("Set kind of stuff"));
                
    nuclear.addFloat ("Power",
        99.5,
        NEEDS_TRANS ("Power Of Engine"),
        NEEDS_TRANS ("Set power of engine"));

    nuclear.addFloat ("Launcher",
        99.5,
        NEEDS_TRANS ("Launcher Efficiency"),
        NEEDS_TRANS ("Set tenderness of button"));

    nuclear.addFloat ("Random",
        0.5,
        NEEDS_TRANS ("Random Rate"),
        NEEDS_TRANS ("Set entropy in life"))
        .setRange (juce::Range<double> (0, 1));
    
    core::Group secret (t.addGroup ("Secret", true));
    
    secret.addText ("Password",
        "000000000000",
        NEEDS_TRANS ("Password"),
        NEEDS_TRANS ("Password for nuclear suitcase"));
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Preferences::Preferences (const juce::File& file) :
    file_ (file),
    tree_ (getDefaultPreferences()),
    isReading_ (false)
{
    addHandler (Tags::SnapToGrid, [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGrid (p.getValue()));
        });
        
    addHandler (Tags::GridSize,   [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGridSize (p.getValue()));
        });
    
    tree_.addListener (this);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::var Preferences::getValue (const juce::String& group, const juce::String& key) const
{
    return tree_.getValue (group, key);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Preferences::treeHasChanged()
{
    if (!isReading_) { const int primeInterval = 293; startTimer (primeInterval); }
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
            tree_.setParametersFrom (t);
            return;
        }
    }
    
    write();
}

void Preferences::write()
{
    tree_.write (file_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
