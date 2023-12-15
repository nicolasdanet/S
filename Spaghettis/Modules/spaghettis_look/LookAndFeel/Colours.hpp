
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

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
    windowsBackground   = 0,
    consoleTextDefault,
    consoleTextSystem,
    consoleTextWarning,
    consoleTextError,
    menubarBackground,
    menubarText,
    menubarPopupBackground,
    menubarPopupBackgroundHighlighted,
    menubarPopupText,
    menubarPopupTextHighlighted,
    menubarPopupSeparator,
    toolbarBackground,
    toolbarBackgroundDown,
    toolbarBackgroundOver,
    toolbarIconOn,
    toolbarIconOff,
    toolbarZoom,
    devicesParameterBackground,
    devicesParameterText,
    devicesComboBoxBackground,
    devicesComboBoxBackgroundActive,
    devicesComboBoxArrow,
    tooltipBackground,
    tooltipText,
    resizerHighlighted,
    callOutBoxBackground,
    callOutBoxOutline,
    alertWindowBackground,
    alertWindowOutline,
    alertWindowText,
    alertWindowIcon,
    alertWindowButtonBackground,
    alertWindowButtonBackgroundOver,
    alertWindowButtonOutline,
    alertWindowButtonText,
    alertWindowButtonTextHighlighted,
    parametersBackground,
    parametersHeaderBackground,
    parametersHeaderText,
    parametersHeaderArrow,
    parametersParameterBackground,
    parametersParameterText,
    parametersSliderBackground,
    parametersSliderTrack,
    parametersColourBackground,
    parametersColourText,
    parametersBoolean,
    parametersBooleanTick,
    makerBackground,
    listBoxBackgroundAlternate,
    listBoxText,
    listBoxTextHighlighted
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static juce::Colour fetchColour (ColourIds i)
{
    const Palette& t = Palette::getInstance();
    
    switch (i) {
        case windowsBackground :                                    return t.backgroundMenu;
        case consoleTextDefault :                                   return t.textDefault;
        case consoleTextSystem :                                    return t.textSystem;
        case consoleTextWarning :                                   return t.textWarning;
        case consoleTextError :                                     return t.textError;
        case menubarBackground :                                    return t.backgroundMenu;
        case menubarText :                                          return t.textMenu;
        case menubarPopupBackground :                               return t.backgroundPopup;
        case menubarPopupBackgroundHighlighted :                    return t.textError;
        case menubarPopupText :                                     return t.textPopup;
        case menubarPopupTextHighlighted :                          return t.textMenu;
        case menubarPopupSeparator :                                return t.thumb;
        case toolbarBackground :                                    return t.backgroundMenu;
        case toolbarBackgroundDown :                                return t.backgroundPopup;
        case toolbarBackgroundOver :                                return t.backgroundMenu;
        case toolbarIconOn :                                        return t.textMenu;
        case toolbarIconOff :                                       return t.thumb;
        case toolbarZoom :                                          return t.textSystem;
        case devicesParameterBackground :                           return t.background;
        case devicesParameterText :                                 return t.textSystem;
        case devicesComboBoxBackground :                            return t.backgroundAlternate;
        case devicesComboBoxBackgroundActive :                      return t.backgroundBox;
        case devicesComboBoxArrow :                                 return t.textSystem;
        case tooltipBackground :                                    return t.backgroundMenu;
        case tooltipText :                                          return t.textMenu;
        case resizerHighlighted :                                   return t.backgroundPopup;
        case callOutBoxBackground :                                 return t.backgroundMenu;
        case callOutBoxOutline :                                    return t.textSystem;
        case alertWindowBackground :                                return t.backgroundMenu;
        case alertWindowOutline :                                   return t.textSystem;
        case alertWindowText :                                      return t.textMenu;
        case alertWindowIcon :                                      return t.textError;
        case alertWindowButtonBackground :                          return t.backgroundWidget;
        case alertWindowButtonBackgroundOver :                      return t.textError;
        case alertWindowButtonOutline :                             return t.textSystem;
        case alertWindowButtonText :                                return t.textSystem;
        case alertWindowButtonTextHighlighted :                     return t.textMenu;
        case parametersBackground :                                 return t.backgroundMenu;
        case parametersHeaderBackground :                           return t.background;
        case parametersHeaderText :                                 return t.textMenu;
        case parametersHeaderArrow :                                return t.textSystem;
        case parametersParameterBackground :                        return t.background;
        case parametersParameterText :                              return t.textSystem;
        case parametersSliderBackground :                           return t.backgroundAlternate;
        case parametersSliderTrack :                                return t.backgroundWidget;
        case parametersColourBackground :                           return t.backgroundAlternate;
        case parametersColourText :                                 return t.textSystem;
        case parametersBoolean :                                    return t.textSystem;
        case parametersBooleanTick :                                return t.textMenu;
        case makerBackground :                                      return t.background;
        case listBoxBackgroundAlternate :                           return t.backgroundAlternate;
        case listBoxText :                                          return t.textSystem;
        case listBoxTextHighlighted :                               return t.textHighlighted;
        default :                                                   return juce::Colours::black;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    const Palette& t = Palette::getInstance();
    
    /* Scroll bar. */
    
    lf->setColour (juce::ScrollBar::thumbColourId,                      t.thumb);
    lf->setColour (juce::ListBox::backgroundColourId,                   t.background);
    
    /* Text based parameters. */
    
    lf->setColour (juce::TextPropertyComponent::textColourId,           t.textSystem);
    lf->setColour (juce::TextPropertyComponent::outlineColourId,        t.transparent);
    lf->setColour (juce::TextPropertyComponent::backgroundColourId,     t.backgroundAlternate);
                   
    lf->setColour (juce::TextEditor::highlightColourId,                 t.transparent);
    lf->setColour (juce::TextEditor::highlightedTextColourId,           t.textHighlighted);
    
    lf->setColour (juce::CaretComponent::caretColourId,                 t.textMenu);
    lf->setColour (juce::Label::backgroundWhenEditingColourId,          t.transparent);
    lf->setColour (juce::Label::textWhenEditingColourId,                t.textSystem);
    lf->setColour (juce::Label::outlineWhenEditingColourId,             t.transparent);
    
    /* Boolean parameters. */
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     t.transparent);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  t.backgroundAlternate);
    
    /* Slider parameters. */
    
    lf->setColour (juce::Label::textColourId,                           t.textSystem);
    
    /* Colour selector sliders. */
    
    lf->setColour (juce::Slider::backgroundColourId,                    t.backgroundWidget);
    lf->setColour (juce::Slider::trackColourId,                         t.background);
    lf->setColour (juce::Slider::thumbColourId,                         t.thumb);
    
    /* Devices combo box. */
    
    lf->setColour (juce::ComboBox::textColourId,                        t.textSystem);
    
    /* Maker entry. */
    
    lf->setColour (juce::TextEditor::textColourId,                      t.textSystem);
    lf->setColour (juce::TextEditor::backgroundColourId,                t.transparent);
    lf->setColour (juce::TextEditor::outlineColourId,                   t.transparent);
    lf->setColour (juce::TextEditor::focusedOutlineColourId,            t.transparent);
    lf->setColour (juce::TextEditor::shadowColourId,                    t.transparent);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
