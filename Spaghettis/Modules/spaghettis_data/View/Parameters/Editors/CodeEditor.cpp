
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

CodeEditor::CodeEditor (const data::Parameter& p, const juce::Font& font) :
    value_ (p.getValueAsValue (false)),
    document_(),
    editor_ (document_, getTokenizer (p))
{
    editor_.setFont (font);
    editor_.setScrollbarThickness (0);
    editor_.setReadOnly (isEnabled());
    editor_.setLineNumbersShown (false);
    editor_.loadContent (value_.toString());
    addAndMakeVisible (editor_);
}

juce::CodeTokeniser* CodeEditor::getTokenizer (const data::Parameter& p)
{
    // DBG (p.getComment());

    return nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void CodeEditor::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::parametersColourBackground));
}

void CodeEditor::resized()
{
    editor_.setBounds (getLocalBounds().withTrimmedTop (3));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
