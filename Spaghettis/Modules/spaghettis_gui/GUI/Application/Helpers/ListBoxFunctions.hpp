
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

