
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PresetElement {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PresetElement (const juce::String& label,
        const data::Parameter& parameter,
        const juce::var& tag) :
            label_ (label),
            parameter_ (parameter),
            tag_ (tag)
    {
        static_assert (std::is_nothrow_move_constructible_v<PresetElement> == true);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getLabel() const
    {
        /* Avoid assertion for empty string in juce::PropertyComponent constructor. */
        
        return label_.isNotEmpty() ? label_ : juce::String (" ");
    }
    
    data::Parameter getParameter() const
    {
        return parameter_;
    }

    juce::var getTag() const
    {
        return tag_;
    }
    
private:
    juce::String label_;
    data::Parameter parameter_;
    juce::var tag_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

