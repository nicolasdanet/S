
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Colour getColourFromFileStatus (const juce::File& f)
{
    if (f.isDirectory()) { return Colours::fetchColour (Colours::parametersDirectoryText); }

    return Colours::fetchColour (Colours::parametersDirectoryTextWrong);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectoryEditor::paint (juce::Graphics& g)
{
    const juce::File f (value_.toString());
    const juce::Rectangle<int> r (getLocalBounds().reduced (4, 2));
    const juce::String text (f.getFullPathName());
    
    g.fillAll (Colours::fetchColour (Colours::parametersColourBackground));
    g.setFont (font_);
    g.setColour (getColourFromFileStatus (f));
    g.drawText (text, r, juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DirectoryEditor::setDirectory (const juce::File& file)
{
    if (file.isDirectory()) {
    //
    value_.setValue (file.getFullPathName());
    
    Directories::getInstance()->setDefaultOpen (file.getParentDirectory());
    //
    }
}

void DirectoryEditor::chooseDirectory()
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

void DirectoryEditor::mouseDown (const juce::MouseEvent&)
{
    if (isEnabled()) { chooseDirectory(); }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
