
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Strings {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static juce::String firstLetterCapitalized (const juce::String& s)
{
    return s.substring (0, 1).toUpperCase() + s.substring (1);
}

static juce::String upToWhitespace (const juce::String& s)
{
    return s.upToFirstOccurrenceOf (" ", false, false);
}

static juce::String removeWhitespaces (const juce::String& s)
{
    return s.removeCharacters (" ");
}

static juce::String withoutFileExtension (const juce::String& s)
{
    int k = s.lastIndexOfChar ('.');

    if (k >= 0) { return s.substring (0, k); }

    return s;
}

static bool startsWithWhitespace (const juce::String& s)
{
    return s.startsWith (" ");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static juce::String trimZerosOfInteger (juce::String t)
{
    const int i = t.indexOfChar ('.');
    
    if (i > 0) {
    //
    const int n = t.lastIndexOfAnyOf ("123456789");
    
    if (n < i) { t = t.substring (0, i); }
    //
    }
    
    return t;
}

static bool isIntegerPartTruncated (juce::String t, double f)
{
    if (std::abs (t.getIntValue() - static_cast<int> (f)) > 1) { return true; };
    
    return false;
}

static auto withNumberOfDigits (double f, int n)
{
    jassert (n > 0);
        
    juce::String s (f, n);
    juce::String t;
    
    for (auto c : s) {
        if (n <= 0) { break; }
        else {
            t += c; if (juce::CharacterFunctions::isDigit (c)) { --n; }
        }
    }
    
    return std::tuple<juce::String, bool> (trimZerosOfInteger (t), isIntegerPartTruncated (t, f));
}

static juce::String withNumberOfDigitsTruncated (double f, int n)
{
    const auto [text, truncated] = withNumberOfDigits (f, juce::jmax (1, n));
    
    if (truncated) { return text.dropLastCharacters (1) + "#"; }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

