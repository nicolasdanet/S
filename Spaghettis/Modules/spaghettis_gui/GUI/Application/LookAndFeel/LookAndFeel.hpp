
/* Copyright (c) 2021 Jojo and others. */

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
    explicit LookAndFeel();
    
    ~LookAndFeel() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void initializeFonts();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    juce::Font getConsoleFont() const;
    juce::Font getMenuFont() const;
    juce::Font getColourFont() const;
    juce::Font getMenuShortcutsFont() const;
    juce::Font getTooltipsFont() const;
    
    juce::Font getListBoxFont() const;
    
    juce::Font getObjectsFont (float);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getWindowTitleHeight (juce::Component *);

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
    juce::AlertWindow* createAlertWindow (const juce::String&,
        const juce::String&,
        const juce::String&,
        const juce::String&,
        const juce::String&,
        juce::MessageBoxIconType,
        int,
        juce::Component*) override;

    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour&, bool, bool) override;
    void drawButtonText (juce::Graphics&, juce::TextButton&, bool, bool) override;
    
    void drawAlertBox (juce::Graphics&,
        juce::AlertWindow&,
        const juce::Rectangle<int>&,
        juce::TextLayout&) override;

private:
    void drawAlertBoxBackground (juce::Graphics&, juce::Rectangle<int>, float);
    void drawAlertBoxIcon (juce::Graphics&, juce::Rectangle<int>, juce::AlertWindow&);
    void drawAlertBoxText (juce::Graphics&, juce::Rectangle<int>, const juce::TextLayout&);
    
public:
    int getAlertWindowButtonHeight() override;
    juce::Font getAlertWindowTitleFont() override;
    juce::Font getAlertWindowMessageFont() override;
    juce::Font getAlertWindowFont() override;

public:
    juce::Font getTextButtonFont();
    juce::Font getTextButtonFont (juce::TextButton&, int) override;

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
    juce::Font getComboBoxFont() const;
    juce::Font getComboBoxFont (juce::ComboBox&) override;

    void drawComboBox (juce::Graphics&, int, int, bool, int, int, int, int, juce::ComboBox&) override;
    void positionComboBoxText (juce::ComboBox&, juce::Label&) override;
    void drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Font getMakerEntryFont() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getDefaultMenuBarHeight() override;
    juce::Font getMenuBarFont (juce::MenuBarComponent&, int, const juce::String&) override;
    void drawMenuBarBackground (juce::Graphics&, int, int, bool, juce::MenuBarComponent&) override;

    void drawMenuBarItem (juce::Graphics&,
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
    juce::Font getPopupMenuFont() override;
    int getPopupMenuBorderSize() override;

    void drawPopupMenuBackgroundWithOptions (juce::Graphics&,
        int,
        int,
        const juce::PopupMenu::Options&) override;
    
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

private:
    void drawPopupMenuBackgroundProceed (juce::Graphics&);

private:
    void drawPopupMenuItemSelector (juce::Graphics&, const juce::Rectangle<int>&);
    void drawPopupMenuItemBackground (juce::Graphics& , const juce::Rectangle<int>&);
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
    juce::Rectangle<int> getPropertyComponentContentPosition (juce::PropertyComponent&) override;
    
    void drawPropertyComponentBackground (juce::Graphics&, int, int, juce::PropertyComponent&) override;
    void drawPropertyComponentLabel (juce::Graphics&, int, int, juce::PropertyComponent&) override;
    
    int getPropertyPanelSectionHeaderHeight (const juce::String& s) override;
    int getPropertyPanelHeight();

public:
    void drawToggleButton (juce::Graphics&, juce::ToggleButton&, bool, bool) override;

public:
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    juce::Font getLabelFont (juce::Label&) override;
    juce::BorderSize<int> getLabelBorderSize (juce::Label&) override;

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
    void drawStretchableLayoutResizerBar (juce::Graphics&, int, int, bool, bool, bool) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getToolbarHeight();

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
    void drawTooltip (juce::Graphics&, const juce::String&, int, int) override;
        
    juce::Rectangle<int> getTooltipBounds (const juce::String&,
        juce::Point<int>,
        juce::Rectangle<int>) override;

private:
    juce::TextLayout getTooltipLayout (const juce::String&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::String font18Name_;
    juce::String font18MonospacedName_;
    juce::String font16BoldName_;
    juce::Font font18_;
    juce::Font font18Monospaced_;
    juce::Font font16Bold_;

private:
    std::vector<std::tuple<int, juce::Font>> fonts_;

private:
    int windowTitleHeight_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
