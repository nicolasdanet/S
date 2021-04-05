
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
    ConsoleComponent() : lines_ (0)
    {
        const auto background = juce::TextEditor::backgroundColourId;
        
        text_.setMultiLine (true);
        text_.setReturnKeyStartsNewLine (true);
        text_.setReadOnly (true);
        text_.setScrollbarsShown (true);
        text_.setCaretVisible (false);
        text_.setPopupMenuEnabled (false);
        text_.setFont (Spaghettis()->getConsoleFont());
        text_.setInputRestrictions (maximumLengthOfLine_);
        text_.setColour (background, Spaghettis()->getColour (Colours::consoleBackground));
        
        addAndMakeVisible (text_);
        setSize (600, 400);

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

private:
    juce::Colour colourWithType (Type type) const
    {
        if (type == Type::normal)       { return Spaghettis()->getColour (Colours::consoleTextDefault); }
        else if (type == Type::warning) { return Spaghettis()->getColour (Colours::consoleTextWarning); }
        else {
            return Spaghettis()->getColour (Colours::consoleTextError);
        }
    }
    
    void removeLinesAtStartIfRequired()
    {
        lines_++;
        
        if (lines_ > maximumNumberOfLines_) {
        //
        const int numberOfLinesToRemove = (maximumNumberOfLines_ / 2) + 1;
        
        text_.moveCaretToTop (false);
        
        for (int i = 0; i < numberOfLinesToRemove; ++i) { text_.moveCaretDown (true); }
        
        text_.insertTextAtCaret (juce::String());
        
        lines_ -= numberOfLinesToRemove;
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void logMessage (const juce::String& m, Type type) override
    {
        text_.moveCaretToEnd();
        text_.setColour (juce::TextEditor::textColourId, colourWithType (type));
        text_.insertTextAtCaret (m + juce::newLine);
        
        removeLinesAtStartIfRequired();
    }
    
private:
    juce::TextEditor text_;
    int lines_;

private:
    static const int maximumLengthOfLine_   = 2048;
    static const int maximumNumberOfLines_  = 8;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

