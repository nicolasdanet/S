
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

data::Data getDefaultPreferences()
{
    data::Data data (Id::PREFERENCES);
    
    data::Group general (data.addGroup (Tag::General));
    data::Group editing (data.addGroup (Tag::Editing));
    data::Group colors  (data.addGroup (Tag::Colors));
    
    const Palette* p = Palette::getInstance();
    
    general.addParameter (Tag::AskBeforeQuit,
        NEEDS_TRANS ("Ask Before Quitting"),
        NEEDS_TRANS ("Show dialog box to confirm quitting"),
        true);
        
    general.addParameter (Tag::DefaultIsRunView,
        NEEDS_TRANS ("Default Is Run View"),
        NEEDS_TRANS ("Open patch as a run view"),
        false);
    
    /* */
        
    editing.addParameter (Tag::GridSize,
        NEEDS_TRANS ("Grid Size"),
        NEEDS_TRANS ("Set magnetic grid spacing"),
        12).setRange (juce::Range<int> (1, 64));

    editing.addParameter (Tag::SnapToGrid,
        NEEDS_TRANS ("Snap To Grid"),
        NEEDS_TRANS ("Enable magnetic grid"),
        true);
        
    /* */
    
    colors.addParameter (Tag::ArrayBackground,
        NEEDS_TRANS ("Array Background"),
        NEEDS_TRANS ("Set background color of graphic arrays"),
        p->backgroundBox);
    
    colors.addParameter (Tag::ArrayValue,
        NEEDS_TRANS ("Array Value"),
        NEEDS_TRANS ("Set content color of graphic arrays"),
        p->backgroundPopup);
    
    //
        
    colors.addParameter (Tag::AtomBackground,
        NEEDS_TRANS ("Atom Background"),
        NEEDS_TRANS ("Set background color of number boxes"),
        p->backgroundBox);
        
    colors.addParameter (Tag::AtomText,
        NEEDS_TRANS ("Atom Text"),
        NEEDS_TRANS ("Set text color of number boxes"),
        p->textBox);
    
    colors.addParameter (Tag::AtomClicked,
        NEEDS_TRANS ("Atom Clicked"),
        NEEDS_TRANS ("Set triangle color while changing value"),
        p->textWarning);
    
    //
        
    colors.addParameter (Tag::BangBackground,
        NEEDS_TRANS ("Bang Background"),
        NEEDS_TRANS ("Set background color of bang"),
        p->backgroundWidget);
    
    colors.addParameter (Tag::BangFlashOff,
        NEEDS_TRANS ("Bang Flash Off"),
        NEEDS_TRANS ("Set color of bang for flash off"),
        p->backgroundAlternate);
        
    colors.addParameter (Tag::BangFlashOn,
        NEEDS_TRANS ("Bang Flash On"),
        NEEDS_TRANS ("Set color of bang for flash on"),
        p->textWarning);

    //
        
    colors.addParameter (Tag::BoxBackground,
        NEEDS_TRANS ("Box Background"),
        NEEDS_TRANS ("Set background color of objects"),
        p->backgroundBox);
    
    colors.addParameter (Tag::BoxText,
        NEEDS_TRANS ("Box Text"),
        NEEDS_TRANS ("Set text color of objects"),
        p->textBox);
        
    colors.addParameter (Tag::BoxPinsBackground,
        NEEDS_TRANS ("Box Pin Background"),
        NEEDS_TRANS ("Set background color of pins"),
        p->backgroundBox.contrasting (0.05f));
    
    colors.addParameter (Tag::BoxSelected,
        NEEDS_TRANS ("Box Selected"),
        NEEDS_TRANS ("Set highlight color for selected object"),
        p->selection);
    
    colors.addParameter (Tag::BoxWrong,
        NEEDS_TRANS ("Box Wrong"),
        NEEDS_TRANS ("Set highlight color for missing object"),
        p->textError);
        
    //
        
    colors.addParameter (Tag::CommentBackground,
        NEEDS_TRANS ("Comment Background"),
        NEEDS_TRANS ("Set background color of comments"),
        p->transparent);
        
    colors.addParameter (Tag::CommentText,
        NEEDS_TRANS ("Comment Text"),
        NEEDS_TRANS ("Set text color of comments"),
        p->textComment);
        
    //
        
    colors.addParameter (Tag::DialBackground,
        NEEDS_TRANS ("Dial Background"),
        NEEDS_TRANS ("Set background color of dials"),
        p->backgroundBox);
    
    colors.addParameter (Tag::DialForeground,
        NEEDS_TRANS ("Dial Foreground"),
        NEEDS_TRANS ("Set inactive foreground color"),
        p->backgroundPopup);
        
    colors.addParameter (Tag::DialNeedle,
        NEEDS_TRANS ("Dial Needle"),
        NEEDS_TRANS ("Set active foreground color"),
        p->textWarning);
    
    colors.addParameter (Tag::DialText,
        NEEDS_TRANS ("Dial Text"),
        NEEDS_TRANS ("Set color of text"),
        p->textBox);
    
    //
        
    colors.addParameter (Tag::Line,
        NEEDS_TRANS ("Line"),
        NEEDS_TRANS ("Set color of connections"),
        p->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::LineSignal,
        NEEDS_TRANS ("Line Signal"),
        NEEDS_TRANS ("Set color for signal connections"),
        p->signal);
    
    colors.addParameter (Tag::LineSelected,
        NEEDS_TRANS ("Line Selected"),
        NEEDS_TRANS ("Set highlight color for selected lines"),
        p->selection);
        
    //
        
    colors.addParameter (Tag::MessageBackground,
        NEEDS_TRANS ("Message Background"),
        NEEDS_TRANS ("Set background color of messages"),
        p->backgroundBox);
        
    colors.addParameter (Tag::MessageText,
        NEEDS_TRANS ("Message Text"),
        NEEDS_TRANS ("Set text color of messages"),
        p->textBox);
        
    colors.addParameter (Tag::MessageClicked,
        NEEDS_TRANS ("Message Clicked"),
        NEEDS_TRANS ("Set text color clicking messages"),
        p->textWarning.brighter (0.25));
    
    //
        
    colors.addParameter (Tag::PatchBackground,
        NEEDS_TRANS ("Patch Background"),
        NEEDS_TRANS ("Set background color of patches"),
        p->background);

    colors.addParameter (Tag::PatchLasso,
        NEEDS_TRANS ("Patch Lasso"),
        NEEDS_TRANS ("Set color of lasso"),
        p->selection);
    
    colors.addParameter (Tag::PatchLocator,
        NEEDS_TRANS ("Patch Locator"),
        NEEDS_TRANS ("Set color of locator"),
        p->textError);
        
    //
        
    colors.addParameter (Tag::PinSignal,
        NEEDS_TRANS ("Pin Signal"),
        NEEDS_TRANS ("Set color of signal pins"),
        p->signal);
    
    colors.addParameter (Tag::PinBang,
        NEEDS_TRANS ("Pin Bang"),
        NEEDS_TRANS ("Set color of bang pins"),
        p->bang);
    
    colors.addParameter (Tag::PinFloat,
        NEEDS_TRANS ("Pin Float"),
        NEEDS_TRANS ("Set color of float pins"),
        p->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::PinSymbol,
        NEEDS_TRANS ("Pin Symbol"),
        NEEDS_TRANS ("Set color of symbol pins"),
        p->backgroundBox.contrasting (0.25f));
    
    colors.addParameter (Tag::PinList,
        NEEDS_TRANS ("Pin List"),
        NEEDS_TRANS ("Set color of list pins"),
        p->backgroundBox.contrasting (0.25f));
        
    colors.addParameter (Tag::PinAnything,
        NEEDS_TRANS ("Pin Anything"),
        NEEDS_TRANS ("Set color of anything pins"),
        p->backgroundBox.contrasting (0.25f));
        
    //
        
    colors.addParameter (Tag::RadioBackground,
        NEEDS_TRANS ("Radio buttons Background"),
        NEEDS_TRANS ("Set background color of radio buttons"),
        p->backgroundWidget);
    
    colors.addParameter (Tag::RadioButton,
        NEEDS_TRANS ("Radio Button"),
        NEEDS_TRANS ("Set color of activated radio buttons"),
        p->backgroundPopup);
    
    //
        
    colors.addParameter (Tag::SliderBackground,
        NEEDS_TRANS ("Slider Background"),
        NEEDS_TRANS ("Set background color of sliders"),
        p->backgroundWidget);
    
    colors.addParameter (Tag::SliderBar,
        NEEDS_TRANS ("Slider Bar"),
        NEEDS_TRANS ("Set foreground color of sliders"),
        p->backgroundPopup);
    
    //
        
    colors.addParameter (Tag::ToggleBackground,
        NEEDS_TRANS ("Toggle Background"),
        NEEDS_TRANS ("Set background color of toggle"),
        p->backgroundWidget);
    
    colors.addParameter (Tag::Toggle,
        NEEDS_TRANS ("Toggle"),
        NEEDS_TRANS ("Set color of cross"),
        p->backgroundPopup);
    
    //
    
    colors.addParameter (Tag::VuBackground,
        NEEDS_TRANS ("Vu Background"),
        NEEDS_TRANS ("Set background color of VU meters"),
        p->backgroundWidget);
    
    colors.addParameter (Tag::VuBarCold,
        NEEDS_TRANS ("Vu Bar Cold"),
        NEEDS_TRANS ("Set foreground color of low VU bars"),
        p->vuCold);
    
    colors.addParameter (Tag::VuBarWarm,
        NEEDS_TRANS ("Vu Bar Warm"),
        NEEDS_TRANS ("Set foreground color of middle VU bars"),
        p->vuWarm);
    
    colors.addParameter (Tag::VuBarHot,
        NEEDS_TRANS ("Vu Bar Hot"),
        NEEDS_TRANS ("Set foreground color of top VU bars"),
        p->vuHot);
        
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
    addParameterHandler (Tag::SnapToGrid, [] (const data::Parameter& p) {
            Spaghettis()->handle (Inputs::setSnapToGrid (p.getValue()));
        });
        
    addParameterHandler (Tag::GridSize,   [] (const data::Parameter& p) {
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

void Preferences::parameterHasChanged (const data::Group&, const data::Parameter&)
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
