
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectorySelector::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectorySelector::setDirectory (const juce::File& file)
{
    if (file.isDirectory()) {
    //
    value_.setValue (file.getFullPathName());
    
    Directories::getInstance()->setDefaultOpen (file.getParentDirectory());
    //
    }
}

void DirectorySelector::chooseDirectory()
{
    const juce::File initial (Directories::getInstance()->getDefaultOpen());
                        
    fileChooser_ = std::make_unique<juce::FileChooser> (NEEDS_TRANS ("Choose a directory..."), initial);
    
    auto callback = [this] (const juce::FileChooser& fileChooser)
    {
        auto files = fileChooser.getResults(); for (const auto& f : files) { setDirectory (f); }
    };
    
    const int t = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
    
    fileChooser_->launchAsync (t, callback);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectorySelector::mouseDown (const juce::MouseEvent&)
{
    if (isEnabled()) { chooseDirectory(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
