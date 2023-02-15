
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

public:
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T>
    static void selectFirstRowIfEqual (juce::ListBox& listBox, const T& items, const juce::String& s)
    {
        constexpr int i = 0;
        
        if (static_cast<int> (items.size()) > i) {
            if (items[i] == s) {
                listBox.setSelectedRows (Helpers::getSparseSetFor (i), juce::dontSendNotification);
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
        g.setFont (Spaghettis()->getLookAndFeel().getListBoxFont());
        g.drawText (text, r.reduced (4, 0), juce::Justification::centredLeft, true);
    }

    static juce::Colour colourWithType (Logger::Type type)
    {
        int c = Colours::consoleTextError;
            
        if (type == Logger::Type::normal)       { c = Colours::consoleTextDefault; }
        else if (type == Logger::Type::system)  { c = Colours::consoleTextSystem;  }
        else if (type == Logger::Type::warning) { c = Colours::consoleTextWarning; }
            
        return Spaghettis()->getColour (c);
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
        if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::listBoxBackgroundAlternate)); }

        if (juce::isPositiveAndBelow (row, items.size())) {
        //
        auto e = items[row];
        
        if constexpr (std::is_same_v<decltype (e), Logger::MessagesElement>) {
            
            const juce::String t = Logger::getText (e);
            const juce::Colour c = isSelected   ? Spaghettis()->getColour (Colours::listBoxTextHighlighted)
                                                : colourWithType (Logger::getType (e));
                                                
            paintItemProceed (t, c, g, width, height);
            
        } else {
        
            const juce::String t = e;
            const juce::Colour c = isSelected   ? Spaghettis()->getColour (Colours::listBoxTextHighlighted)
                                                : Spaghettis()->getColour (Colours::listBoxText);
                                                
            paintItemProceed (t, c, g, width, height);
        }
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

