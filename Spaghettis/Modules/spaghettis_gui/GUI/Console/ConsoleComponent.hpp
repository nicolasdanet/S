
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ConsoleComponent :    protected ConsoleFactoryHelper,     /* MUST be the first. */
                            public    BaseComponent,
                            public    juce::ListBoxModel,
                            public    spaghettis::Logger,
                            private   juce::AsyncUpdater {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ConsoleComponent (const juce::String& keyName);
    
    ~ConsoleComponent() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void update();
    void handleAsyncUpdate() override;
    void logMessage (MessagesPacket& m) override;
    void clear();
    void parse();
    void restore();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void logMessageProceed (MessagesPacket&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getNumRows() override;
    void paintListBoxItem (int, juce::Graphics&, int, int, bool) override;
    void listBoxItemClicked (int, const juce::MouseEvent&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;
    void listWasScrolled() override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void removeMessagesIfRequired (std::deque<Logger::MessagesElement>& messages)
    {
        const int maximum_ = 2048;
        const int removed_ = 64;
        
        int size = static_cast<int> (messages.size());
        
        if (size >= maximum_) {
        //
        const int n = juce::nextPowerOfTwo (size - maximum_ + removed_);
        
        jassert (n < size);
        
        messages.erase (messages.cbegin(), messages.cbegin() + n);
        //
        }
    }
    
    template <class T> static void parseMessages (T& m, bool showMessages, bool showErrors)
    {
        if (showMessages == false || showErrors == false) {
        //
        auto f = [showMessages, showErrors] (const Logger::MessagesElement& e)
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
    std::deque<Logger::MessagesElement> messages_;
    std::deque<Logger::MessagesElement> history_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

