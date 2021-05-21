
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent :    public spaghettis::ApplicationComponent,
                            public spaghettis::Logger {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Avoid mouse interaction. */

private:
    class TextLogger : public juce::TextEditor {
    
    public:
        TextLogger() = default;
        ~TextLogger() = default;
    
    public:
        void mouseDown (const juce::MouseEvent& e) override
        {
            setHighlightedRegion (juce::Range<int>());
        }
        
        void mouseUp            (const juce::MouseEvent& e) override { }
        void mouseDrag          (const juce::MouseEvent& e) override { }
        void mouseDoubleClick   (const juce::MouseEvent& e) override { mouseDown (e); }
    
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextLogger)
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent() : lines_ (0)
    {
        const auto background = juce::TextEditor::backgroundColourId;
        const auto highlight  = juce::TextEditor::highlightColourId;
        
        text_.setMultiLine (true);
        text_.setReturnKeyStartsNewLine (true);
        text_.setReadOnly (true);
        text_.setScrollbarsShown (true);
        text_.setCaretVisible (false);
        text_.setPopupMenuEnabled (false);
        text_.setFont (Spaghettis()->getConsoleFont());
        text_.setInputRestrictions (maximumLengthOfLine_);
        text_.setColour (background, Spaghettis()->getColour (Colours::consoleBackground));
        text_.setColour (highlight, Spaghettis()->getColour (Colours::consoleHighlight));
        
        addAndMakeVisible (text_);
        
        Spaghettis()->setLogger (this);
        
        setSize (500, 300);
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
        text_.setBounds (getBoundsMenubarResized());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::Colour colourWithType (Type type) const
    {
        if (type == Type::normal)       { return Spaghettis()->getColour (Colours::consoleTextDefault); }
        if (type == Type::system)       { return Spaghettis()->getColour (Colours::consoleTextSystem); }
        else if (type == Type::warning) { return Spaghettis()->getColour (Colours::consoleTextWarning); }
        else {
            return Spaghettis()->getColour (Colours::consoleTextError);
        }
    }
    
    void removeLinesAtStartIfRequired()
    {
        if (++lines_ % trimRate_ == 0) {
        //
        text_.setCaretPosition (text_.getTotalNumChars() / 2);
        text_.moveCaretToStartOfLine (false);
        text_.moveCaretToTop (true);
        text_.insertTextAtCaret (juce::String ("...") + juce::newLine);
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void logMessage (const juce::String& m, Type type) override
    {
        removeLinesAtStartIfRequired();
        
        text_.moveCaretToEnd();
        text_.setColour (juce::TextEditor::textColourId, colourWithType (type));
        text_.insertTextAtCaret (m + juce::newLine);
    }

private:
    TextLogger text_;
    unsigned int lines_;

private:
    static const int maximumLengthOfLine_   = 2048;
    static const int trimRate_              = 4096;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

