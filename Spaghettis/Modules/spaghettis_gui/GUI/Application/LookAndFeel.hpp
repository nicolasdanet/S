
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class LookAndFeel : public juce::LookAndFeel_V4 {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    LookAndFeel()
    {
        #if defined ( JUCE_MAC )
        
            font_ = juce::Font (18.0);
        
        #else
            
            font_ = juce::Font (18.0, juce::Font::bold);
            
        #endif
        
        setColour (juce::TextEditor::outlineColourId,   juce::Colours::transparentBlack);
        setColour (juce::TextEditor::shadowColourId,    juce::Colours::transparentBlack);
        setColour (juce::ScrollBar::thumbColourId,      juce::Colours::grey);
    
        Colours::initialize (this);
    }
    
    ~LookAndFeel() = default;

public:
    int getDefaultMenuBarHeight() override
    {
        return 26;
    }

    juce::Font getMenuBarFont (juce::MenuBarComponent&, int, const juce::String&) override
    {
        return font_;
    }

    void drawMenuBarBackground (juce::Graphics& g, int, int, bool, juce::MenuBarComponent& m) override
    {
        g.fillAll (m.findColour (Colours::menubarBackground));
    }

    const juce::Font& getConsoleFont() const
    {
        return font_;
    }
    
private:
    juce::Font font_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
