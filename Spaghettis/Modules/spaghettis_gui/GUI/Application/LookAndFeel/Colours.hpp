
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SPAGHETTIS_COLOUR(c)    (c | 0x7f000000)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct Colours {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Palette {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Palette()
    {
        /* < https://ethanschoonover.com/solarized/ > */
    
        // const juce::Colour base03       = juce::Colour (0xff002b36);
        // const juce::Colour base02       = juce::Colour (0xff073642);
        const juce::Colour base01       = juce::Colour (0xff586e75);
        // const juce::Colour base00    = juce::Colour (0xff657b83);
        // const juce::Colour base0     = juce::Colour (0xff839496);
        const juce::Colour base1        = juce::Colour (0xff93a1a1);
        // const juce::Colour base2     = juce::Colour (0xffeee8d5);
        // const juce::Colour base3     = juce::Colour (0xfffdf6e3);
        const juce::Colour yellow       = juce::Colour (0xffb58900);
        const juce::Colour orange       = juce::Colour (0xffcb4b16);
        // const juce::Colour red       = juce::Colour (0xffdc322f);
        // const juce::Colour magenta   = juce::Colour (0xffd33682);
        // const juce::Colour violet    = juce::Colour (0xff6c71c4);
        // const juce::Colour blue      = juce::Colour (0xff268bd2);
        // const juce::Colour cyan      = juce::Colour (0xff2aa198);
        const juce::Colour green        = juce::Colour (0xff859900);
        
        textDefault                     = base01;
        textSystem                      = base1;
        textWarning                     = yellow;
        textError                       = orange;
        textHighlighted                 = green;
        textMenu                        = juce::Colours::white;
        textPopup                       = juce::Colours::lightgrey;
        textBox                         = base1;
        thumb                           = juce::Colours::grey;
        transparent                     = juce::Colours::transparentBlack;
        
        background                      = juce::Colour (0xff1f2029);
        backgroundAlternate             = background.darker (0.10);
        backgroundPopup                 = background.brighter (0.15);
        backgroundBox                   = background.darker (0.25);

        #if JUCE_LINUX
        backgroundMenu                  = background.darker (0.50);
        backgroundWidget                = background.darker (0.75);
        #else
        backgroundMenu                  = background.darker (0.25);
        backgroundWidget                = background.darker (0.50);
        #endif
    }

    ~Palette() = default;

public:
    Palette (const Palette&) = default;
    Palette (Palette&&) = default;
    Palette& operator = (const Palette&) = default;
    Palette& operator = (Palette&&) = default;
    
public:
    juce::Colour textDefault;
    juce::Colour textSystem;
    juce::Colour textWarning;
    juce::Colour textError;
    juce::Colour textHighlighted;
    juce::Colour textMenu;
    juce::Colour textPopup;
    juce::Colour textBox;
    juce::Colour thumb;
    juce::Colour transparent;
    juce::Colour background;
    juce::Colour backgroundAlternate;
    juce::Colour backgroundPopup;
    juce::Colour backgroundBox;
    juce::Colour backgroundMenu;
    juce::Colour backgroundWidget;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Palette* getPalette()
{
    static Palette p; return &p;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum ColourIds : int {
    windowBackground        = SPAGHETTIS_COLOUR (0),
    consoleTextDefault,
    consoleTextSystem,
    consoleTextWarning,
    consoleTextError,
    consoleTextHighlighted,
    consoleBackground,
    consoleBackgroundAlternate,
    menubarBackground,
    menubarText,
    menubarPopupBackground,
    menubarPopupBackgroundHighlighted,
    menubarPopupText,
    menubarPopupTextHighlighted,
    menubarPopupSeparator,
    toolbarBackground,
    toolbarBackgroundDown,
    toolbarBackgroundOver,
    toolbarIconOn,
    toolbarIconOff,
    searchpathsBackground,
    searchpathsBackgroundAlternate,
    searchpathsText,
    searchpathsTextHighlighted,
    devicesParameterBackground,
    devicesParameterText,
    devicesComboBoxBackground,
    devicesComboBoxBackgroundActive,
    devicesComboBoxArrow,
    tooltipBackground,
    tooltipText,
    callOutBoxBackground,
    callOutBoxOutline,
    alertWindowBackground,
    alertWindowOutline,
    alertWindowText,
    alertWindowIcon,
    alertWindowButtonBackground,
    alertWindowButtonBackgroundOver,
    alertWindowButtonOutline,
    alertWindowButtonText,
    alertWindowButtonTextHighlighted,
    parametersHeaderBackground,
    parametersHeaderText,
    parametersHeaderArrow,
    parametersParameterBackground,
    parametersParameterText,
    parametersSliderBackground,
    parametersSliderTrack,
    parametersColourBackground,
    parametersColourText,
    parametersBoolean,
    parametersBooleanTick
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    const Palette* t = getPalette();
    
    lf->setColour (consoleTextDefault,                                  t->textDefault);
    lf->setColour (consoleTextSystem,                                   t->textSystem);
    lf->setColour (consoleTextWarning,                                  t->textWarning);
    lf->setColour (consoleTextError,                                    t->textError);
    lf->setColour (consoleTextHighlighted,                              t->textHighlighted);
    lf->setColour (consoleBackground,                                   t->background);
    lf->setColour (consoleBackgroundAlternate,                          t->backgroundAlternate);
    lf->setColour (menubarBackground,                                   t->backgroundMenu);
    lf->setColour (menubarText,                                         t->textMenu);
    lf->setColour (menubarPopupBackground,                              t->backgroundPopup);
    lf->setColour (menubarPopupBackgroundHighlighted,                   t->textError);
    lf->setColour (menubarPopupText,                                    t->textPopup);
    lf->setColour (menubarPopupTextHighlighted,                         t->textMenu);
    lf->setColour (menubarPopupSeparator,                               t->thumb);
    lf->setColour (toolbarBackground,                                   t->backgroundMenu);
    lf->setColour (toolbarBackgroundDown,                               t->textError);
    lf->setColour (toolbarBackgroundOver,                               t->backgroundMenu);
    lf->setColour (toolbarIconOn,                                       t->textMenu);
    lf->setColour (toolbarIconOff,                                      t->thumb);
    lf->setColour (searchpathsBackground,                               t->background);
    lf->setColour (searchpathsBackgroundAlternate,                      t->backgroundAlternate);
    lf->setColour (searchpathsText,                                     t->textSystem);
    lf->setColour (searchpathsTextHighlighted,                          t->textHighlighted);
    lf->setColour (devicesParameterBackground,                          t->background);
    lf->setColour (devicesParameterText,                                t->textSystem);
    lf->setColour (devicesComboBoxBackground,                           t->background);
    lf->setColour (devicesComboBoxBackgroundActive,                     t->backgroundAlternate);
    lf->setColour (devicesComboBoxArrow,                                t->textSystem);
    lf->setColour (tooltipBackground,                                   t->backgroundMenu);
    lf->setColour (tooltipText,                                         t->textMenu);
    lf->setColour (callOutBoxBackground,                                t->backgroundMenu);
    lf->setColour (callOutBoxOutline,                                   t->textSystem);
    lf->setColour (alertWindowBackground,                               t->backgroundMenu);
    lf->setColour (alertWindowOutline,                                  t->textSystem);
    lf->setColour (alertWindowText,                                     t->textMenu);
    lf->setColour (alertWindowIcon,                                     t->textError);
    lf->setColour (alertWindowButtonBackground,                         t->backgroundWidget);
    lf->setColour (alertWindowButtonBackgroundOver,                     t->textError);
    lf->setColour (alertWindowButtonOutline,                            t->textSystem);
    lf->setColour (alertWindowButtonText,                               t->textSystem);
    lf->setColour (alertWindowButtonTextHighlighted,                    t->textMenu);
    lf->setColour (parametersHeaderBackground,                          t->background);
    lf->setColour (parametersHeaderText,                                t->textMenu);
    lf->setColour (parametersHeaderArrow,                               t->textSystem);
    lf->setColour (parametersParameterBackground,                       t->background);
    lf->setColour (parametersParameterText,                             t->textSystem);
    lf->setColour (parametersSliderBackground,                          t->backgroundAlternate);
    lf->setColour (parametersSliderTrack,                               t->backgroundWidget);
    lf->setColour (parametersColourBackground,                          t->backgroundAlternate);
    lf->setColour (parametersColourText,                                t->textSystem);
    lf->setColour (parametersBoolean,                                   t->textSystem);
    lf->setColour (parametersBooleanTick,                               t->textMenu);
    
    lf->setColour (windowBackground,                                    lf->findColour (toolbarBackground));
    
    /* Scroll bar. */
    
    lf->setColour (juce::ScrollBar::thumbColourId,                      t->thumb);
    lf->setColour (juce::ListBox::backgroundColourId,                   t->background);
    
    /* Text based parameters. */
    
    lf->setColour (juce::TextPropertyComponent::textColourId,           t->textSystem);
    lf->setColour (juce::TextPropertyComponent::outlineColourId,        t->transparent);
    lf->setColour (juce::TextPropertyComponent::backgroundColourId,     t->backgroundAlternate);
                   
    lf->setColour (juce::TextEditor::highlightColourId,                 t->transparent);
    lf->setColour (juce::TextEditor::highlightedTextColourId,           t->textHighlighted);
    lf->setColour (juce::CaretComponent::caretColourId,                 t->textMenu);
    lf->setColour (juce::Label::backgroundWhenEditingColourId,          t->transparent);
    lf->setColour (juce::Label::textWhenEditingColourId,                t->textSystem);
    lf->setColour (juce::Label::outlineWhenEditingColourId,             t->transparent);
    
    /* Boolean parameters. */
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     t->transparent);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  t->backgroundAlternate);
    
    /* Slider parameters. */
    
    lf->setColour (juce::Label::textColourId,                           t->textSystem);
    
    /* Colour selector sliders. */
    
    lf->setColour (juce::Slider::backgroundColourId,                    t->backgroundWidget);
    lf->setColour (juce::Slider::trackColourId,                         t->background);
    lf->setColour (juce::Slider::thumbColourId,                         t->thumb);
    
    /* Devices combo box. */
    
    lf->setColour (juce::ComboBox::textColourId,                        t->textSystem);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
