
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class LNF : public juce::LookAndFeel_V4 {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    LNF();
    
    ~LNF() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void drawArrowDown (juce::Graphics&, const juce::Rectangle<int>&);
    static void drawArrowUp (juce::Graphics&, const juce::Rectangle<int>&);
    static void drawArrowRight (juce::Graphics&, const juce::Rectangle<int>&);

public:
    static void drawTick (juce::Graphics&, const juce::Rectangle<int>&);
    static void drawCross (juce::Graphics&, const juce::Rectangle<int>&);
    
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour&, bool, bool) override;
    void drawButtonText (juce::Graphics&, juce::TextButton&, bool, bool) override;
    
public:
    juce::Font getTextButtonFont();
    juce::Font getTextButtonFont (juce::TextButton&, int) override;

public:
    static int getButtonHeight();
    static bool isButtonInsideAlertWindow (const juce::Button&);
    
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
    juce::Font getComboBoxFont (juce::ComboBox&) override;

    void drawComboBox (juce::Graphics&, int, int, bool, int, int, int, int, juce::ComboBox&) override;
    void positionComboBoxText (juce::ComboBox&, juce::Label&) override;
    void drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&) override;

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
    void drawPopupMenuItemTick (juce::Graphics&, juce::Rectangle<int>&);
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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LNF)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
