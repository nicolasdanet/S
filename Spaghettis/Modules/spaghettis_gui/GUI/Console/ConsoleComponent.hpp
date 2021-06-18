
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
                                                        ApplicationComponent (keyName, getIconsFactory())
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
        
        if (getButtonState (Icons::autoscroll)) {
        //
        const int i = static_cast<int> (messages_.size()) - 1;
        
        listBox_.scrollToEnsureRowIsOnscreen (juce::jmax (i, 0));
        //
        }
    }
    
    void logMessage (MessagesPacket& m) override
    {
        removeMessagesIfRequired (messages_);
        
        parseMessages (m, getButtonState (Icons::message), getButtonState (Icons::error));

        messages_.insert (messages_.end(), m.begin(), m.end());
        
        triggerAsyncUpdate();
    }
    
    void clear()
    {
        messages_.clear(); triggerAsyncUpdate();
    }
    
    void parse()
    {
        parseMessages (messages_, getButtonState (Icons::message), getButtonState (Icons::error));
        
        triggerAsyncUpdate();
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
        const auto& e = messages_[row];
        
        g.setColour (isSelected ? Spaghettis()->getColour (Colours::consoleTextHighlighted)
                                : colourWithType (Logger::getType (e)));
        
        g.setFont (Spaghettis()->getLookAndFeel().getFontConsole());
        g.drawText (Logger::getText (e), r.reduced (4, 0), juce::Justification::centredLeft, true);
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
    static juce::Colour colourWithType (Type type)
    {
        int c = Colours::consoleTextError;
        
        if (type == Type::normal)       { c = Colours::consoleTextDefault; }
        else if (type == Type::system)  { c = Colours::consoleTextSystem;  }
        else if (type == Type::warning) { c = Colours::consoleTextWarning; }
        
        return Spaghettis()->getColour (c);
    }

    static void removeMessagesIfRequired (MessagesContainer& messages)
    {
        const int maximum_ = 2048;
        const int removed_ = 64;
        
        int size = static_cast<int> (messages.size());
        
        if (size >= maximum_) {
        //
        const int n = juce::nextPowerOfTwo (size - maximum_ + removed_);
        
        jassert (n < size);
        
        messages.erase (messages.begin(), messages.begin() + n);
        //
        }
    }
    
    template <class T>  static void parseMessages (T& m, bool showMessages, bool showErrors)
    {
        if (showMessages == false || showErrors == false) {
        //
        auto f = [showMessages, showErrors](const Logger::MessagesElement& e)
        {
            Type t = Logger::getType (e);
            
            if ((t == Type::normal || t == Type::system) && showMessages == false)    { return true; }
            else if ((t == Type::warning || t == Type::error) && showErrors == false) { return true; }
            else {
                return false;
            }
        };
        
        m.erase (std::remove_if (m.begin(), m.end(), f), m.end());
        //
        }
    }
    
private:
    juce::ListBox listBox_;
    MessagesContainer messages_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

