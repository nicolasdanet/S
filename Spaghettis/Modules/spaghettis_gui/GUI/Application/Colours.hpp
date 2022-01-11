
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
    preferencesHeaderBackground,
    preferencesHeaderText,
    preferencesHeaderArrow,
    preferencesParameterBackground,
    preferencesParameterText,
    preferencesSliderBackground,
    preferencesSliderTrack,
    preferencesColourBackground,
    preferencesColourText,
    preferencesBoolean,
    preferencesBooleanTick
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    /* < https://ethanschoonover.com/solarized/ > */
    
    // const juce::Colour base03                = juce::Colour (0xff002b36);
    // const juce::Colour base02                = juce::Colour (0xff073642);
    const juce::Colour base01                   = juce::Colour (0xff586e75);
    // const juce::Colour base00                = juce::Colour (0xff657b83);
    // const juce::Colour base0                 = juce::Colour (0xff839496);
    const juce::Colour base1                    = juce::Colour (0xff93a1a1);
    // const juce::Colour base2                 = juce::Colour (0xffeee8d5);
    // const juce::Colour base3                 = juce::Colour (0xfffdf6e3);
    const juce::Colour yellow                   = juce::Colour (0xffb58900);
    const juce::Colour orange                   = juce::Colour (0xffcb4b16);
    // const juce::Colour red                   = juce::Colour (0xffdc322f);
    // const juce::Colour magenta               = juce::Colour (0xffd33682);
    // const juce::Colour violet                = juce::Colour (0xff6c71c4);
    // const juce::Colour blue                  = juce::Colour (0xff268bd2);
    // const juce::Colour cyan                  = juce::Colour (0xff2aa198);
    const juce::Colour green                    = juce::Colour (0xff859900);
        
    const juce::Colour background               = juce::Colour (0xff1f2029);

    #if JUCE_LINUX
    const juce::Colour backgroundDark2          = background.darker (0.75);
    const juce::Colour backgroundDark1          = background.darker (0.50);
    const juce::Colour backgroundDark0          = background.darker (0.25);
    const juce::Colour backgroundDark           = background.darker (0.10);
    const juce::Colour backgroundBright         = background.brighter (0.15);
    #else
    const juce::Colour backgroundDark2          = background.darker (0.50);
    const juce::Colour backgroundDark1          = background.darker (0.25);
    const juce::Colour backgroundDark0          = background.darker (0.25);
    const juce::Colour backgroundDark           = background.darker (0.10);
    const juce::Colour backgroundBright         = background.brighter (0.15);
    #endif
    
    jassert (background.isOpaque());
    
    for (int i = windowBackground; i < preferencesBooleanTick; ++i) {
        jassert (lf->isColourSpecified (i) == false);
    }
    
    lf->setColour (consoleTextDefault,                                  base01);
    lf->setColour (consoleTextSystem,                                   base1);
    lf->setColour (consoleTextWarning,                                  yellow);
    lf->setColour (consoleTextError,                                    orange);
    lf->setColour (consoleTextHighlighted,                              green);
    lf->setColour (consoleBackground,                                   background);
    lf->setColour (consoleBackgroundAlternate,                          backgroundDark);
    lf->setColour (menubarBackground,                                   backgroundDark1);
    lf->setColour (menubarText,                                         juce::Colours::white);
    lf->setColour (menubarPopupBackground,                              backgroundBright);
    lf->setColour (menubarPopupBackgroundHighlighted,                   orange);
    lf->setColour (menubarPopupText,                                    juce::Colours::lightgrey);
    lf->setColour (menubarPopupTextHighlighted,                         juce::Colours::white);
    lf->setColour (menubarPopupSeparator,                               juce::Colours::black);
    lf->setColour (toolbarBackground,                                   backgroundDark1);
    lf->setColour (toolbarBackgroundDown,                               orange);
    lf->setColour (toolbarBackgroundOver,                               backgroundDark1);
    lf->setColour (toolbarIconOn,                                       juce::Colours::white);
    lf->setColour (toolbarIconOff,                                      juce::Colours::grey);
    lf->setColour (searchpathsBackground,                               background);
    lf->setColour (searchpathsBackgroundAlternate,                      backgroundDark);
    lf->setColour (searchpathsText,                                     base1);
    lf->setColour (searchpathsTextHighlighted,                          green);
    lf->setColour (devicesParameterBackground,                          background);
    lf->setColour (devicesParameterText,                                base1);
    lf->setColour (devicesComboBoxBackground,                           backgroundDark);
    lf->setColour (devicesComboBoxBackgroundActive,                     backgroundDark0);
    lf->setColour (devicesComboBoxArrow,                                base1);
    lf->setColour (tooltipBackground,                                   backgroundDark1);
    lf->setColour (tooltipText,                                         juce::Colours::white);
    lf->setColour (callOutBoxBackground,                                backgroundDark1);
    lf->setColour (callOutBoxOutline,                                   base1);
    lf->setColour (alertWindowBackground,                               backgroundDark1);
    lf->setColour (alertWindowOutline,                                  base1);
    lf->setColour (alertWindowText,                                     juce::Colours::white);
    lf->setColour (preferencesHeaderBackground,                         background);
    lf->setColour (preferencesHeaderText,                               juce::Colours::white);
    lf->setColour (preferencesHeaderArrow,                              base1);
    lf->setColour (preferencesParameterBackground,                      background);
    lf->setColour (preferencesParameterText,                            base1);
    lf->setColour (preferencesSliderBackground,                         backgroundDark);
    lf->setColour (preferencesSliderTrack,                              backgroundDark2);
    lf->setColour (preferencesColourBackground,                         backgroundDark);
    lf->setColour (preferencesColourText,                               base1);
    lf->setColour (preferencesBoolean,                                  base1);
    lf->setColour (preferencesBooleanTick,                              juce::Colours::white);
    
    lf->setColour (windowBackground,                                    lf->findColour (toolbarBackground));
    
    /* Scroll bar. */
    
    lf->setColour (juce::ScrollBar::thumbColourId,                      juce::Colours::grey);
    lf->setColour (juce::ListBox::backgroundColourId,                   background);
    
    /* Text based preferences. */
    
    lf->setColour (juce::TextPropertyComponent::textColourId,           base1);
    lf->setColour (juce::TextPropertyComponent::outlineColourId,        juce::Colours::transparentBlack);
    lf->setColour (juce::TextPropertyComponent::backgroundColourId,     backgroundDark);
                   
    lf->setColour (juce::TextEditor::highlightColourId,                 juce::Colours::transparentBlack);
    lf->setColour (juce::TextEditor::highlightedTextColourId,           green);
    lf->setColour (juce::CaretComponent::caretColourId,                 juce::Colours::white);
    lf->setColour (juce::Label::backgroundWhenEditingColourId,          juce::Colours::transparentBlack);
    lf->setColour (juce::Label::textWhenEditingColourId,                base1);
    lf->setColour (juce::Label::outlineWhenEditingColourId,             juce::Colours::transparentBlack);
    
    /* Boolean preferences. */
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     juce::Colours::transparentBlack);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  backgroundDark);
    
    /* Colour selector sliders. */
    
    lf->setColour (juce::Slider::backgroundColourId,                    backgroundDark2);
    lf->setColour (juce::Slider::trackColourId,                         background);
    lf->setColour (juce::Slider::thumbColourId,                         juce::Colours::grey);
    
    /* Slider preferences. */
    
    lf->setColour (juce::Label::textColourId,                           base1);
    
    /* Devices combo box. */
    
    lf->setColour (juce::ComboBox::textColourId,                        base1);
    
    /* Dialog boxes on Linux. */
    
    lf->setColour (juce::TextButton::buttonColourId,                    backgroundDark2);
    lf->setColour (juce::TextButton::textColourOffId,                   juce::Colours::white);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
