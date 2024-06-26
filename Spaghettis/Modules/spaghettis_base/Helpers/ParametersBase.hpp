
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParametersBase {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParametersBase (const juce::Font& font, const juce::Font& fontMonospaced, int w) :
        font_ (font),
        fontMonospaced_ (fontMonospaced),
        width_ (w)
    {
        jassert (w > 0);
    }

    ~ParametersBase() = default;
    
public:
    ParametersBase (const ParametersBase&) = default;
    ParametersBase (ParametersBase&&) = default;
    ParametersBase& operator = (const ParametersBase&) = default;
    ParametersBase& operator = (ParametersBase&&) = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Font getFont() const
    {
        return font_;
    }
    
    juce::Font getMonospacedFont() const
    {
        return fontMonospaced_;
    }
    
    int getRequiredWidth() const
    {
        return width_;
    }

    int getRequiredHeight() const
    {
        return font_.getHeight() * 1.5;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::Font font_;
    juce::Font fontMonospaced_;
    int width_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

