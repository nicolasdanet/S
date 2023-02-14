
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ConsoleComponent::ConsoleComponent (const juce::String& keyName) :
    ConsoleFactoryHelper (this),
    BaseComponent (getIconsFactory(), keyName)
{
    listBox_.setModel (this);
    ListBoxFunctions::initialize (listBox_, false);
    ListBoxFunctions::updateScrollBars (listBox_, messages_);
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void removeMessagesIfRequired (std::deque<Logger::MessagesElement>& messages)
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
    auto f = [showMessages, showErrors] (const Logger::MessagesElement& e)
    {
        Logger::Type t = Logger::getType (e);
        
        if ((t == Logger::Type::normal || t == Logger::Type::system) && showMessages == false) {
            return true;
        } else if ((t == Logger::Type::warning || t == Logger::Type::error) && showErrors == false) {
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

void ConsoleComponent::update()
{
    ListBoxFunctions::update (listBox_, messages_);
    
    if (getButtonState (Icons::autoscroll)) {
    //
    ListBoxFunctions::scrollToEnd (listBox_, messages_);
    //
    }
}

void ConsoleComponent::handleAsyncUpdate()
{
    update();
}

void ConsoleComponent::logMessage (MessagesPacket& m)
{
    removeMessagesIfRequired (messages_);
    removeMessagesIfRequired (history_);
    
    history_.insert (history_.cend(), m.cbegin(), m.cend());
    
    logMessageProceed (m);
}

void ConsoleComponent::clear()
{
    messages_.clear(); triggerAsyncUpdate();
}

void ConsoleComponent::parse()
{
    parseMessages (messages_, getButtonState (Icons::message), getButtonState (Icons::error));
    
    triggerAsyncUpdate();
}

void ConsoleComponent::restore()
{
    MessagesPacket m (history_.cbegin(), history_.cend());
    
    messages_.clear(); logMessageProceed (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ConsoleComponent::logMessageProceed (MessagesPacket& m)
{
    parseMessages (m, getButtonState (Icons::message), getButtonState (Icons::error));
    
    messages_.insert (messages_.cend(), m.cbegin(), m.cend());
    
    triggerAsyncUpdate();
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

void ConsoleComponent::listBoxItemClicked (int row, const juce::MouseEvent &)
{
    if (juce::isPositiveAndBelow (row, messages_.size()) == false) { triggerAsyncUpdate(); }
}

void ConsoleComponent::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
}

void ConsoleComponent::resized()
{
    listBox_.setBounds (setBoundsForBarsAndGetRemaining());
    
    ListBoxFunctions::updateScrollBars (listBox_, messages_);
}

void ConsoleComponent::listWasScrolled()
{
    ListBoxFunctions::updateScrollBars (listBox_, messages_);
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
