
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class TextBlock {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit TextBlock (juce::String s = juce::String()) : text_ (s)
    {
    }
    
    ~TextBlock() = default;

    TextBlock (const TextBlock&) = default;
    TextBlock (TextBlock&&) = default;
    TextBlock& operator = (const TextBlock&) = default;
    TextBlock& operator = (TextBlock&&) = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String toString() const
    {
        return text_;
    }
    
private:
    juce::String text_;
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
