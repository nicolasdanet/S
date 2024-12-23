
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void initializeLines (std::vector<std::unique_ptr<InfoLine>>& lines,
    const data::Parameter& p,
    const juce::Font& font,
    InfoEditor* owner)
{
    const juce::StringArray a (juce::StringArray::fromLines (p.getValueTyped<InfoBlock>().toString()));
    
    for (const auto& s : a)     { lines.push_back (std::make_unique<InfoLine> (s, font)); }
    for (const auto& l : lines) {
        l->setEnabled (false);
        owner->addAndMakeVisible (l.get());
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

InfoEditor::InfoEditor (const data::Parameter& p, const juce::Font& font)
{
    initializeLines (lines_, p, font, this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int InfoEditor::getNumberOfLines() const
{
    return static_cast<int> (lines_.size());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InfoEditor::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::parametersColourBackground));
}

void InfoEditor::resized()
{
    const int size = static_cast<int> (lines_.size());
    
    if (size) {
    //
    juce::Rectangle<int> bounds (getLocalBounds());

    const int h = bounds.getHeight() / size;
    
    /* Two step to avoid the pixel offset due to integer rounding error. */
    
    if (size > 1) {
        for (int i = 0; i < size - 1; ++i) { lines_[i]->setBounds (bounds.removeFromTop (h)); }
    }
    
    lines_.back()->setBounds (bounds);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
