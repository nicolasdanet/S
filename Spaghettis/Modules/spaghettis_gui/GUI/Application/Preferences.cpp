
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

void getDefaultTest (core::Tree& t)
{
    core::Group nuclear (t.addGroup ("Nuclear"));
        
    nuclear.addText ("Engine",
        NEEDS_TRANS ("Kind Of Engine"),
        NEEDS_TRANS ("Set kind of stuff"),
        "Yellow Submarine");
                
    nuclear.addFloat ("Power",
        NEEDS_TRANS ("Power Of Engine"),
        NEEDS_TRANS ("Set power of engine"),
        99.5);

    nuclear.addFloat ("Launcher",
        NEEDS_TRANS ("Launcher Efficiency"),
        NEEDS_TRANS ("Set tenderness of button"),
        99.5);

    nuclear.addFloat ("Random",
        NEEDS_TRANS ("Random Rate"),
        NEEDS_TRANS ("Set entropy in life"),
        0.5)
        .setRange (juce::Range<double> (0, 1));
    
    core::Group secret (t.addGroup ("Secret", true));
    
    secret.addText ("Password",
        NEEDS_TRANS ("Password"),
        NEEDS_TRANS ("Password for nuclear suitcase"),
        "000000000000");
}
    
core::Tree getDefaultPreferences()
{
    core::Tree t (Ids::PREFERENCES);
    
    core::Group general (t.addGroup (Tags::General));
    core::Group editing (t.addGroup (Tags::Editing));
    core::Group colors  (t.addGroup (Tags::Colors));
        
    general.addBoolean (Tags::AskBeforeQuit,
        NEEDS_TRANS ("Ask Before Quitting"),
        NEEDS_TRANS ("Show dialog box to confirm quitting"),
        true);
        
    general.addBoolean (Tags::DefaultIsRunView,
        NEEDS_TRANS ("Default Is Run View"),
        NEEDS_TRANS ("Open a patch into a run view"),
        false);
    
    editing.addBoolean (Tags::SnapToGrid,
        NEEDS_TRANS ("Snap To Grid"),
        NEEDS_TRANS ("Enable magnetic grid"),
        true);

    editing.addInteger (Tags::GridSize,
        NEEDS_TRANS ("Grid Size"),
        NEEDS_TRANS ("Set magnetic grid spacing"),
        12)
        .setRange (juce::Range<int> (1, 64));
    
    colors.addColour (Tags::PatchBackground,
        NEEDS_TRANS ("Patch Background"),
        NEEDS_TRANS ("Set background color of patch"),
        juce::Colour (0xff1f2029));

    getDefaultTest (t);
    
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
