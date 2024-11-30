
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    windowsText,
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
    buttonBackground,
    buttonBackgroundOver,
    buttonText,
    buttonTextHighlighted,
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
    parametersBooleanText,
    parametersBooleanTick,
    parametersRadioOn,
    parametersRadioOff,
    parametersInfoBackground,
    parametersInfoText,
    parametersInfoTagBackground,
    parametersInfoTagText,
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
    const Palette* p = Palette::getInstance();
    
    switch (i) {
        case windowsBackground :                                    return p->backgroundMenu;
        case windowsText :                                          return p->backgroundPopup;
        case consoleTextDefault :                                   return p->textDefault;
        case consoleTextSystem :                                    return p->textSystem;
        case consoleTextWarning :                                   return p->textWarning;
        case consoleTextError :                                     return p->textError;
        case menubarBackground :                                    return p->backgroundMenu;
        case menubarText :                                          return p->textMenu;
        case menubarPopupBackground :                               return p->backgroundPopup;
        case menubarPopupBackgroundHighlighted :                    return p->textError;
        case menubarPopupText :                                     return p->textPopup;
        case menubarPopupTextHighlighted :                          return p->textMenu;
        case menubarPopupSeparator :                                return p->thumb;
        case toolbarBackground :                                    return p->backgroundMenu;
        case toolbarBackgroundDown :                                return p->backgroundPopup;
        case toolbarBackgroundOver :                                return p->backgroundMenu;
        case toolbarIconOn :                                        return p->textMenu;
        case toolbarIconOff :                                       return p->thumb;
        case toolbarZoom :                                          return p->textSystem;
        case tooltipBackground :                                    return p->backgroundMenu;
        case tooltipText :                                          return p->textMenu;
        case resizerHighlighted :                                   return p->backgroundPopup;
        case callOutBoxBackground :                                 return p->backgroundMenu;
        case callOutBoxOutline :                                    return p->textSystem;
        case alertWindowBackground :                                return p->backgroundMenu;
        case alertWindowOutline :                                   return p->textSystem;
        case alertWindowText :                                      return p->textMenu;
        case alertWindowIcon :                                      return p->textError;
        case alertWindowButtonBackground :                          return p->backgroundWidget;
        case alertWindowButtonBackgroundOver :                      return p->textError;
        case alertWindowButtonOutline :                             return p->textSystem;
        case alertWindowButtonText :                                return p->textSystem;
        case alertWindowButtonTextHighlighted :                     return p->textMenu;
        case buttonBackground :                                     return p->backgroundWidget;
        case buttonBackgroundOver :                                 return p->background;
        case buttonText :                                           return p->textSystem;
        case buttonTextHighlighted :                                return p->textMenu;
        case parametersBackground :                                 return p->backgroundMenu;
        case parametersHeaderBackground :                           return p->background;
        case parametersHeaderText :                                 return p->textMenu;
        case parametersHeaderArrow :                                return p->textSystem;
        case parametersParameterBackground :                        return p->background;
        case parametersParameterText :                              return p->textSystem;
        case parametersSliderBackground :                           return p->backgroundAlternate;
        case parametersSliderTrack :                                return p->backgroundWidget;
        case parametersColourBackground :                           return p->backgroundAlternate;
        case parametersColourText :                                 return p->textSystem;
        case parametersBoolean :                                    return p->textSystem;
        case parametersBooleanText :                                return p->textSystem;
        case parametersBooleanTick :                                return p->textMenu;
        case parametersRadioOn :                                    return p->textMenu;
        case parametersRadioOff :                                   return p->textSystem;
        case parametersInfoBackground :                             return p->backgroundAlternate;
        case parametersInfoText :                                   return p->textInfo;
        case parametersInfoTagBackground :                          return p->background;
        case parametersInfoTagText :                                return p->textPopup;
        case makerBackground :                                      return p->background;
        case listBoxBackgroundAlternate :                           return p->backgroundAlternate;
        case listBoxText :                                          return p->textSystem;
        case listBoxTextHighlighted :                               return p->textHighlighted;
        default :                                                   return juce::Colours::black;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    const Palette* p = Palette::getInstance();
    
    /* Scroll bar. */
    
    lf->setColour (juce::ScrollBar::thumbColourId,                      p->thumb);
    lf->setColour (juce::ListBox::backgroundColourId,                   p->background);
    
    /* Text based parameters. */
    
    lf->setColour (juce::TextPropertyComponent::textColourId,           p->textSystem);
    lf->setColour (juce::TextPropertyComponent::outlineColourId,        p->transparent);
    lf->setColour (juce::TextPropertyComponent::backgroundColourId,     p->backgroundAlternate);
                   
    lf->setColour (juce::TextEditor::highlightColourId,                 p->transparent);
    lf->setColour (juce::TextEditor::highlightedTextColourId,           p->textHighlighted);
    
    lf->setColour (juce::CaretComponent::caretColourId,                 p->textMenu);
    lf->setColour (juce::Label::backgroundColourId,                     p->backgroundAlternate);
    lf->setColour (juce::Label::backgroundWhenEditingColourId,          p->transparent);
    lf->setColour (juce::Label::textWhenEditingColourId,                p->textSystem);
    lf->setColour (juce::Label::outlineWhenEditingColourId,             p->transparent);
    
    /* Boolean parameters. */
    
    lf->setColour (juce::BooleanPropertyComponent::outlineColourId,     p->transparent);
    lf->setColour (juce::BooleanPropertyComponent::backgroundColourId,  p->backgroundAlternate);
    
    /* Slider parameters. */
    
    lf->setColour (juce::Label::textColourId,                           p->textSystem);
    
    /* Code paramaters. */
    
    lf->setColour (0x1004500, /* CodeEditorComponent::backgroundColourId */         p->backgroundAlternate);
    lf->setColour (0x1004502, /* CodeEditorComponent::highlightColourId */          p->textHighlighted);
    lf->setColour (0x1004503, /* CodeEditorComponent::defaultTextColourId */        p->textSystem);
    lf->setColour (0x1004504, /* CodeEditorComponent::lineNumberBackgroundId */     p->background);
    lf->setColour (0x1004505, /* CodeEditorComponent::lineNumberTextId */           p->textPopup);
        
    /* Colour selector sliders. */
    
    lf->setColour (juce::Slider::backgroundColourId,                    p->backgroundWidget);
    lf->setColour (juce::Slider::trackColourId,                         p->background);
    lf->setColour (juce::Slider::thumbColourId,                         p->thumb);
    
    /* Maker entry. */
    
    lf->setColour (juce::TextEditor::textColourId,                      p->textSystem);
    lf->setColour (juce::TextEditor::backgroundColourId,                p->transparent);
    lf->setColour (juce::TextEditor::outlineColourId,                   p->transparent);
    lf->setColour (juce::TextEditor::focusedOutlineColourId,            p->transparent);
    lf->setColour (juce::TextEditor::shadowColourId,                    p->transparent);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
