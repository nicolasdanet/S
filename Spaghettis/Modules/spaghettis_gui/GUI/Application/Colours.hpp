
/* Copyright (c) 2021 Nicolas Danet. */

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
    menubarBackgroundPopup,
    menubarBackgroundHighlighted,
    menubarText,
    menubarSeparator,
    toolbarBackground,
    toolbarBackgroundDown,
    toolbarBackgroundOver,
    toolbarIconOn,
    toolbarIconOff,
    searchpathsBackground,
    searchpathsBackgroundAlternate,
    searchpathsText,
    searchpathsTextHighlighted,
    tooltipBackground,
    tooltipText,
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
    
    const juce::Colour background               = juce::Colour (0xff1f2029);
    const juce::Colour backgroundAlternate      = background.darker (0.10);
    
    const juce::Colour base01                   = juce::Colour (0xff586e75);
    const juce::Colour base1                    = juce::Colour (0xff93a1a1);
    const juce::Colour yellow                   = juce::Colour (0xffb58900);
    const juce::Colour orange                   = juce::Colour (0xffcb4b16);
    const juce::Colour green                    = juce::Colour (0xff859900);

    #if JUCE_LINUX
    const juce::Colour backgroundDark           = background.darker (0.50);
    const juce::Colour orangeDark               = orange;
    #else
    const juce::Colour backgroundDark           = background.darker (0.25);
    const juce::Colour orangeDark               = orange.darker (0.25);
    #endif
    
    // const juce::Colour base03   = juce::Colour (0xff002b36);
    // const juce::Colour base02   = juce::Colour (0xff073642);
    // const juce::Colour base00   = juce::Colour (0xff657b83);
    // const juce::Colour base0    = juce::Colour (0xff839496);
    // const juce::Colour base2    = juce::Colour (0xffeee8d5);
    // const juce::Colour base3    = juce::Colour (0xfffdf6e3);
    // const juce::Colour red      = juce::Colour (0xffdc322f);
    // const juce::Colour magenta  = juce::Colour (0xffd33682);
    // const juce::Colour violet   = juce::Colour (0xff6c71c4);
    // const juce::Colour blue     = juce::Colour (0xff268bd2);
    // const juce::Colour cyan     = juce::Colour (0xff2aa198);
    
    jassert (background.isOpaque());
    
    for (int i = windowBackground ; i < preferencesBooleanTick; ++i) {
        jassert (lf->isColourSpecified (i) == false);
    }
    
    lf->setColour (consoleTextDefault,                                  base01);
    lf->setColour (consoleTextSystem,                                   base1);
    lf->setColour (consoleTextWarning,                                  yellow);
    lf->setColour (consoleTextError,                                    orange);
    lf->setColour (consoleTextHighlighted,                              green);
    lf->setColour (consoleBackground,                                   background);
    lf->setColour (consoleBackgroundAlternate,                          backgroundAlternate);
    lf->setColour (menubarBackground,                                   backgroundDark);
    lf->setColour (menubarBackgroundPopup,                              backgroundDark);
    lf->setColour (menubarBackgroundHighlighted,                        orange);
    lf->setColour (menubarText,                                         juce::Colours::white);
    lf->setColour (menubarSeparator,                                    juce::Colours::black);
    lf->setColour (toolbarBackground,                                   backgroundDark);
    lf->setColour (toolbarBackgroundDown,                               orangeDark);
    lf->setColour (toolbarBackgroundOver,                               backgroundDark);
    lf->setColour (toolbarIconOn,                                       juce::Colours::white);
    lf->setColour (toolbarIconOff,                                      juce::Colours::grey);
    lf->setColour (searchpathsBackground,                               background);
    lf->setColour (searchpathsBackgroundAlternate,                      backgroundAlternate);
    lf->setColour (searchpathsText,                                     base1);
    lf->setColour (searchpathsTextHighlighted,                          green);
    lf->setColour (tooltipBackground,                                   backgroundDark);
    lf->setColour (tooltipText,                                         juce::Colours::white);
    lf->setColour (preferencesHeaderBackground,                         background);
    lf->setColour (preferencesHeaderText,                               juce::Colours::white);
    lf->setColour (preferencesHeaderArrow,                              base1);
    lf->setColour (preferencesParameterBackground,                      background);
    lf->setColour (preferencesParameterText,                            base1);
    lf->setColour (preferencesSliderBackground,                         backgroundAlternate);
    lf->setColour (preferencesSliderTrack,                              background.darker (0.5));
    lf->setColour (preferencesColourBackground,                         backgroundAlternate);
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
    lf->setColour (juce::TextPropertyComponent::backgroundColourId,     backgroundAlternate);
                   
    lf->setColour (juce::TextEditor::highlightColourId,                 juce::Colours::transparentBlack);
    lf->setColour (juce::TextEditor::highlightedTextColourId,           green);
    lf->setColour (juce::CaretComponent::caretColourId,                 juce::Colours::white);
    lf->setColour (juce::Label::backgroundWhenEditingColourId,          backgroundAlternate);
    lf->setColour (juce::Label::textWhenEditingColourId,                base1);
    lf->setColour (juce::Label::outlineWhenEditingColourId,             juce::Colours::transparentBlack);
    
    /* Boolean preferences. */
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     juce::Colours::transparentBlack);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  backgroundAlternate);
    
    /* Slider preferences. */
    
    lf->setColour (juce::Label::textColourId,                           base1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
