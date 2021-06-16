
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent :    protected ConsoleFactoryHelper,     /* MUST be the first. */
                            public    ApplicationComponent,
                            public    juce::ListBoxModel,
                            public    spaghettis::Logger,
                            private   juce::AsyncUpdater {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using MessagesContainer = std::deque<Logger::MessagesElement>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent (const juce::String& keyName) :    ConsoleFactoryHelper (this),
                                                        ApplicationComponent (keyName, getIconsFactory()),
                                                        showMessages_ (true),
                                                        showErrors_ (true)
    {
        listBox_.setModel (this);
        ApplicationComponent::listBoxInitialize (listBox_, false);
        ApplicationComponent::listBoxUpdate (listBox_, messages_, false);
        addAndMakeVisible (listBox_);
 
        Spaghettis()->setLogger (this);
        
        loadToolbarButtonsStates();
        
        setSize (600, 300);
    }
    
    ~ConsoleComponent() override
    {
        Spaghettis()->setLogger (nullptr);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void handleAsyncUpdate() override
    {
        ApplicationComponent::listBoxUpdate (listBox_, messages_, true);
        
        if (getIconToggleState (Icons::autoscroll)) {
        //
        listBox_.scrollToEnsureRowIsOnscreen (static_cast<int> (messages_.size()));
        //
        }
    }
    
    void logMessage (const MessagesPacket& m) override
    {
        removeMessagesIfRequired(); messages_.insert (messages_.end(), m.begin(), m.end());
        
        triggerAsyncUpdate();
    }
    
    void clear()
    {
        messages_.clear(); triggerAsyncUpdate();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getNumRows() override
    {
        return ApplicationComponent::lisBoxGetNumberOfRowsToDraw (static_cast<int> (messages_.size()));
    }

    void paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool isSelected) override
    {
        if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::consoleBackgroundAlternate)); }
        else {
            /* g.fillAll (Spaghettis()->getColour (Colours::consoleBackground)); */
        }
        
        if (juce::isPositiveAndBelow (row, messages_.size())) {
        //
        const juce::Rectangle<int> r (width, height);
        const Logger::MessagesElement& e = messages_[row];
        
        g.setColour (isSelected ? Spaghettis()->getColour (Colours::consoleTextHighlighted)
                                : colourWithType (std::get<1> (e)));
        
        g.setFont (Spaghettis()->getLookAndFeel().getFontConsole());
        g.drawText (std::get<0> (e), r.reduced (4, 0), juce::Justification::centredLeft, true);
        //
        }
    }
    
    void listBoxItemClicked (int row, const juce::MouseEvent &) override
    {
        if (juce::isPositiveAndBelow (row, messages_.size()) == false) { triggerAsyncUpdate(); }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
    }
    
    void resized() override
    {
        listBox_.setBounds (getBoundsRemaining());
        
        ApplicationComponent::listBoxUpdate (listBox_, messages_, false);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (&listBox_);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void removeMessagesIfRequired()
    {
        int size = static_cast<int> (messages_.size());
        
        if (size >= maximum_) {
        //
        const int n = juce::nextPowerOfTwo (size - maximum_ + removed_);
        
        jassert (n < size);
        
        messages_.erase (messages_.begin(), messages_.begin() + n);
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static juce::Colour colourWithType (Type type)
    {
        int c = Colours::consoleTextError;
        
        if (type == Type::normal)       { c = Colours::consoleTextDefault; }
        else if (type == Type::system)  { c = Colours::consoleTextSystem;  }
        else if (type == Type::warning) { c = Colours::consoleTextWarning; }
        
        return Spaghettis()->getColour (c);
    }

private:
    juce::ListBox listBox_;
    MessagesContainer messages_;

private:
    bool showMessages_;
    bool showErrors_;
    
private:
    static const int maximum_ = 2048;
    static const int removed_ = 64;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

