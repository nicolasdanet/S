
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

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
        const juce::Colour base0        = juce::Colour (0xff839496);
        const juce::Colour base1        = juce::Colour (0xff93a1a1);
        // const juce::Colour base2     = juce::Colour (0xffeee8d5);
        // const juce::Colour base3     = juce::Colour (0xfffdf6e3);
        const juce::Colour yellow       = juce::Colour (0xffb58900);
        const juce::Colour orange       = juce::Colour (0xffcb4b16);
        const juce::Colour red          = juce::Colour (0xffdc322f);
        // const juce::Colour magenta   = juce::Colour (0xffd33682);
        const juce::Colour violet       = juce::Colour (0xff6c71c4);
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
        textComment                     = base0;
        vuCold                          = green;
        vuWarm                          = yellow;
        vuHot                           = red;
        selection                       = green;
        thumb                           = juce::Colours::grey;
        transparent                     = juce::Colours::transparentBlack;
        signal                          = yellow;
        bang                            = violet;
        
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
    static Palette& getInstance()
    {
        static Palette p; return p;
    }
    
public:
    juce::Colour textDefault;
    juce::Colour textSystem;
    juce::Colour textWarning;
    juce::Colour textError;
    juce::Colour textHighlighted;
    juce::Colour textMenu;
    juce::Colour textPopup;
    juce::Colour textBox;
    juce::Colour textComment;
    juce::Colour vuCold;
    juce::Colour vuWarm;
    juce::Colour vuHot;
    juce::Colour selection;
    juce::Colour thumb;
    juce::Colour transparent;
    juce::Colour signal;
    juce::Colour bang;
    juce::Colour background;
    juce::Colour backgroundAlternate;
    juce::Colour backgroundPopup;
    juce::Colour backgroundBox;
    juce::Colour backgroundMenu;
    juce::Colour backgroundWidget;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
