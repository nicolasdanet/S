
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class LookAndFeel : public juce::LookAndFeel_V4 {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    LookAndFeel()
    {
        #if defined ( JUCE_MAC )
        
            font_ = juce::Font (18.0);
        
        #else
            
            font_ = juce::Font (18.0, juce::Font::bold);
            
        #endif
        
        Colours::initialize (this);
    }
    
    ~LookAndFeel() = default;

public:
    int getDefaultMenuBarHeight() override
    {
        return 26;
    }

    juce::Font getMenuBarFont (juce::MenuBarComponent&, int, const juce::String&) override
    {
        return font_;
    }

    void drawMenuBarBackground (juce::Graphics& g, int, int, bool, juce::MenuBarComponent&) override
    {
        g.fillAll (findColour (Colours::menubarBackground));
    }

    void drawMenuBarItem (juce::Graphics& g,
        int width,
        int height,
        int itemIndex,
        const juce::String& itemText,
        bool isMouseOverItem,
        bool isMenuOpen,
        bool /*isMouseOverBar*/,
        juce::MenuBarComponent& m) override
    {
        const bool highlighted = (isMenuOpen || isMouseOverItem) && m.isEnabled();
        
        if (highlighted) { g.fillAll (findColour (Colours::menubarBackgroundHighlighted)); }

        g.setColour (findColour (Colours::menubarText));
        g.setFont (getMenuBarFont (m, itemIndex, itemText));
        g.drawFittedText (itemText, 0, 0, width, height, juce::Justification::centred, 1);
    }

    juce::Font getPopupMenuFont() override
    {
        return font_;
    }

    const juce::Font& getConsoleFont() const
    {
        return font_;
    }
    
private:
    juce::Font font_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
