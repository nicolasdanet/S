
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent : public juce::Component, public spaghettis::Logger, private juce::AsyncUpdater {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent()
    {
        addAndMakeVisible (text_);
        
        text_.setMultiLine (true);
        text_.setReturnKeyStartsNewLine (true);
        text_.setReadOnly (true);
        text_.setScrollbarsShown (true);
        text_.setCaretVisible (false);
        text_.setPopupMenuEnabled (false);

        setSize (600, 400);
    }
    
    ~ConsoleComponent() override
    {
    
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void resized() override
    {
        text_.setBounds (getLocalBounds());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void logMessage (const juce::String& m) override
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
        
        for (const juce::String& s : scoped) { logMessageProceed (s); }
    }
    
    void logMessageProceed (const juce::String& m)
    {
        text_.moveCaretToEnd();
        text_.insertTextAtCaret (m + juce::newLine);
    }
    
private:
    juce::TextEditor text_;
    juce::StringArray messages_;
    juce::CriticalSection lock_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

