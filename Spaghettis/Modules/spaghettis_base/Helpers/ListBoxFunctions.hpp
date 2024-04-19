
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> struct ListBoxConverter {

    static juce::Colour getColour (const T& e, bool isSelected)
    {
        return isSelected ? Colours::fetchColour (Colours::listBoxTextHighlighted)
                          : Colours::fetchColour (Colours::listBoxText);
    }

    static juce::String getText (const T& e)
    {
        return juce::String (e);
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct ListBoxFunctions {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void initialize (juce::ListBox& listBox, bool multiple)
    {
        const int h = static_cast<int> (Fonts::getFont().getHeight() * 1.5);
        
        listBox.setMultipleSelectionEnabled (multiple);
        listBox.setClickingTogglesRowSelection (true);
        listBox.setRowHeight (h);
        listBox.getViewport()->setScrollBarsShown (false, false, true, true);
    }

    static int getNumberOfRowsToDraw (int contentSize)
    {
        return juce::jmax (32, contentSize);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static juce::SparseSet<int> getSparseSetFor (int i)
    {
        juce::SparseSet<int> set; set.addRange ({i, i + 1}); return set;
    }
 
public:
    template <class T>
    static void selectFirstRowIfEqual (juce::ListBox& listBox, const T& items, const juce::String& s)
    {
        constexpr int i = 0;
        
        if (static_cast<int> (items.size()) > i) {
            if (items[i] == s) {
                listBox.setSelectedRows (getSparseSetFor (i), juce::dontSendNotification);
            }
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void scrollToStart (juce::ListBox& listBox)
    {
        listBox.scrollToEnsureRowIsOnscreen (0);
    }

    template <class T> static void scrollToEnd (juce::ListBox& listBox, const T& items)
    {
        const int i = static_cast<int> (items.size()) - 1;
        
        listBox.scrollToEnsureRowIsOnscreen (juce::jmax (i, 0));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void updateProceedBars (juce::ListBox& listBox, int size)
    {
        const bool show = (listBox.getNumRowsOnScreen() < size) ||
                          (size > 0 && listBox.getRowContainingPosition (0, 0) >= size);
        
        listBox.getViewport()->setScrollBarsShown (show, show, true, true);
    }

    static void updateProceedContent (juce::ListBox& listBox, bool deselect)
    {
        if (deselect) { listBox.deselectAllRows(); }
        
        listBox.updateContent();
        listBox.repaint();
    }
    
    static void updateProceed (juce::ListBox& listBox, int size, bool deselect)
    {
        updateProceedBars (listBox, size);      /* First in order to properly refresh the scroll bars. */
        
        updateProceedContent (listBox, deselect);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T> static void update (juce::ListBox& listBox, const T& items, bool deselect = true)
    {
        updateProceed (listBox, static_cast<int> (items.size()), deselect);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void paintItemProceed (const juce::String& text,
        const juce::Colour& c,
        juce::Graphics& g,
        int width,
        int height)
    {
        const juce::Rectangle<int> r (width, height);
        
        g.setColour (c);
        g.setFont (Fonts::getFont());
        g.drawText (text, r.reduced (4, 0), juce::Justification::centredLeft, true);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T> static void paintItem (const T& items,
        int row,
        juce::Graphics& g,
        int width,
        int height,
        bool isSelected)
    {
        if (row % 2) { g.fillAll (Colours::fetchColour (Colours::listBoxBackgroundAlternate)); }

        if (juce::isPositiveAndBelow (row, items.size())) {
        //
        auto e = items[row];
        
        const juce::Colour c = ListBoxConverter<decltype (e)>::getColour (e, isSelected);
        const juce::String s = ListBoxConverter<decltype (e)>::getText (e);
        
        paintItemProceed (s, c, g, width, height);
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

