
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
        int,
        int,
        int,
        const juce::String&,
        bool,
        bool,
        bool,
        juce::MenuBarComponent&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Font getPopupMenuFont() override
    {
        return font_;
    }
    
    void drawPopupMenuBackground (juce::Graphics& g, int, int) override
    {
        g.fillAll (findColour (Colours::menubarBackground));
    }

    int getPopupMenuBorderSize() override
    {
        return 0;
    }
    
    void getIdealPopupMenuItemSize (const juce::String&,
        const bool,
        int,
        int&,
        int&) override;
    
    void drawPopupMenuItem (juce::Graphics& g,
        const juce::Rectangle<int>&,
        const bool,
        const bool,
        const bool,
        const bool,
        const bool,
        const juce::String&,
        const juce::String&,
        const juce::Drawable*,
        const juce::Colour* const) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void drawPopupMenuItemSelector (juce::Graphics&, const juce::Rectangle<int>&);
    void drawPopupMenuItemBackground (juce::Graphics& , const juce::Rectangle<int>&);
    void drawPopupMenuItemTick (juce::Graphics&, const juce::Rectangle<int>&);
    void drawPopupMenuItemSubMenu (juce::Graphics&, juce::Rectangle<int>&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
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
