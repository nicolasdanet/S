
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PreferencesComponent :    protected PreferencesFactoryHelper,
                                public ParametersView,
                                public BaseComponent<CommandsHandler> {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PreferencesComponent (juce::ApplicationCommandManager& command,
        juce::PropertiesFile* propertiesFile,
        const juce::String& keyName) :
            PreferencesFactoryHelper (this),
            ParametersView (Spaghettis()->getPreferences().getData(),
                PropertyLookAndFeel (Fonts::getFont(), Fonts::getMonospacedFont(), 250)),
            BaseComponent (getIconsFactory(), Spaghettis()->getMenu(), command, propertiesFile, keyName)
    {
        CommandsHandler::addCloseWindowCommand (this);
        
        addAndMakeVisible (&getConcertinaPanel());
        
        setOpaque (true); setSize (800, 400);
    }
    
    ~PreferencesComponent() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Colours::fetchColour (Colours::parametersBackground));
    }
    
    void resized() override
    {
        resizeConcertinaPanel (setBoundsForBarsAndGetRemaining());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (&getConcertinaPanel());
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

