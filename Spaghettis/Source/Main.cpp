
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <JuceHeader.h>

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
        juce::LookAndFeel::setDefaultLookAndFeel (spaghettis::Spaghettis()->getLookAndFeel());
        
        console_.reset (new spaghettis::Console (getApplicationName()));
        
        spaghettis::Spaghettis()->start (getCommandLineParameterArray());
        
        runningFromCommandLine_ = spaghettis::Spaghettis()->isRunningFromCommandLine();
    }

    void shutdown() override
    {
        spaghettis::Spaghettis()->shutdown();
        
        console_ = nullptr;
        
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
            juce::File f (a.resolveAsFile());
            SPAGHETTIS_DEBUG ("?");
            spaghettis::Spaghettis()->handle (spaghettis::Inputs::openFile (f));
        }
        //
        }
    }

private:
    spaghettis::SpaghettisOwner spaghettis_;
    std::unique_ptr<spaghettis::Console> console_;
    bool runningFromCommandLine_;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

START_JUCE_APPLICATION (SpaghettisApplication)

