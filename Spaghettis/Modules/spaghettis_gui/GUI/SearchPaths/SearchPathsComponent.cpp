
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

SearchPathsComponent::SearchPathsComponent (juce::ApplicationCommandManager& command,
    juce::PropertiesFile* propertiesFile,
    const juce::String& keyName) :
        SearchPathsFactoryHelper (this),
        BaseComponent (getIconsFactory(), Spaghettis()->getMenu(), command, propertiesFile, keyName),
        paths_ (Spaghettis()->getSearchPaths())
{
    CommandsHandler::addCloseWindowCommand (this);
    
    listBox_.setModel (this);
    ListBoxFunctions::initialize (listBox_, true);
    ListBoxFunctions::update (listBox_, paths_);
    addAndMakeVisible (listBox_);
        
    setOpaque (true); setSize (400, 500);
}
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SearchPathsComponent::removeSelectedPaths()
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
    
    if (done) { ListBoxFunctions::update (listBox_, paths_); setSearchPaths(); }
}

void SearchPathsComponent::addPaths()
{
    chooseAndAppendFolder();
}

void SearchPathsComponent::sortPaths()
{
    if (paths_.size() > 1) {
    //
    paths_.sortNatural();
    
    ListBoxFunctions::update (listBox_, paths_); setSearchPaths();
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int SearchPathsComponent::getNumRows()
{
    return ListBoxFunctions::getNumberOfRowsToDraw (paths_.size());
}

void SearchPathsComponent::paintListBoxItem (int row, juce::Graphics& g, int w, int h, bool isSelected)
{
    ListBoxFunctions::paintItem (paths_, row, g, w, h, isSelected);
}

void SearchPathsComponent::listBoxItemClicked (int row, const juce::MouseEvent &)
{
    if (juce::isPositiveAndBelow (row, paths_.size()) == false) {
        ListBoxFunctions::update (listBox_, paths_);
    }
}

void SearchPathsComponent::deleteKeyPressed (int)
{
    removeSelectedPaths();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SearchPathsComponent::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::windowsBackground));
}

void SearchPathsComponent::resized()
{
    listBox_.setBounds (setBoundsForBarsAndGetRemaining());
    
    ListBoxFunctions::update (listBox_, paths_, false);
}

void SearchPathsComponent::listWasScrolled()
{
    ListBoxFunctions::update (listBox_, paths_, false);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool SearchPathsComponent::tryGrabFocus()
{
    return tryGrabFocusForComponent (&listBox_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SearchPathsComponent::handleAsyncUpdate()
{
    Spaghettis()->setSearchPaths (paths_);
}

void SearchPathsComponent::setSearchPaths()
{
    triggerAsyncUpdate();
}

void SearchPathsComponent::appendFullPathName (const juce::String& filepath)
{
    paths_.addIfNotAlreadyThere (filepath);
    ListBoxFunctions::update (listBox_, paths_);
    setSearchPaths();
}

void SearchPathsComponent::appendFile (const juce::File& file)
{
    if (file.isDirectory()) {
    //
    appendFullPathName (file.getFullPathName());
    
    Spaghettis()->setCurrentOpenDirectory (file.getParentDirectory());
    //
    }
}

void SearchPathsComponent::chooseAndAppendFolder()
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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
