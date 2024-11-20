
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String trimLeadingWhitespaces (const juce::String& s)
{
    if (s.startsWith ("    "))      { jassert (s.length() >= 4); return s.substring (4); }
    else if (s.startsWith ("  "))   { jassert (s.length() >= 2); return s.substring (2); }
    else {
        return s.trimStart();
    }
}

int fetchContent (data::Group& group, const juce::StringArray& table, int i, int size)
{
    const juce::String key (Strings::removeWhitespaces (table[i]));
    const juce::String label (table[i]);
    
    TextBlock text;
    
    int k = 0;
    
    for (int j = i + 1; j < size; ++j) {
        const juce::String s (table[j]);
        if (Strings::startsWithWhitespace (s)) {
            if (k) { text.append (juce::newLine); }
            text.append (trimLeadingWhitespaces (s));
            k++;
        } else {
            break;
        }
    }

    if (k) { group.addParameter (key, label, label, text).setEditable (false); }
    
    return k;
}

void fetchParameters (data::Group& group, const juce::StringArray& table)
{
    const int size = table.size();
    
    for (int i = 0; i < size; ++i) {
        if (!Strings::startsWithWhitespace (table[i])) {
            i += fetchContent (group, table, i, size);
        }
    }
}

void parseResources (data::Group& group, const juce::String& data)
{
    juce::StringArray table (juce::StringArray::fromLines (data));
    
    table.removeEmptyStrings (false);
    
    fetchParameters (group, table);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DocumentationElement::build (const juce::String& data)
{
    data::Group group (data_.addGroup (Tag::Documentation));
    
    parseResources (group, data);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
