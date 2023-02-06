
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MakerList::MakerList() : items_ (Spaghettis()->getAutocomplete().getContent())
{
    listBox_.setModel (this);
    ListBoxFunctions::initialize (listBox_, false);
    ListBoxFunctions::update (listBox_, items_, false);
    addAndMakeVisible (listBox_);
        
    setOpaque (true);
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
    /*
    if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::makerBackgroundAlternate)); }

    if (juce::isPositiveAndBelow (row, items_.size())) {
    //
    const juce::Rectangle<int> r (width, height);
    
    g.setColour (isSelected ? Spaghettis()->getColour (Colours::makerTextHighlighted)
                            : Spaghettis()->getColour (Colours::makerText));
                                
    g.setFont (Spaghettis()->getLookAndFeel().getMakerEntryFont());
    g.drawText (items_[row], r.reduced (4, 0), juce::Justification::centredLeft, true);
    //
    }
    */
    
    ListBoxFunctions::paintItem (items_, row, g, width, height, isSelected);
}

void MakerList::listWasScrolled()
{
    ListBoxFunctions::update (listBox_, items_, false);
}

void MakerList::listBoxItemClicked (int row, const juce::MouseEvent &)
{
    if (juce::isPositiveAndBelow (row, items_.size())) { }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerList::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::makerBackground));
}
    
void MakerList::resized()
{
    listBox_.setBounds (getLocalBounds());
        
    ListBoxFunctions::update (listBox_, items_, false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
