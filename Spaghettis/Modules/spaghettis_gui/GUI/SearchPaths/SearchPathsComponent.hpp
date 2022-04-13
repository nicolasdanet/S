
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SearchPathsComponent :    protected SearchPathsFactoryHelper,     /* MUST be the first. */
                                public    BaseComponent,
                                public    juce::ListBoxModel,
                                private   juce::AsyncUpdater  {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit SearchPathsComponent (const juce::String& keyName) :
        SearchPathsFactoryHelper (this),
        BaseComponent (getIconsFactory(), keyName),
        paths_ (Spaghettis()->getSearchPaths())
    {
        listBox_.setModel (this);
        ListBoxFunctions::initialize (listBox_, true);
        ListBoxFunctions::update (listBox_, paths_, false);
        addAndMakeVisible (listBox_);
        
        setDefaultSize (this, 400, 500);
    }
    
    ~SearchPathsComponent() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void removeSelectedPaths()
    {
        bool done = false;
        
        for (int i = getNumRows() - 1; i >= 0; --i) {
        //
        if (listBox_.isRowSelected (i)) {
        if (i < paths_.size()) {
            paths_.remove (i); done = true;
        }
        }
        //
        }
        
        if (done) { ListBoxFunctions::update (listBox_, paths_, true); setSearchPaths(); }
    }
    
    void addPaths()
    {
        chooseAndAppendFolder();
    }
    
    void sortPaths (bool reverse = false)
    {
        if (paths_.size() > 1) {
        //
        paths_.sortNatural();
        
        if (reverse) {
        //
        juce::StringArray scoped; scoped.ensureStorageAllocated (paths_.size());
        
        for (const auto& s : paths_) { scoped.insert (0, s); }
        
        scoped.swapWith (paths_);
        //
        }
        
        ListBoxFunctions::update (listBox_, paths_, true); setSearchPaths();
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getNumRows() override
    {
        return ListBoxFunctions::getNumberOfRowsToDraw (paths_.size());
    }

    void paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool isSelected) override
    {
        if (row % 2) { g.fillAll (Spaghettis()->getColour (Colours::searchpathsBackgroundAlternate)); }

        if (juce::isPositiveAndBelow (row, paths_.size())) {
        //
        const juce::Rectangle<int> r (width, height);
        
        g.setColour (isSelected ? Spaghettis()->getColour (Colours::searchpathsTextHighlighted)
                                : Spaghettis()->getColour (Colours::searchpathsText));
                                    
        g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
        g.drawText (paths_[row], r.reduced (4, 0), juce::Justification::centredLeft, true);
        //
        }
    }
    
    void listBoxItemClicked (int row, const juce::MouseEvent &) override
    {
        if (juce::isPositiveAndBelow (row, paths_.size()) == false) {
            ListBoxFunctions::update (listBox_, paths_, true);
        }
    }
    
    void deleteKeyPressed (int) override
    {
        removeSelectedPaths();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
    }
    
    void resized() override
    {
        listBox_.setBounds (setBarsBoundsAndGetRemaining());
        
        ListBoxFunctions::update (listBox_, paths_, false);
    }

    void listWasScrolled() override
    {
        ListBoxFunctions::update (listBox_, paths_, false);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (&listBox_);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void handleAsyncUpdate() override
    {
        Spaghettis()->setSearchPaths (paths_);
    }
    
    void setSearchPaths()
    {
        triggerAsyncUpdate();
    }
    
    void appendFullPathName (const juce::String& filepath)
    {
        paths_.addIfNotAlreadyThere (filepath);
        ListBoxFunctions::update (listBox_, paths_, true);
        setSearchPaths();
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

private:
    juce::ListBox listBox_;
    juce::StringArray paths_;

private:
    std::unique_ptr<juce::FileChooser> fileChooser_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchPathsComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

