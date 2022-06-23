
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

void getDefaultTest (core::Data& data)
{
    core::Group peace (data.addGroup ("Peace"));
    
    peace.addParameter ("Engine",
        NEEDS_TRANS ("Kind Of Engine"),
        NEEDS_TRANS ("Set kind of stuff"),
        juce::String ("Flowers"));
                
    peace.addParameter ("Power",
        NEEDS_TRANS ("Power Of Engine"),
        NEEDS_TRANS ("Set power of engine"),
        99.5);

    peace.addParameter ("Launcher",
        NEEDS_TRANS ("Love Efficiency"),
        NEEDS_TRANS ("Set love impact"),
        99.5).setHidden (true);

    peace.addParameter ("Random",
        NEEDS_TRANS ("Random Rate"),
        NEEDS_TRANS ("Set entropy in life"),
        0.5).setRange (juce::Range<double> (0, 1));
    
    peace.addParameter ("Guernica",
        NEEDS_TRANS ("Picasso"),
        NEEDS_TRANS ("Set size of picture"),
        juce::Rectangle<int> (0, 0, 349, 776));
    
    core::Group secret (data.addGroup ("Secret", true));
    
    secret.addParameter ("Password",
        NEEDS_TRANS ("Password"),
        NEEDS_TRANS ("Word for magic attack"),
        juce::String ("iloveyou"));
}

core::Data getDefaultPreferences()
{
    core::Data data (Ids::PREFERENCES);
    
    core::Group general (data.addGroup (Tags::General));
    core::Group editing (data.addGroup (Tags::Editing));
    core::Group colors  (data.addGroup (Tags::Colors));
        
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
        Colours::getPalette()->background);

    colors.addParameter (Tags::PatchText,
        NEEDS_TRANS ("Patch Text"),
        NEEDS_TRANS ("Set text color of patch"),
        Colours::getPalette()->backgroundPopup);
        
    colors.addParameter (Tags::BoxBackground,
        NEEDS_TRANS ("Box Background"),
        NEEDS_TRANS ("Set background color of objects"),
        Colours::getPalette()->backgroundBox);
    
    colors.addParameter (Tags::BoxText,
        NEEDS_TRANS ("Box Text"),
        NEEDS_TRANS ("Set text color of objects"),
        Colours::getPalette()->textBox);
    
    colors.addParameter (Tags::LabelBackground,
        NEEDS_TRANS ("Label Background"),
        NEEDS_TRANS ("Set background color of labels"),
        Colours::getPalette()->backgroundBox);
        
    colors.addParameter (Tags::LabelText,
        NEEDS_TRANS ("Label Text"),
        NEEDS_TRANS ("Set text color of labels"),
        Colours::getPalette()->textLabel);
        
    colors.addParameter (Tags::Line,
        NEEDS_TRANS ("Line"),
        NEEDS_TRANS ("Set color of connections"),
        Colours::getPalette()->backgroundBox.contrasting (0.25));
    
    colors.addParameter (Tags::LineSignal,
        NEEDS_TRANS ("Line Signal"),
        NEEDS_TRANS ("Set color of signal connections"),
        Colours::getPalette()->signal);
        
    colors.addParameter (Tags::PinBackground,
        NEEDS_TRANS ("Pin Background"),
        NEEDS_TRANS ("Set background color of pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.05));
    
    colors.addParameter (Tags::PinOver,
        NEEDS_TRANS ("Pin Over"),
        NEEDS_TRANS ("Set color of pins while mouse is over"),
        Colours::getPalette()->textHighlighted);
        
    colors.addParameter (Tags::PinSignal,
        NEEDS_TRANS ("Pin Signal"),
        NEEDS_TRANS ("Set color of signal pins"),
        Colours::getPalette()->signal);
    
    colors.addParameter (Tags::PinBang,
        NEEDS_TRANS ("Pin Bang"),
        NEEDS_TRANS ("Set color of bang pins"),
        Colours::getPalette()->bang);
    
    colors.addParameter (Tags::PinFloat,
        NEEDS_TRANS ("Pin Float"),
        NEEDS_TRANS ("Set color of float pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25));
    
    colors.addParameter (Tags::PinSymbol,
        NEEDS_TRANS ("Pin Symbol"),
        NEEDS_TRANS ("Set color of symbol pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25));
    
    colors.addParameter (Tags::PinList,
        NEEDS_TRANS ("Pin List"),
        NEEDS_TRANS ("Set color of list pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25));
        
    colors.addParameter (Tags::PinAnything,
        NEEDS_TRANS ("Pin Anything"),
        NEEDS_TRANS ("Set color of anything pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25));
    
    colors.addParameter (Tags::BangBackground,
        NEEDS_TRANS ("Bang Background"),
        NEEDS_TRANS ("Set background color of bang widget"),
        Colours::getPalette()->backgroundWidget);
    
    colors.addParameter (Tags::BangFlashOff,
        NEEDS_TRANS ("Bang Flash Off"),
        NEEDS_TRANS ("Set color of bang widget for flash off"),
        Colours::getPalette()->backgroundAlternate);
        
    colors.addParameter (Tags::BangFlashOn,
        NEEDS_TRANS ("Bang Flash On"),
        NEEDS_TRANS ("Set color of bang widget for flash on"),
        Colours::getPalette()->textWarning);

    getDefaultTest (data);
    
    return data;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Preferences::Preferences (const juce::File& file) :
    file_ (file),
    data_ (getDefaultPreferences()),
    isReading_ (false)
{
    addParameterHandler (Tags::SnapToGrid, [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGrid (p.getValue()));
        });
        
    addParameterHandler (Tags::GridSize,   [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGridSize (p.getValue()));
        });
    
    data_.addObserver (this);
}

Preferences::~Preferences()
{
    data_.removeObserver (this);
    
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

        if (data_.read (file_)) { return; }
    }
    
    write();
}

void Preferences::write()
{
    data_.write (file_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
