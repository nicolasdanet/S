
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
    explicit LookAndFeel()
    {
        Colours::initialize (this);
        
        initializeFonts();
    }
    
    ~LookAndFeel() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getWindowTitleHeight (juce::Component *);
    
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
        return getMenuFont();
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
        #if JUCE_LINUX
            return 30;
        #else
            return 32;
        #endif
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
        return getMenuFont();
    }
    
    void drawPopupMenuBackgroundWithOptions (juce::Graphics&,
        int,
        int,
        const juce::PopupMenu::Options&) override;

    int getPopupMenuBorderSize() override
    {
        return 0;
    }
    
    void getIdealPopupMenuItemSize (const juce::String&,
        const bool,
        int,
        int&,
        int&) override;
    
    void drawPopupMenuItemWithOptions (juce::Graphics&,
        const juce::Rectangle<int>&,
        bool,
        const juce::PopupMenu::Item&,
        const juce::PopupMenu::Options&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void drawPopupMenuBackgroundProceed (juce::Graphics&, bool);

private:
    void drawPopupMenuItemSelector (juce::Graphics&, const juce::Rectangle<int>&);
    void drawPopupMenuItemBackground (juce::Graphics& , const juce::Rectangle<int>&, bool);
    void drawPopupMenuItemTick (juce::Graphics&, juce::Rectangle<int>);
    void drawPopupMenuItemSubMenu (juce::Graphics&, juce::Rectangle<int>&);
    void drawPopupMenuItemShortcut (juce::Graphics&, const juce::Rectangle<int>&, const juce::String&);
    void drawPopupMenuItemProceed (juce::Graphics&,
        const juce::Rectangle<int>&,
        const bool,
        const bool,
        const bool,
        const bool,
        const bool,
        const bool,
        const juce::String&,
        const juce::String&);
        
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Font getComboBoxFont()
    {
        return getMenuFont();
    }
    
    juce::Font getComboBoxFont (juce::ComboBox& box) override
    {
        return getComboBoxFont();
    }

    void drawComboBox (juce::Graphics&, int, int, bool, int, int, int, int, juce::ComboBox&) override;
    void positionComboBoxText (juce::ComboBox&, juce::Label&) override;
    void drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drawTooltip (juce::Graphics&, const juce::String&, int, int) override;
        
    juce::Rectangle<int> getTooltipBounds (const juce::String&,
        juce::Point<int>,
        juce::Rectangle<int>) override;

private:
    juce::TextLayout getTooltipLayout (const juce::String&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drawPropertyComponentBackground (juce::Graphics&, int, int, juce::PropertyComponent&) override;
    void drawPropertyComponentLabel (juce::Graphics&, int, int, juce::PropertyComponent&) override;
    
    int getPropertyPanelSectionHeaderHeight (const juce::String& s) override
    {
        if (s.isEmpty()) { return 0; } else { return getPropertyPanelHeight(); }
    }

    int getPropertyPanelHeight()
    {
        return static_cast<int> (getConsoleFont().getHeight() * 1.5);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drawToggleButton (juce::Graphics&, juce::ToggleButton&, bool, bool) override;
    
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    
    juce::Font getLabelFont (juce::Label&) override
    {
        return getConsoleFont();
    }
    
    juce::BorderSize<int> getLabelBorderSize (juce::Label&) override
    {
        /* Avoid the text to drift down when editing in preferences. */
        
        #if JUCE_RPI
        
        return juce::BorderSize<int> { 2, 5, 0, 5 };
        
        #else
        
        return juce::BorderSize<int> { 3, 5, 0, 5 };
        
        #endif
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drawLinearSlider (juce::Graphics&,
        int,
        int,
        int,
        int,
        float,
        float,
        float,
        const juce::Slider::SliderStyle,
        juce::Slider&) override;
    
    juce::Label* createSliderTextBox (juce::Slider&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drawCallOutBoxBackground (juce::CallOutBox&,
        juce::Graphics&,
        const juce::Path&,
        juce::Image&) override;
                                               
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void drawArrowOpened (juce::Graphics&, const juce::Rectangle<int>&);
    static void drawArrowClosed (juce::Graphics&, const juce::Rectangle<int>&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::Colour getColourFromValue (const juce::Value& v)
    {
        const juce::String name (v.toString());
        
        if (name.length() == 8) { return juce::Colour::fromString (name); }
        else {
            jassertfalse; return juce::Colours::black;
        }
    }

    static void setValueWithColour (juce::Value& v, const juce::Colour& c)
    {
        v.setValue (c.toString().paddedLeft ('0', 8));
    }
    
    static juce::String getDisplayStringFromColour (const juce::Colour& colour)
    {
        return colour.toDisplayString (true);
    }
    
    static juce::Colour getColorFromString (const juce::String& s)
    {
        return juce::Colour::fromString (s.length() == 8 ? s : "ff000000");
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::String initializeFontsBest (const juce::StringArray&);
    void initializeFonts();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const juce::Font& getConsoleFont() const
    {
        return font18_;
    }
    
    const juce::Font& getMenuFont() const
    {
        return font18_;
    }
    
    const juce::Font& getColourFont() const
    {
        return font18Monospaced_;
    }
    
    const juce::Font& getMenuShortcutsFont() const
    {
        return font18Monospaced_;
    }

    const juce::Font& getTooltipsFont() const
    {
        return font16Bold_;
    }

private:
    juce::String font18Name_;
    juce::String font18MonospacedName_;
    juce::String font16BoldName_;
    juce::Font font18_;
    juce::Font font18Monospaced_;
    juce::Font font16Bold_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SliderLabel : public juce::Label {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit SliderLabel()
    {
        setJustificationType (juce::Justification::centred);
    }
    
    ~SliderLabel() = default;

public:
    void mouseWheelMove (const juce::MouseEvent&, const juce::MouseWheelDetails&) override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLabel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
