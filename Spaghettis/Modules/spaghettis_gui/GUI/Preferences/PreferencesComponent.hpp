
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PreferencesComponent :    public ParameterView,
                                public BaseComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PreferencesComponent (juce::PropertiesFile& propertiesFile, const juce::String& keyName) :
        ParameterView (Spaghettis()->getPreferences().getData(),
            ParameterBase (Fonts::getFont(), Fonts::getMonospacedFont(), 250)),
        BaseComponent (nullptr, propertiesFile, keyName)
    {
        addAndMakeVisible (&getPanel());
        
        requireExpandPanel();
        
        setOpaque (true); setSize (600, 400);
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
        resizePanel (setBoundsForBarsAndGetRemaining());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (&getPanel());
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

