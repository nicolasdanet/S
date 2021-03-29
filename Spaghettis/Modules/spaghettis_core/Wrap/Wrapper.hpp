
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Wrapper : private juce::AsyncUpdater, private juce::Thread {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Wrapper() : juce::Thread (juce::String ("Core"))
    {
    
    }
    
    ~Wrapper()
    {
        jassert (isThreadRunning() == false);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void run() override
    {
        bool error = false;
        
        try {
            error = core::main_threadLoop (this);
        }
        catch (...) {
            error = true;
        }
    
        if (error) {
            post (NEEDS_TRANS (juce::String ("spaghettis: core error")), Logger::Type::error);
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void start (const juce::StringArray& commandLine)
    {
        commandLine_ = commandLine;
        
        startThread();
    }
    
    void shutdown()
    {
        core::main_threadExit();
        
        bool good = stopThread (1000);
        
        jassert (good); (void)good;
        
        cancelPendingUpdate();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setLogger (Logger* logger)
    {
        logger_ = logger;
    }

    juce::StringArray getPendedFiles() const
    {
        return commandLine_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void post (const juce::String& m, Logger::Type type = Logger::Type::normal)
    {
        {
            const juce::ScopedLock lock (lock_); messages_.add (m);
        }
        
        triggerAsyncUpdate();
    }
    
private:
    void handleAsyncUpdate() override
    {
        juce::StringArray scoped;
        
        {
            const juce::ScopedLock lock (lock_); scoped.swapWith (messages_);
        }
        
        if (logger_) {
        //
        for (const auto& s : scoped) { logger_->logMessage (s, Logger::Type::normal); }
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Logger *logger_;
    juce::StringArray messages_;
    juce::CriticalSection lock_;
    juce::StringArray commandLine_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wrapper)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
