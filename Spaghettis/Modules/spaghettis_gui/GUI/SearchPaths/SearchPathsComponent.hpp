
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
        const int h = static_cast<int> (Spaghettis()->getLookAndFeel().getFontConsole().getHeight() * 1.5);
        
        listBox_.setModel (this);
        listBox_.setMultipleSelectionEnabled (true);
        listBox_.setClickingTogglesRowSelection (true);
        listBox_.setRowHeight (h);
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

private:
    void updateSearchPaths()
    {
        listBox_.deselectAllRows();
        listBox_.repaint();
    }
    
    void appendPath (const juce::String& filepath)
    {
        paths_.addIfNotAlreadyThere (filepath); updateSearchPaths();
    }
    
    void chooseAndAppendFolder()
    {
        /*
        fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a folder to add..."),
                                    currentOpenDirectory_,
                                    spaghettis::core::getFileExtensions());
        
        int flags = juce::FileBrowserComponent::canSelectMultipleItems
                            | juce::FileBrowserComponent::openMode
                            | juce::FileBrowserComponent::canSelectFiles;
            
        auto callback = [] (const juce::FileChooser& fileChooser)
        {
            auto files = fileChooser.getResults(); for (const auto& f : files) { Spaghettis()->openPatch (f); }
        };
                        
        fileChooser_->launchAsync (flags, callback);
        */
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getNumRows() override
    {
        return rows_;
    }

    void paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool isSelected) override
    {
        if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::searchpathsBackgroundAlternate)); }

        if (juce::isPositiveAndBelow (row, paths_.size())) {
        //
        const juce::Rectangle<int> r (width, height);
        
        g.setColour (isSelected ? Spaghettis()->getColour (Colours::searchpathsTextHighlighted)
                                : Spaghettis()->getColour (Colours::searchpathsText));
                                    
        g.setFont (Spaghettis()->getLookAndFeel().getFontConsole());
        g.drawText (paths_[row], r.reduced (4, 0), juce::Justification::centredLeft, true);
        //
        }
    }
    
    void listBoxItemClicked (int row, const juce::MouseEvent &) override
    {
        if (juce::isPositiveAndBelow (row, paths_.size()) == false) {
            if (listBox_.getNumSelectedRows() > 1) { updateSearchPaths(); }
            else {
                chooseAndAppendFolder();
            }
        }
    }
    
    void deleteKeyPressed (int) override
    {
        for (int i = getNumRows() - 1; i >= 0; i--) {
        //
        if (listBox_.isRowSelected (i)) {
        if (i < paths_.size()) {
            paths_.remove (i);
        }
        }
        //
        }
        
        updateSearchPaths();
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
    juce::StringArray paths_;

private:
    std::unique_ptr<juce::FileChooser> fileChooser_;

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

