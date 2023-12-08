
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MakerList::MakerList (MakerComponent& owner, const juce::Value& v) :
    owner_ (owner),
    items_ (Spaghettis()->getAutocomplete().getContent()),
    v_ (v)
{
    v_.addListener (this);
    
    listBox_.setModel (this);
    ListBoxFunctions::initialize (listBox_, false);
    ListBoxFunctions::update (listBox_, items_);
    addAndMakeVisible (listBox_);
        
    setOpaque (true);
}

MakerList::~MakerList()
{
    v_.removeListener (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int MakerList::getNumRows()
{
    return ListBoxFunctions::getNumberOfRowsToDraw (items_.size());
}

void MakerList::paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool isSelected)
{
    ListBoxFunctions::paintItem (items_, row, g, width, height, isSelected);
}

void MakerList::listWasScrolled()
{
    ListBoxFunctions::update (listBox_, items_, false);
}

void MakerList::listBoxItemClicked (int row, const juce::MouseEvent &)
{
    if (juce::isPositiveAndBelow (row, items_.size())) { v_.setValue (items_[row]); }
}

void MakerList::returnKeyPressed (int)
{
    owner_.enter();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerList::valueChanged (juce::Value& v)
{
    const juce::String s (Strings::upToWhitespace (v.toString()));
    
    if (!isEqualToSelectedItem (s)) {
    //
    if (s != previous_) {
        sort (s);
        ListBoxFunctions::selectFirstRowIfEqual (listBox_, items_, s);
        previous_ = s;
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerList::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
}
    
void MakerList::resized()
{
    listBox_.setBounds (getLocalBounds());
        
    ListBoxFunctions::update (listBox_, items_, false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool MakerList::isEqualToSelectedItem (const juce::String& s) const
{
    const int k = listBox_.getSelectedRow();
    
    if ((k >= 0) && (k < items_.size()) && (s == items_[k])) { return true; }
    
    return false;
}

void MakerList::sort (const juce::String& s)
{
    juce::StringArray scoped (Spaghettis()->getAutocomplete().getContent (s));
    
    items_.swapWith (scoped);
    
    ListBoxFunctions::update (listBox_, items_);
    ListBoxFunctions::scrollToStart (listBox_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
