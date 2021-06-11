
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent :    protected ConsoleFactoryHelper,     /* MUST be the first. */
                            public    ApplicationComponent,
                            public    spaghettis::Logger {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent() :    ConsoleFactoryHelper (this),
                            ApplicationComponent (getIconsFactory())
    {
        // addAndMakeVisible (text_);
        
        Spaghettis()->setLogger (this);
        
        setSize (600, 300);
    }
    
    ~ConsoleComponent() override
    {
        Spaghettis()->setLogger (nullptr);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
    }
    
    void resized() override
    {
        getBoundsRemaining();
        // .setBounds (getBoundsRemaining());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void logMessage (const juce::String& m, Type type) override
    {

    }
    
    void clear()
    {
        
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static juce::Colour colourWithType (Type type)
    {
        int c = Colours::consoleTextError;
        
        if (type == Type::normal)       { c = Colours::consoleTextDefault; }
        else if (type == Type::system)  { c = Colours::consoleTextSystem;  }
        else if (type == Type::warning) { c = Colours::consoleTextWarning; }
        
        return Spaghettis()->getColour (c);
    }
    
private:
    juce::ListBox listBox_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

