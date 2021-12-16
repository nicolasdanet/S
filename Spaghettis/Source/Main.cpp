
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <JuceHeader.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if ! ( JUCE_MAC )
#if ! ( JUCE_LINUX )
    #error "Unsupported platform!"
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SpaghettisApplication : public juce::JUCEApplication {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SpaghettisApplication() : runningFromCommandLine_ (false)
    {
        SPAGHETTIS_DEBUG ("Hello!");
    }
    
    ~SpaghettisApplication()
    {
        SPAGHETTIS_DEBUG ("Goodbye!");
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void initialise (const juce::String&) override
    {
        juce::LookAndFeel::setDefaultLookAndFeel (&spaghettis::Spaghettis()->getLookAndFeel());
        
        spaghettis::Spaghettis()->getCommandManager().registerAllCommandsForTarget (this);
        spaghettis::Spaghettis()->start (getCommandLineParameterArray());

        runningFromCommandLine_ = spaghettis::Spaghettis()->isRunningFromCommandLine();
    }

    void shutdown() override
    {
        spaghettis::Spaghettis()->shutdown();
        
        juce::LookAndFeel::setDefaultLookAndFeel (nullptr);
    }
    
    void systemRequestedQuit() override
    {
        quit();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const juce::String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }
    
    const juce::String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }
    
    bool moreThanOneInstanceAllowed() override
    {
        return false;
    }

    void anotherInstanceStarted (const juce::String& s) override
    {
        if (!runningFromCommandLine_) {     /* Avoid startup files to be launched twice on macOS. */
        //
        juce::ArgumentList cmd (getApplicationName(), s);

        for (const auto& a : cmd.arguments) {
            juce::File f (a.resolveAsFile()); spaghettis::Spaghettis()->openPatch (f);
        }
        //
        }
    }

public:
    void getAllCommands (juce::Array<juce::CommandID>& c) override
    {
        juce::JUCEApplication::getAllCommands (c); spaghettis::Commands::getApplicationCommands (c);
    }

    void getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        juce::JUCEApplication::getCommandInfo (c, r); spaghettis::Commands::getCommandInfo (c, r);
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        if (spaghettis::Commands::perform (info) == false) { return juce::JUCEApplication::perform (info); }
        else {
            return true;
        }
    }
    
private:
    spaghettis::SpaghettisOwner spaghettis_;

private:
    bool runningFromCommandLine_;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

START_JUCE_APPLICATION (SpaghettisApplication)

