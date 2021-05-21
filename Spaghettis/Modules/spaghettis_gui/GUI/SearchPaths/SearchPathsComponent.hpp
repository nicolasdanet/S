
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SearchPathsComponent :    public ApplicationComponent,
                                public juce::ListBoxModel {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SearchPathsComponent()
    {
        listBox_.setModel (this);
        listBox_.setMultipleSelectionEnabled (true);
        listBox_.setClickingTogglesRowSelection (true);
        
        // listBox_.setRowHeight ();
        // Spaghettis()->getConsoleFont();
        
        listBox_.getViewport()->setScrollBarsShown (false, false, true, true);
        
        addAndMakeVisible (listBox_);
        
        setSize (500, 300);
    }
    
    ~SearchPathsComponent() override
    {
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getNumRows() override
    {
        return rows_;
    }

    void paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::searchpathsBackgroundAlternate)); }

        /*
        if (rowIsSelected)
                g.fillAll (Colours::lightblue);
        if (juce::isPositiveAndBelow (row, oscLogList.size()))
        {
            g.setColour (juce::Colours::white);

            g.drawText (oscLogList[row],
                        juce::Rectangle<int> (width, height).reduced (4, 0),
                        juce::Justification::centredLeft, true);
        }
        */
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Spaghettis()->getColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void resized() override
    {
        listBox_.setBounds (getBoundsMenubarResized());
    }

private:
    juce::ListBox listBox_;

private:
    static int const rows_ = 64;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchPathsComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

