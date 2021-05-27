
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SearchPathsComponent :    public  ApplicationComponent,
                                public  juce::ListBoxModel,
                                private juce::AsyncUpdater  {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SearchPathsComponent() : paths_ (Spaghettis()->getSearchPaths())
    {
        const int h = static_cast<int> (Spaghettis()->getLookAndFeel().getFontConsole().getHeight() * 1.5);
        
        listBox_.setModel (this);
        listBox_.setMultipleSelectionEnabled (true);
        listBox_.setClickingTogglesRowSelection (true);
        listBox_.setRowHeight (h);
        listBox_.getViewport()->setScrollBarsShown (false, false, true, true);
        
        addAndMakeVisible (listBox_);
        
        setSize (400, 500);
    }
    
    ~SearchPathsComponent() override
    {
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void handleAsyncUpdate() override
    {
        Spaghettis()->setSearchPaths (paths_);
    }
    
private:
    void setSearchPaths()
    {
        triggerAsyncUpdate();
    }
    
    void updateView()
    {
        listBox_.deselectAllRows();
        listBox_.repaint();
    }
    
    void appendFullPathName (const juce::String& filepath)
    {
        paths_.addIfNotAlreadyThere (filepath); updateView(); setSearchPaths();
    }
    
    void appendFile (const juce::File& file)
    {
        if (file.isDirectory()) {
        //
        appendFullPathName (file.getFullPathName());
        
        Spaghettis()->setCurrentOpenDirectory (file.getParentDirectory());
        //
        }
    }
    
    void chooseAndAppendFolder()
    {
        fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a folder..."),
                            Spaghettis()->getCurrentOpenDirectory());
    
        const int t = juce::FileBrowserComponent::canSelectMultipleItems
                            | juce::FileBrowserComponent::openMode
                            | juce::FileBrowserComponent::canSelectDirectories;
        
        auto callback = [this] (const juce::FileChooser& fileChooser)
        {
            auto files = fileChooser.getResults(); for (const auto& f : files) { appendFile (f); }
        };
                        
        fileChooser_->launchAsync (t, callback);
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
            if (listBox_.getNumSelectedRows() > 1) { updateView(); }
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
        
        updateView(); setSearchPaths();
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (&listBox_);
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

