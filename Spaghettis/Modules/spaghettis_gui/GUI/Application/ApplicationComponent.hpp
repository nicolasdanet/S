
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ApplicationComponent :    public juce::Component,
                                public juce::ApplicationCommandTarget {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ApplicationComponent()
    {
        Spaghettis()->getCommandManager().registerAllCommandsForTarget (this);
        
        addKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
        
        setSize (500, 300);
    }
    
    ~ApplicationComponent() override
    {
        removeKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::ApplicationCommandTarget* getNextCommandTarget() override
    {
        return findFirstTargetParentComponent();
    }

    void getAllCommands (juce::Array<juce::CommandID>& c) override
    {
        Commands::getAllCommands (c);
    }

    void getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        Commands::getCommandInfo (c, r);
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        return Commands::perform (info);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

