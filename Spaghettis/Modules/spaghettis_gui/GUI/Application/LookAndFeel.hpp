
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
        Colours::initialize (this);
        
        initializeFonts();
    }
    
    ~LookAndFeel() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getDefaultMenuBarHeight() override
    {
        return 28;
    }

    juce::Font getMenuBarFont (juce::MenuBarComponent&, int, const juce::String&) override
    {
        return getFontConsole();
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
    int getToolbarHeight()
    {
        return 30;
    }
    
    void paintToolbarBackground (juce::Graphics&, int w, int h, juce::Toolbar&) override;
    
    void paintToolbarButtonBackground (juce::Graphics&,
        int,
        int,
        bool,
        bool,
        juce::ToolbarItemComponent&) override;
                                                   
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Font getPopupMenuFont() override
    {
        return getFontConsole();
    }
    
    void drawPopupMenuBackground (juce::Graphics& g, int, int) override
    {
        g.fillAll (findColour (Colours::menubarBackgroundPopup));
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

public:
    void drawTooltip (juce::Graphics&, const juce::String&, int, int) override;
        
    juce::Rectangle<int> getTooltipBounds (const juce::String&,
        juce::Point<int>,
        juce::Rectangle<int>) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void drawPopupMenuItemSelector (juce::Graphics&, const juce::Rectangle<int>&);
    void drawPopupMenuItemBackground (juce::Graphics& , const juce::Rectangle<int>&);
    void drawPopupMenuItemTick (juce::Graphics&, juce::Rectangle<int>);
    void drawPopupMenuItemSubMenu (juce::Graphics&, juce::Rectangle<int>&);
    void drawPopupMenuItemShortcut (juce::Graphics&, const juce::Rectangle<int>&, const juce::String&);

private:
    juce::TextLayout getTooltipLayout (const juce::String&);
    
private:
    juce::String initializeFontsBest (const juce::StringArray&);
    void initializeFonts();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const juce::Font& getFontConsole() const
    {
        return fontConsole_;
    }
    
    const juce::Font& getFontShortcuts() const
    {
        return fontShortcuts_;
    }

    const juce::Font& getTooltipsFont() const
    {
        return fontTooltips_;
    }
    
private:
    juce::String fontConsoleName_;
    juce::String fontShortcutsName_;
    juce::String fontTooltipsName_;
    juce::Font fontConsole_;
    juce::Font fontShortcuts_;
    juce::Font fontTooltips_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
