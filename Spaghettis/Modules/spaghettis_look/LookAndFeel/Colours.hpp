
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SPAGHETTIS_COLOUR(c)    (c | 0x7f000000)

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
    windowsBackground                               = SPAGHETTIS_COLOUR (0),
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
    devicesComboBoxArrow, //
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
    makerText,
    listBoxBackground,
    listBoxBackgroundAlternate,
    listBoxText,
    listBoxTextHighlighted
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static juce::Colour fetchColour (ColourIds i)
{
    return juce::LookAndFeel::getDefaultLookAndFeel().findColour (i);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::LookAndFeel* lf)
{
    const Palette& t = Palette::getInstance();
    
    lf->setColour (windowsBackground,                                   t.backgroundMenu);
    lf->setColour (consoleTextDefault,                                  t.textDefault);
    lf->setColour (consoleTextSystem,                                   t.textSystem);
    lf->setColour (consoleTextWarning,                                  t.textWarning);
    lf->setColour (consoleTextError,                                    t.textError);
    lf->setColour (menubarBackground,                                   t.backgroundMenu);
    lf->setColour (menubarText,                                         t.textMenu);
    lf->setColour (menubarPopupBackground,                              t.backgroundPopup);
    lf->setColour (menubarPopupBackgroundHighlighted,                   t.textError);
    lf->setColour (menubarPopupText,                                    t.textPopup);
    lf->setColour (menubarPopupTextHighlighted,                         t.textMenu);
    lf->setColour (menubarPopupSeparator,                               t.thumb);
    lf->setColour (toolbarBackground,                                   t.backgroundMenu);
    lf->setColour (toolbarBackgroundDown,                               t.backgroundPopup);
    lf->setColour (toolbarBackgroundOver,                               t.backgroundMenu);
    lf->setColour (toolbarIconOn,                                       t.textMenu);
    lf->setColour (toolbarIconOff,                                      t.thumb);
    lf->setColour (toolbarZoom,                                         t.textSystem);
    lf->setColour (devicesParameterBackground,                          t.background);
    lf->setColour (devicesParameterText,                                t.textSystem);
    lf->setColour (devicesComboBoxBackground,                           t.backgroundAlternate);
    lf->setColour (devicesComboBoxBackgroundActive,                     t.backgroundBox);
    lf->setColour (devicesComboBoxArrow,                                t.textSystem);
    lf->setColour (tooltipBackground,                                   t.backgroundMenu);
    lf->setColour (tooltipText,                                         t.textMenu);
    lf->setColour (resizerHighlighted,                                  t.backgroundPopup);
    lf->setColour (callOutBoxBackground,                                t.backgroundMenu);
    lf->setColour (callOutBoxOutline,                                   t.textSystem);
    lf->setColour (alertWindowBackground,                               t.backgroundMenu);
    lf->setColour (alertWindowOutline,                                  t.textSystem);
    lf->setColour (alertWindowText,                                     t.textMenu);
    lf->setColour (alertWindowIcon,                                     t.textError);
    lf->setColour (alertWindowButtonBackground,                         t.backgroundWidget);
    lf->setColour (alertWindowButtonBackgroundOver,                     t.textError);
    lf->setColour (alertWindowButtonOutline,                            t.textSystem);
    lf->setColour (alertWindowButtonText,                               t.textSystem);
    lf->setColour (alertWindowButtonTextHighlighted,                    t.textMenu);
    lf->setColour (parametersBackground,                                t.backgroundMenu);
    lf->setColour (parametersHeaderBackground,                          t.background);
    lf->setColour (parametersHeaderText,                                t.textMenu);
    lf->setColour (parametersHeaderArrow,                               t.textSystem);
    lf->setColour (parametersParameterBackground,                       t.background);
    lf->setColour (parametersParameterText,                             t.textSystem);
    lf->setColour (parametersSliderBackground,                          t.backgroundAlternate);
    lf->setColour (parametersSliderTrack,                               t.backgroundWidget);
    lf->setColour (parametersColourBackground,                          t.backgroundAlternate);
    lf->setColour (parametersColourText,                                t.textSystem);
    lf->setColour (parametersBoolean,                                   t.textSystem);
    lf->setColour (parametersBooleanTick,                               t.textMenu);
    lf->setColour (makerBackground,                                     t.background);
    lf->setColour (makerText,                                           t.textSystem);
    lf->setColour (listBoxBackground,                                   t.background);
    lf->setColour (listBoxBackgroundAlternate,                          t.backgroundAlternate);
    lf->setColour (listBoxText,                                         t.textSystem);
    lf->setColour (listBoxTextHighlighted,                              t.textHighlighted);
    
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
