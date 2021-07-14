
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
    preferencesBackground,
    preferencesText
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
    
    jassert (lf->isColourSpecified (windowBackground)                   == false);
    jassert (lf->isColourSpecified (consoleTextDefault)                 == false);
    jassert (lf->isColourSpecified (consoleTextSystem)                  == false);
    jassert (lf->isColourSpecified (consoleTextWarning)                 == false);
    jassert (lf->isColourSpecified (consoleTextError)                   == false);
    jassert (lf->isColourSpecified (consoleTextHighlighted)             == false);
    jassert (lf->isColourSpecified (consoleBackground)                  == false);
    jassert (lf->isColourSpecified (consoleBackgroundAlternate)         == false);
    jassert (lf->isColourSpecified (menubarBackground)                  == false);
    jassert (lf->isColourSpecified (menubarBackgroundPopup)             == false);
    jassert (lf->isColourSpecified (menubarBackgroundHighlighted)       == false);
    jassert (lf->isColourSpecified (menubarText)                        == false);
    jassert (lf->isColourSpecified (menubarSeparator)                   == false);
    jassert (lf->isColourSpecified (toolbarBackground)                  == false);
    jassert (lf->isColourSpecified (toolbarBackgroundDown)              == false);
    jassert (lf->isColourSpecified (toolbarBackgroundOver)              == false);
    jassert (lf->isColourSpecified (toolbarIconOn)                      == false);
    jassert (lf->isColourSpecified (toolbarIconOff)                     == false);
    jassert (lf->isColourSpecified (searchpathsBackground)              == false);
    jassert (lf->isColourSpecified (searchpathsBackgroundAlternate)     == false);
    jassert (lf->isColourSpecified (searchpathsText)                    == false);
    jassert (lf->isColourSpecified (searchpathsTextHighlighted)         == false);
    jassert (lf->isColourSpecified (tooltipBackground)                  == false);
    jassert (lf->isColourSpecified (tooltipText)                        == false);
    jassert (lf->isColourSpecified (preferencesBackground)              == false);
    jassert (lf->isColourSpecified (preferencesText)                    == false);
    
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
    lf->setColour (preferencesBackground,                               background);
    lf->setColour (preferencesText,                                     base1);
    
    lf->setColour (windowBackground,                                    lf->findColour (toolbarBackground));
    lf->setColour (juce::ScrollBar::thumbColourId,                      juce::Colours::grey);
    lf->setColour (juce::ListBox::backgroundColourId,                   background);
    
    //lf->setColour (juce::PropertyComponent::labelTextColourId,          base1);
    //lf->setColour (juce::PropertyComponent::backgroundColourId,         background);
    //lf->setColour (juce::TextPropertyComponent::textColourId,           base1);
    //lf->setColour (juce::TextPropertyComponent::outlineColourId,        juce::Colours::transparentBlack);
    //lf->setColour (juce::TextPropertyComponent::backgroundColourId,     background);
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     juce::Colours::transparentBlack);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  background);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
