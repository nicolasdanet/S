
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
    consoleTextDefault              = SPAGHETTIS_COLOUR (1),
    consoleTextSystem               = SPAGHETTIS_COLOUR (2),
    consoleTextWarning              = SPAGHETTIS_COLOUR (3),
    consoleTextError                = SPAGHETTIS_COLOUR (4),
    consoleBackground               = SPAGHETTIS_COLOUR (5),
    consoleHighlight                = SPAGHETTIS_COLOUR (6),
    menubarBackground               = SPAGHETTIS_COLOUR (7),
    menubarBackgroundPopup          = SPAGHETTIS_COLOUR (8),
    menubarBackgroundHighlighted    = SPAGHETTIS_COLOUR (9),
    menubarText                     = SPAGHETTIS_COLOUR (10),
    menubarSeparator                = SPAGHETTIS_COLOUR (11),
    searchpathsBackground           = SPAGHETTIS_COLOUR (12),
    searchpathsBackgroundAlternate  = SPAGHETTIS_COLOUR (13),
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    /* < https://ethanschoonover.com/solarized/ > */
    
    const juce::Colour background   = juce::Colour (0xff1f2029);
    
    const juce::Colour base01   = juce::Colour (0xff586e75);
    const juce::Colour base1    = juce::Colour (0xff93a1a1);
    const juce::Colour yellow   = juce::Colour (0xffb58900);
    const juce::Colour orange   = juce::Colour (0xffcb4b16);
    const juce::Colour green    = juce::Colour (0xff859900);
    
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
    
    jassert (lf->isColourSpecified (consoleTextDefault)             == false);
    jassert (lf->isColourSpecified (consoleTextSystem)              == false);
    jassert (lf->isColourSpecified (consoleTextWarning)             == false);
    jassert (lf->isColourSpecified (consoleTextError)               == false);
    jassert (lf->isColourSpecified (consoleBackground)              == false);
    jassert (lf->isColourSpecified (consoleHighlight)               == false);
    jassert (lf->isColourSpecified (menubarBackground)              == false);
    jassert (lf->isColourSpecified (menubarBackgroundPopup)         == false);
    jassert (lf->isColourSpecified (menubarBackgroundHighlighted)   == false);
    jassert (lf->isColourSpecified (menubarText)                    == false);
    jassert (lf->isColourSpecified (menubarSeparator)               == false);
    jassert (lf->isColourSpecified (searchpathsBackground)          == false);
    jassert (lf->isColourSpecified (searchpathsBackgroundAlternate) == false);
    
    lf->setColour (consoleTextDefault,                          base01);
    lf->setColour (consoleTextSystem,                           base1);
    lf->setColour (consoleTextWarning,                          yellow);
    lf->setColour (consoleTextError,                            orange);
    lf->setColour (consoleBackground,                           background);
    lf->setColour (consoleHighlight,                            juce::Colours::transparentBlack);
    lf->setColour (menubarBackground,                           background.darker (0.50));
    lf->setColour (menubarBackgroundPopup,                      background.darker (1.00));
    lf->setColour (menubarBackgroundHighlighted,                orange);
    lf->setColour (menubarText,                                 juce::Colours::white);
    lf->setColour (menubarSeparator,                            juce::Colours::black);
    lf->setColour (searchpathsBackground,                       background);
    lf->setColour (searchpathsBackgroundAlternate,              background.darker (0.10));
    
    lf->setColour (juce::TextEditor::highlightedTextColourId,   green);
    lf->setColour (juce::TextEditor::outlineColourId,           juce::Colours::transparentBlack);
    lf->setColour (juce::TextEditor::shadowColourId,            juce::Colours::transparentBlack);
    lf->setColour (juce::ScrollBar::thumbColourId,              juce::Colours::grey);
    lf->setColour (juce::ListBox::backgroundColourId,           background);
    lf->setColour (juce::ListBox::outlineColourId,              juce::Colours::transparentBlack);
    lf->setColour (juce::ListBox::textColourId,                 base01);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
