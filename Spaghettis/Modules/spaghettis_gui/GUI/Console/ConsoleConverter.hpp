
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct ConsoleConverter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static juce::Colour getColourFromElement (const Logger::MessagesElement&, bool);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template<> struct ListBoxConverter<Logger::MessagesElement> {

    static juce::Colour getColour (const Logger::MessagesElement& e, bool isSelected)
    {
        return ConsoleConverter::getColourFromElement (e, isSelected);
    }
    
    static juce::String getText (const Logger::MessagesElement& e)
    {
        return Logger::getText (e);
    }

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
