
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ConsoleComponent::ConsoleComponent (juce::ApplicationCommandManager& command,
    juce::PropertiesFile* propertiesFile,
    const juce::String& keyName) :
        ConsoleFactoryHelper (this),
        BaseComponent (getIconsFactory(), Spaghettis()->getMenu(), command, propertiesFile, keyName)
{
    CommandsHandler::addCloseWindowCommand (this);
    
    listBox_.setModel (this);
    ListBoxFunctions::initialize (listBox_, false);
    ListBoxFunctions::update (listBox_, messages_);
    addAndMakeVisible (listBox_);
 
    Spaghettis()->setLogger (this);
        
    loadToolbarButtonsStates();

    setOpaque (true); setSize (600, 300);
}
    
ConsoleComponent::~ConsoleComponent()
{
    Spaghettis()->setLogger (nullptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ConsoleComponent::locate()
{
    const int n = listBox_.getLastRowSelected();
    
    if (juce::isPositiveAndBelow (n, messages_.size())) {
    //
    auto f = [u = messages_[n].getUniquePath()]()
    {
        if (Spaghettis()->getPatches().locate (u) == false) { Spaghettis()->resetConsole(); }
    };
    
    /* Delay call to front for cosmetic purposes only. */
    
    juce::Timer::callAfterDelay (250, f);
    //
    }
}

void ConsoleComponent::reset()
{
    ListBoxFunctions::update (listBox_, messages_, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void removeMessagesIfRequired (std::deque<LoggerMessage>& messages)
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

template <class T> void parseMessages (T& m, bool showMessages, bool showErrors)
{
    if (showMessages == false || showErrors == false) {
    //
    auto f = [showMessages, showErrors] (const LoggerMessage& e)
    {
        LoggerType t = e.getType();
        
        if ((t == LoggerType::normal || t == LoggerType::system) && showMessages == false) {
            return true;
        } else if ((t == LoggerType::warning || t == LoggerType::error) && showErrors == false) {
            return true;
        } else {
            return false;
        }
    };
    
    m.erase (std::remove_if (m.begin(), m.end(), f), m.end());
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ConsoleComponent::update (bool deselect)
{
    ListBoxFunctions::update (listBox_, messages_, deselect);
    
    if (getButtonState (Icons::autoscroll)) {
    //
    ListBoxFunctions::scrollToEnd (listBox_, messages_);
    //
    }
}

void ConsoleComponent::handleAsyncUpdate()
{
    update (false);
}

void ConsoleComponent::logMessage (std::vector<LoggerMessage>& m)
{
    removeMessagesIfRequired (messages_);
    removeMessagesIfRequired (history_);
    
    history_.insert (history_.cend(), m.cbegin(), m.cend());
    
    logMessageProceed (m); triggerAsyncUpdate();
}

void ConsoleComponent::clear()
{
    messages_.clear(); update (true);
}

void ConsoleComponent::parse()
{
    parseMessages (messages_, getButtonState (Icons::message), getButtonState (Icons::error));
    
    update (true);
}

void ConsoleComponent::restore()
{
    std::vector<LoggerMessage> m (history_.cbegin(), history_.cend());
    
    messages_.clear(); logMessageProceed (m); update (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ConsoleComponent::logMessageProceed (std::vector<LoggerMessage>& m)
{
    parseMessages (m, getButtonState (Icons::message), getButtonState (Icons::error));
    
    messages_.insert (messages_.cend(), m.cbegin(), m.cend());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int ConsoleComponent::getNumRows()
{
    return ListBoxFunctions::getNumberOfRowsToDraw (static_cast<int> (messages_.size()));
}

void ConsoleComponent::paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool isSelected)
{
    ListBoxFunctions::paintItem (messages_, row, g, width, height, isSelected);
}

void ConsoleComponent::listBoxItemClicked (int row, const juce::MouseEvent& e)
{
    if (!juce::isPositiveAndBelow (row, messages_.size())) { update (true); }
    else if (Mouse::isDoubleClick (e)) {
        locate();
    }
}

void ConsoleComponent::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::windowsBackground));
}

void ConsoleComponent::resized()
{
    listBox_.setBounds (setBoundsForBarsAndGetRemaining());
    
    ListBoxFunctions::update (listBox_, messages_, false);
}

void ConsoleComponent::listWasScrolled()
{
    ListBoxFunctions::update (listBox_, messages_, false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ConsoleComponent::tryGrabFocus()
{
    return tryGrabFocusForComponent (&listBox_);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
