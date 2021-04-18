
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SPAGHETTIS_COLOUR(c)    (c | 0xff000000)

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

enum ColourIds {
    consoleTextDefault              = SPAGHETTIS_COLOUR (1),
    consoleTextWarning              = SPAGHETTIS_COLOUR (2),
    consoleTextError                = SPAGHETTIS_COLOUR (3),
    consoleBackground               = SPAGHETTIS_COLOUR (4),
    consoleHighlight                = SPAGHETTIS_COLOUR (5),
    menubarBackground               = SPAGHETTIS_COLOUR (6),
    menubarBackgroundHighlighted    = SPAGHETTIS_COLOUR (7),
    menubarText                     = SPAGHETTIS_COLOUR (8),
    menubarSeparator                = SPAGHETTIS_COLOUR (9)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    jassert (lf->isColourSpecified (consoleTextDefault)             == false);
    jassert (lf->isColourSpecified (consoleTextWarning)             == false);
    jassert (lf->isColourSpecified (consoleTextError)               == false);
    jassert (lf->isColourSpecified (consoleBackground)              == false);
    jassert (lf->isColourSpecified (consoleHighlight)               == false);
    jassert (lf->isColourSpecified (menubarBackground)              == false);
    jassert (lf->isColourSpecified (menubarBackgroundHighlighted)   == false);
    jassert (lf->isColourSpecified (menubarText)                    == false);
    jassert (lf->isColourSpecified (menubarSeparator)               == false);
    
    auto console = juce::Colour (0xff323e44);
    
    lf->setColour (consoleTextDefault,                  juce::Colours::white);
    lf->setColour (consoleTextWarning,                  juce::Colours::orange);
    lf->setColour (consoleTextError,                    juce::Colours::red);
    lf->setColour (consoleBackground,                   console);
    lf->setColour (consoleHighlight,                    juce::Colours::transparentBlack);
    lf->setColour (menubarBackground,                   console.darker (0.50));
    lf->setColour (menubarBackgroundHighlighted,        console.brighter (0.05));
    lf->setColour (menubarText,                         juce::Colours::white);
    lf->setColour (menubarSeparator,                    juce::Colours::black);
    
    lf->setColour (juce::TextEditor::outlineColourId,               juce::Colours::transparentBlack);
    lf->setColour (juce::TextEditor::shadowColourId,                juce::Colours::transparentBlack);
    lf->setColour (juce::ScrollBar::thumbColourId,                  juce::Colours::grey);
    lf->setColour (juce::PopupMenu::backgroundColourId,             console.darker (0.50));
    lf->setColour (juce::PopupMenu::highlightedBackgroundColourId,  console.brighter (0.05));
    lf->setColour (juce::PopupMenu::textColourId,                   juce::Colours::white);
    lf->setColour (juce::PopupMenu::highlightedTextColourId,        juce::Colours::white);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
