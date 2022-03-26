
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
    
        // const juce::Colour base03    = juce::Colour (0xff002b36);
        // const juce::Colour base02    = juce::Colour (0xff073642);
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
        
        text                            = base1;
        textAlternate                   = base01;
        textWarning                     = yellow;
        textError                       = orange;
        textHighlighted                 = green;
        
        background                      = juce::Colour (0xff1f2029);
        backgroundAlternate             = background.darker (0.10);
        backgroundPopup                 = background.brighter (0.15);
        
        #if JUCE_LINUX
        backgroundAlert                 = background.darker (0.50);
        backgroundWidget                = background.darker (0.75);
        #else
        backgroundAlert                 = background.darker (0.25);
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
    juce::Colour text;
    juce::Colour textAlternate;
    juce::Colour textWarning;
    juce::Colour textError;
    juce::Colour textHighlighted;
    juce::Colour background;
    juce::Colour backgroundAlternate;
    juce::Colour backgroundPopup;
    juce::Colour backgroundAlert;
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
    
    lf->setColour (consoleTextDefault,                                  t->textAlternate);
    lf->setColour (consoleTextSystem,                                   t->text);
    lf->setColour (consoleTextWarning,                                  t->textWarning);
    lf->setColour (consoleTextError,                                    t->textError);
    lf->setColour (consoleTextHighlighted,                              t->textHighlighted);
    lf->setColour (consoleBackground,                                   t->background);
    lf->setColour (consoleBackgroundAlternate,                          t->backgroundAlternate);
    lf->setColour (menubarBackground,                                   t->backgroundAlert);
    lf->setColour (menubarText,                                         juce::Colours::white);
    lf->setColour (menubarPopupBackground,                              t->backgroundPopup);
    lf->setColour (menubarPopupBackgroundHighlighted,                   t->textError);
    lf->setColour (menubarPopupText,                                    juce::Colours::lightgrey);
    lf->setColour (menubarPopupTextHighlighted,                         juce::Colours::white);
    lf->setColour (menubarPopupSeparator,                               juce::Colours::black);
    lf->setColour (toolbarBackground,                                   t->backgroundAlert);
    lf->setColour (toolbarBackgroundDown,                               t->textError);
    lf->setColour (toolbarBackgroundOver,                               t->backgroundAlert);
    lf->setColour (toolbarIconOn,                                       juce::Colours::white);
    lf->setColour (toolbarIconOff,                                      juce::Colours::grey);
    lf->setColour (searchpathsBackground,                               t->background);
    lf->setColour (searchpathsBackgroundAlternate,                      t->backgroundAlternate);
    lf->setColour (searchpathsText,                                     t->text);
    lf->setColour (searchpathsTextHighlighted,                          t->textHighlighted);
    lf->setColour (devicesParameterBackground,                          t->background);
    lf->setColour (devicesParameterText,                                t->text);
    lf->setColour (devicesComboBoxBackground,                           t->background);
    lf->setColour (devicesComboBoxBackgroundActive,                     t->backgroundAlternate);
    lf->setColour (devicesComboBoxArrow,                                t->text);
    lf->setColour (tooltipBackground,                                   t->backgroundAlert);
    lf->setColour (tooltipText,                                         juce::Colours::white);
    lf->setColour (callOutBoxBackground,                                t->backgroundAlert);
    lf->setColour (callOutBoxOutline,                                   t->text);
    lf->setColour (alertWindowBackground,                               t->backgroundAlert);
    lf->setColour (alertWindowOutline,                                  t->text);
    lf->setColour (alertWindowText,                                     juce::Colours::white);
    lf->setColour (alertWindowIcon,                                     t->textError);
    lf->setColour (alertWindowButtonBackground,                         t->backgroundWidget);
    lf->setColour (alertWindowButtonBackgroundOver,                     t->textError);
    lf->setColour (alertWindowButtonOutline,                            t->text);
    lf->setColour (alertWindowButtonText,                               t->text);
    lf->setColour (alertWindowButtonTextHighlighted,                    juce::Colours::white);
    lf->setColour (parametersHeaderBackground,                          t->background);
    lf->setColour (parametersHeaderText,                                juce::Colours::white);
    lf->setColour (parametersHeaderArrow,                               t->text);
    lf->setColour (parametersParameterBackground,                       t->background);
    lf->setColour (parametersParameterText,                             t->text);
    lf->setColour (parametersSliderBackground,                          t->backgroundAlternate);
    lf->setColour (parametersSliderTrack,                               t->backgroundWidget);
    lf->setColour (parametersColourBackground,                          t->backgroundAlternate);
    lf->setColour (parametersColourText,                                t->text);
    lf->setColour (parametersBoolean,                                   t->text);
    lf->setColour (parametersBooleanTick,                               juce::Colours::white);
    
    lf->setColour (windowBackground,                                    lf->findColour (toolbarBackground));
    
    /* Scroll bar. */
    
    lf->setColour (juce::ScrollBar::thumbColourId,                      juce::Colours::grey);
    lf->setColour (juce::ListBox::backgroundColourId,                   t->background);
    
    /* Text based parameters. */
    
    lf->setColour (juce::TextPropertyComponent::textColourId,           t->text);
    lf->setColour (juce::TextPropertyComponent::outlineColourId,        juce::Colours::transparentBlack);
    lf->setColour (juce::TextPropertyComponent::backgroundColourId,     t->backgroundAlternate);
                   
    lf->setColour (juce::TextEditor::highlightColourId,                 juce::Colours::transparentBlack);
    lf->setColour (juce::TextEditor::highlightedTextColourId,           t->textHighlighted);
    lf->setColour (juce::CaretComponent::caretColourId,                 juce::Colours::white);
    lf->setColour (juce::Label::backgroundWhenEditingColourId,          juce::Colours::transparentBlack);
    lf->setColour (juce::Label::textWhenEditingColourId,                t->text);
    lf->setColour (juce::Label::outlineWhenEditingColourId,             juce::Colours::transparentBlack);
    
    /* Boolean parameters. */
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     juce::Colours::transparentBlack);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  t->backgroundAlternate);
    
    /* Slider parameters. */
    
    lf->setColour (juce::Label::textColourId,                           t->text);
    
    /* Colour selector sliders. */
    
    lf->setColour (juce::Slider::backgroundColourId,                    t->backgroundWidget);
    lf->setColour (juce::Slider::trackColourId,                         t->background);
    lf->setColour (juce::Slider::thumbColourId,                         juce::Colours::grey);
    
    /* Devices combo box. */
    
    lf->setColour (juce::ComboBox::textColourId,                        t->text);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
