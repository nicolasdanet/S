
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

core::Data getDefaultPreferences()
{
    core::Data data (Id::PREFERENCES);
    
    core::Group general (data.addGroup (Tag::General));
    core::Group editing (data.addGroup (Tag::Editing));
    core::Group colors  (data.addGroup (Tag::Colors));
        
    general.addParameter (Tag::AskBeforeQuit,
        NEEDS_TRANS ("Ask Before Quitting"),
        NEEDS_TRANS ("Show dialog box to confirm quitting"),
        true);
        
    general.addParameter (Tag::DefaultIsRunView,
        NEEDS_TRANS ("Default Is Run View"),
        NEEDS_TRANS ("Open a patch into a run view"),
        false);
    
    editing.addParameter (Tag::SnapToGrid,
        NEEDS_TRANS ("Snap To Grid"),
        NEEDS_TRANS ("Enable magnetic grid"),
        true);

    editing.addParameter (Tag::GridSize,
        NEEDS_TRANS ("Grid Size"),
        NEEDS_TRANS ("Set magnetic grid spacing"),
        12).setRange (juce::Range<int> (1, 64));
    
    colors.addParameter (Tag::PatchBackground,
        NEEDS_TRANS ("Patch Background"),
        NEEDS_TRANS ("Set background color of patch"),
        Colours::getPalette()->background);

    colors.addParameter (Tag::PatchText,
        NEEDS_TRANS ("Patch Text"),
        NEEDS_TRANS ("Set text color of patch"),
        Colours::getPalette()->backgroundPopup);
    
    colors.addParameter (Tag::Lasso,
        NEEDS_TRANS ("Lasso"),
        NEEDS_TRANS ("Set color of lasso"),
        Colours::getPalette()->selection);
    
    colors.addParameter (Tag::Locator,
        NEEDS_TRANS ("Locator"),
        NEEDS_TRANS ("Set color of locator"),
        Colours::getPalette()->textError);
        
    colors.addParameter (Tag::BoxBackground,
        NEEDS_TRANS ("Box Background"),
        NEEDS_TRANS ("Set background color of objects"),
        Colours::getPalette()->backgroundBox);
    
    colors.addParameter (Tag::BoxPinsBackground,
        NEEDS_TRANS ("Box Pin Background"),
        NEEDS_TRANS ("Set background color of pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.05f));
    
    colors.addParameter (Tag::BoxSelected,
        NEEDS_TRANS ("Box Selected"),
        NEEDS_TRANS ("Set highlight color for selected object"),
        Colours::getPalette()->selection);
    
    colors.addParameter (Tag::BoxWrong,
        NEEDS_TRANS ("Box Wrong"),
        NEEDS_TRANS ("Set highlight color for bad object"),
        Colours::getPalette()->textError);
        
    colors.addParameter (Tag::BoxText,
        NEEDS_TRANS ("Box Text"),
        NEEDS_TRANS ("Set text color of objects"),
        Colours::getPalette()->textBox);
    
    colors.addParameter (Tag::LabelBackground,
        NEEDS_TRANS ("Label Background"),
        NEEDS_TRANS ("Set background color of run view labels"),
        Colours::getPalette()->backgroundBox.withAlpha (0.5f));
        
    colors.addParameter (Tag::LabelText,
        NEEDS_TRANS ("Label Text"),
        NEEDS_TRANS ("Set text color of run view labels"),
        Colours::getPalette()->textBox);
        
    colors.addParameter (Tag::Line,
        NEEDS_TRANS ("Line"),
        NEEDS_TRANS ("Set color of connections"),
        Colours::getPalette()->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::LineSignal,
        NEEDS_TRANS ("Line Signal"),
        NEEDS_TRANS ("Set color of signal connections"),
        Colours::getPalette()->signal);
    
    colors.addParameter (Tag::LineSelected,
        NEEDS_TRANS ("Line Selected"),
        NEEDS_TRANS ("Set highlight color for selected lines"),
        Colours::getPalette()->selection);
        
    colors.addParameter (Tag::PinOver,
        NEEDS_TRANS ("Pin Over"),
        NEEDS_TRANS ("Set color of pins while mouse is over"),
        Colours::getPalette()->selection);
    
    colors.addParameter (Tag::PinSignal,
        NEEDS_TRANS ("Pin Signal"),
        NEEDS_TRANS ("Set color of signal pins"),
        Colours::getPalette()->signal);
    
    colors.addParameter (Tag::PinBang,
        NEEDS_TRANS ("Pin Bang"),
        NEEDS_TRANS ("Set color of bang pins"),
        Colours::getPalette()->bang);
    
    colors.addParameter (Tag::PinFloat,
        NEEDS_TRANS ("Pin Float"),
        NEEDS_TRANS ("Set color of float pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::PinSymbol,
        NEEDS_TRANS ("Pin Symbol"),
        NEEDS_TRANS ("Set color of symbol pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::PinList,
        NEEDS_TRANS ("Pin List"),
        NEEDS_TRANS ("Set color of list pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25f));
        
    colors.addParameter (Tag::PinAnything,
        NEEDS_TRANS ("Pin Anything"),
        NEEDS_TRANS ("Set color of anything pins"),
        Colours::getPalette()->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::ArrayBackground,
        NEEDS_TRANS ("Array Background"),
        NEEDS_TRANS ("Set background color of graphic arrays"),
        Colours::getPalette()->backgroundBox);
    
    colors.addParameter (Tag::ArrayValue,
        NEEDS_TRANS ("Array Value"),
        NEEDS_TRANS ("Set content color of graphic arrays"),
        Colours::getPalette()->backgroundPopup);
        
    colors.addParameter (Tag::AtomBackground,
        NEEDS_TRANS ("Atom Background"),
        NEEDS_TRANS ("Set background color of number boxes"),
        Colours::getPalette()->backgroundBox);
        
    colors.addParameter (Tag::AtomText,
        NEEDS_TRANS ("Atom Text"),
        NEEDS_TRANS ("Set text color of number boxes"),
        Colours::getPalette()->textBox);
    
    colors.addParameter (Tag::AtomClicked,
        NEEDS_TRANS ("Atom Clicked"),
        NEEDS_TRANS ("Set triangle color while changing value"),
        Colours::getPalette()->textWarning);
        
    colors.addParameter (Tag::BangBackground,
        NEEDS_TRANS ("Bang Background"),
        NEEDS_TRANS ("Set background color of bang"),
        Colours::getPalette()->backgroundWidget);
    
    colors.addParameter (Tag::BangFlashOff,
        NEEDS_TRANS ("Bang Flash Off"),
        NEEDS_TRANS ("Set color of bang for flash off"),
        Colours::getPalette()->backgroundAlternate);
        
    colors.addParameter (Tag::BangFlashOn,
        NEEDS_TRANS ("Bang Flash On"),
        NEEDS_TRANS ("Set color of bang for flash on"),
        Colours::getPalette()->textWarning);

    colors.addParameter (Tag::ToggleBackground,
        NEEDS_TRANS ("Toggle Background"),
        NEEDS_TRANS ("Set background color of toggle"),
        Colours::getPalette()->backgroundWidget);
    
    colors.addParameter (Tag::Toggle,
        NEEDS_TRANS ("Toggle"),
        NEEDS_TRANS ("Set color of cross"),
        Colours::getPalette()->backgroundPopup);
    
    colors.addParameter (Tag::RadioBackground,
        NEEDS_TRANS ("Radio buttons Background"),
        NEEDS_TRANS ("Set background color of radio buttons"),
        Colours::getPalette()->backgroundWidget);
    
    colors.addParameter (Tag::RadioButton,
        NEEDS_TRANS ("Radio Button"),
        NEEDS_TRANS ("Set color of activated radio buttons"),
        Colours::getPalette()->backgroundPopup);
    
    colors.addParameter (Tag::SliderBackground,
        NEEDS_TRANS ("Slider Background"),
        NEEDS_TRANS ("Set background color of sliders"),
        Colours::getPalette()->backgroundWidget);
    
    colors.addParameter (Tag::SliderBar,
        NEEDS_TRANS ("Slider Bar"),
        NEEDS_TRANS ("Set foreground color of sliders"),
        Colours::getPalette()->backgroundPopup);
    
    colors.addParameter (Tag::DialBackground,
        NEEDS_TRANS ("Dial Background"),
        NEEDS_TRANS ("Set background color of dials"),
        Colours::getPalette()->backgroundBox);
    
    colors.addParameter (Tag::DialForeground,
        NEEDS_TRANS ("Dial Foreground"),
        NEEDS_TRANS ("Set inactive foreground color"),
        Colours::getPalette()->backgroundPopup);
        
    colors.addParameter (Tag::DialNeedle,
        NEEDS_TRANS ("Dial Needle"),
        NEEDS_TRANS ("Set active foreground color"),
        Colours::getPalette()->textWarning);
    
    colors.addParameter (Tag::DialText,
        NEEDS_TRANS ("Dial Text"),
        NEEDS_TRANS ("Set color of text"),
        Colours::getPalette()->textBox);
        
    colors.addParameter (Tag::CommentBackground,
        NEEDS_TRANS ("Comment Background"),
        NEEDS_TRANS ("Set background color of comments"),
        Colours::getPalette()->transparent);
        
    colors.addParameter (Tag::CommentText,
        NEEDS_TRANS ("Comment Text"),
        NEEDS_TRANS ("Set text color of comments"),
        Colours::getPalette()->textComment);
    
    colors.addParameter (Tag::MessageBackground,
        NEEDS_TRANS ("Message Background"),
        NEEDS_TRANS ("Set background color of messages"),
        Colours::getPalette()->backgroundBox);
        
    colors.addParameter (Tag::MessageText,
        NEEDS_TRANS ("Message Text"),
        NEEDS_TRANS ("Set text color of messages"),
        Colours::getPalette()->textBox);
        
    colors.addParameter (Tag::MessageClicked,
        NEEDS_TRANS ("Message Clicked"),
        NEEDS_TRANS ("Set text color clicking messages"),
        Colours::getPalette()->textWarning.brighter (0.25));
        
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
    addParameterHandler (Tag::SnapToGrid, [] (const core::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGrid (p.getValue()));
        });
        
    addParameterHandler (Tag::GridSize,   [] (const core::Parameter& p) {
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

void Preferences::parameterHasChanged (const core::Group&, const core::Parameter&)
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

        if (data_.readValuesFromFile (file_)) { return; }
    }
    
    write();
}

void Preferences::write()
{
    data_.writeValuesToFile (file_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
