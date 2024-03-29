
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PresetsView {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend struct ViewCommon;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PresetsView (std::vector<PresetElement>&&, const ParametersBase&);
    
    ~PresetsView() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Font getFont() const;
    juce::String getName() const;

public:
    juce::ConcertinaPanel& getConcertinaPanel();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void resizeConcertinaPanel (const juce::Rectangle<int>&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addPanel (juce::PropertyPanel*);

private:
    juce::ConcertinaPanel panel_;
    std::vector<PresetElement> elements_;
    ParametersBase base_;
        
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetsView)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

