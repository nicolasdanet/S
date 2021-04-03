
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent : public juce::Component, public spaghettis::Logger {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent()
    {
        const int background = juce::TextEditor::backgroundColourId;
        
        addAndMakeVisible (text_);
        
        text_.setMultiLine (true);
        text_.setReturnKeyStartsNewLine (true);
        text_.setReadOnly (true);
        text_.setScrollbarsShown (true);
        text_.setCaretVisible (false);
        text_.setPopupMenuEnabled (false);
        text_.setFont (Spaghettis()->getConsoleFont());
        setSize (600, 400);
        
        text_.setColour (background, Spaghettis()->getColour (Colours::consoleBackground));
        
        Spaghettis()->setLogger (this);
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
        g.fillAll (Spaghettis()->getColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void resized() override
    {
        text_.setBounds (getLocalBounds());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void logMessage (const juce::String& m, Type) override
    {
        text_.moveCaretToEnd();
        text_.insertTextAtCaret (m + juce::newLine);
    }
    
private:
    juce::TextEditor text_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

