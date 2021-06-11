
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
                            public    spaghettis::Logger {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using MessagesElement   = std::pair<juce::String, Logger::Type>;
using MessagesContainer = std::vector<MessagesElement>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ConsoleComponent() :    ConsoleFactoryHelper (this),
                            ApplicationComponent (getIconsFactory())
    {
        messages_.reserve (maximum_);
        
        listBox_.setModel (this);
        ApplicationComponent::listBoxInitialize (listBox_);
        update (false);
        addAndMakeVisible (listBox_);
 
        Spaghettis()->setLogger (this);
        
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
    int getNumRows() override
    {
        return ApplicationComponent::lisBoxGetNumberOfRowsToDraw (static_cast<int> (messages_.size()));
    }

    void paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool isSelected) override
    {
        if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::consoleBackgroundAlternate)); }
        else {
            // g.fillAll (Spaghettis()->getColour (Colours::consoleBackground));
        }
        
        /*
        if (juce::isPositiveAndBelow (row, messages_.size())) {
        //
        const juce::Rectangle<int> r (width, height);
        
        g.setColour (isSelected ? Spaghettis()->getColour (Colours::searchpathsTextHighlighted)
                                : Spaghettis()->getColour (Colours::searchpathsText));
                                    
        g.setFont (Spaghettis()->getLookAndFeel().getFontConsole());
        g.drawText (paths_[row], r.reduced (4, 0), juce::Justification::centredLeft, true);
        //
        }
        */
    }
    
    void listBoxItemClicked (int row, const juce::MouseEvent &) override
    {
        if (juce::isPositiveAndBelow (row, messages_.size()) == false) { update(); }
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
        listBox_.setBounds (getBoundsRemaining()); update (false);
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

public:
    void logMessage (const juce::String& m, Type type) override
    {
        messages_.emplace_back (m, type);
    }
    
    void clear()
    {
        messages_.clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void update (bool updateRows = true)
    {
        if (updateRows) {
            ApplicationComponent::listBoxUpdateRows (listBox_);
        }
        
        ApplicationComponent::listBoxShowScrollBarIfRequired (listBox_, static_cast<int> (messages_.size()));
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
    static const int maximum_ = 2048;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

