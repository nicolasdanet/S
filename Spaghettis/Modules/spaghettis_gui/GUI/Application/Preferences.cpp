
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
        
    general.addParameter (Tags::AskBeforeQuit,
        NEEDS_TRANS ("Ask Before Quitting"),
        NEEDS_TRANS ("Show dialog box to confirm quitting"),
        true);
        
    general.addParameter (Tags::DefaultIsRunView,
        NEEDS_TRANS ("Default Is Run View"),
        NEEDS_TRANS ("Open a patch into a run view"),
        false);
    
    editing.addParameter (Tags::SnapToGrid,
        NEEDS_TRANS ("Snap To Grid"),
        NEEDS_TRANS ("Enable magnetic grid"),
        true);

    editing.addParameter (Tags::GridSize,
        NEEDS_TRANS ("Grid Size"),
        NEEDS_TRANS ("Set magnetic grid spacing"),
        12).setRange (juce::Range<int> (1, 64));
    
    colors.addParameter (Tags::PatchBackground,
        NEEDS_TRANS ("Patch Background"),
        NEEDS_TRANS ("Set background color of patch"),
        juce::Colour (0xff1f2029));

    colors.addParameter (Tags::BoxBackground,
        NEEDS_TRANS ("Box Background"),
        NEEDS_TRANS ("Set background color of objects"),
        juce::Colour (0xff1f2029));
        
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
    addParameterHandler (Tags::SnapToGrid, [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGrid (p.getValue()));
        });
        
    addParameterHandler (Tags::GridSize,   [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGridSize (p.getValue()));
        });
    
    tree_.addListener (this);
}

Preferences::~Preferences()
{
    tree_.removeListener (this);
    
    write();
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
    {
        juce::ScopedValueSetter<bool> scoped (isReading_, true, false);

        if (tree_.read (file_)) { return; }
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
