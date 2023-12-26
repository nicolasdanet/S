
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
        juce::LookAndFeel::setDefaultLookAndFeel (&lnf_);
        
        spaghettis::Spaghettis()->getCommandManager().registerAllCommandsForTarget (this);
        spaghettis::Spaghettis()->start (getCommandLineParameterArray());

        runningFromCommandLine_ = spaghettis::Spaghettis()->isRunningFromCommandLine();
    }

    void shutdown() override
    {
        spaghettis::Spaghettis()->shutdown();
        
        juce::LookAndFeel::setDefaultLookAndFeel (nullptr);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T> void performOrWaitAllRequestsDone (T f) const
    {
        const spaghettis::ExitStatus status = spaghettis::Spaghettis()->getExitStatus();
        
        if (status == spaghettis::ExitStatus::cancel)    { return; }
        else if (status == spaghettis::ExitStatus::work) { f(); }
        else if (status == spaghettis::ExitStatus::wait) {
            const int primeInterval = 457;
            auto t = [this, f = f]() { performOrWaitAllRequestsDone<T> (f); };
            juce::Timer::callAfterDelay (primeInterval, t);
        }
    }
    
    void systemRequestedQuit() override
    {
        if (spaghettis::Spaghettis()->requestToQuit()) {
            performOrWaitAllRequestsDone ([]() { spaghettis::Spaghettis()->closeAllPatches(); });
            performOrWaitAllRequestsDone ([]() { quit(); });
        }
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void getAllCommands (juce::Array<juce::CommandID>& c) override
    {
        juce::JUCEApplication::getAllCommands (c); commands_.getCommands (c, true);
    }

    void getCommandInfo (juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        juce::JUCEApplication::getCommandInfo (c, r); commands_.getInfo (c, r);
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        if (commands_.perform (info) == false) { return juce::JUCEApplication::perform (info); }
        else {
            return true;
        }
    }

private:
    spaghettis::LNF lnf_;
    
private:
    spaghettis::CommandsHandler commands_;
    spaghettis::SpaghettisOwner spaghettis_;
    
private:
    bool runningFromCommandLine_;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

START_JUCE_APPLICATION (SpaghettisApplication)

