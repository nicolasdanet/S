
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct ListBoxFunctions {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void initialize (juce::ListBox& listBox, bool multiple)
{
    const int h = static_cast<int> (Spaghettis()->getLookAndFeel().getConsoleFont().getHeight() * 1.5);
    
    listBox.setMultipleSelectionEnabled (multiple);
    listBox.setClickingTogglesRowSelection (true);
    listBox.setRowHeight (h);
    listBox.getViewport()->setScrollBarsShown (false, false, true, true);
}

static int getNumberOfRowsToDraw (int contentSize)
{
    return juce::jmax (32, contentSize);
}

static void updateProceed (juce::ListBox& listBox, int size, bool updateRows)
{
    if (updateRows) {
        listBox.updateContent();
        listBox.deselectAllRows();
        listBox.repaint();
    }
    
    const bool show = (listBox.getNumRowsOnScreen() < size) ||
                      (size > 0 && listBox.getRowContainingPosition (0, 0) >= size);
    
    listBox.getViewport()->setScrollBarsShown (show, show, true, true);
}

template <class T> static void update (juce::ListBox& listBox, T& c, bool updateRows)
{
    updateProceed (listBox, static_cast<int> (c.size()), updateRows);
}

template <class T> static void paintItem (const T& items,
    int row,
    juce::Graphics& g,
    int width,
    int height,
    bool isSelected)
{
    if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::listBoxBackgroundAlternate)); }

    if (juce::isPositiveAndBelow (row, items.size())) {
    //
    const juce::Rectangle<int> r (width, height);
    
    g.setColour (isSelected ? Spaghettis()->getColour (Colours::listBoxTextHighlighted)
                            : Spaghettis()->getColour (Colours::listBoxText));
                                
    g.setFont (Spaghettis()->getLookAndFeel().getListBoxFont());
    g.drawText (items[row], r.reduced (4, 0), juce::Justification::centredLeft, true);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

