
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent :    public juce::Component,
                            public juce::ApplicationCommandTarget,
                            public spaghettis::Logger {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent() : lines_ (0)
    {
        const auto background = juce::TextEditor::backgroundColourId;
        const auto highlight  = juce::TextEditor::highlightColourId;
        
        #if defined ( JUCE_LINUX )
        
        menuBar_.reset (new juce::MenuBarComponent (Spaghettis()->getMenuBarModel()));
        
        addAndMakeVisible (menuBar_.get());
        
        #endif
        
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
        Spaghettis()->getCommandManager()->registerAllCommandsForTarget (this);
        Spaghettis()->getCommandManager()->setFirstCommandTarget (this);
        
        addKeyListener (Spaghettis()->getCommandManager()->getKeyMappings());
        
        setSize (600, 400);
    }
    
    ~ConsoleComponent() override
    {
        removeKeyListener (Spaghettis()->getCommandManager()->getKeyMappings());
        
        Spaghettis()->getCommandManager()->setFirstCommandTarget (nullptr);
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
        auto b = getLocalBounds();

        #if defined ( JUCE_LINUX )
        
        menuBar_->setBounds (b.removeFromTop (Spaghettis()->getDefaultMenuBarHeight()));
        
        #endif
        
        text_.setBounds (b);
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
        c.add (Commands::fileOpen);
    }

    void getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        if (c == static_cast<int> (Commands::fileOpen)) {
            r.setInfo (NEEDS_TRANS ("Open..."), NEEDS_TRANS ("Open a Patch"), NEEDS_TRANS ("File"), 0);
            r.addDefaultKeypress ('o', juce::ModifierKeys::commandModifier);
        }
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        if (info.commandID == static_cast<int> (Commands::fileOpen)) { DBG ("?"); return true; }

        return false;
    }

private:
    juce::TextEditor text_;
    unsigned int lines_;

#if defined ( JUCE_LINUX )

private:
    std::unique_ptr<juce::MenuBarComponent> menuBar_;

#endif

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

