
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour ConsoleConverter::getColourFromElement (const Logger::MessagesElement& e, bool isSelected)
{
    if (isSelected) {

        core::UniquePath p = Logger::getUniquePath (e);
        
        if (p.isValid()) {
            if (Spaghettis()->getPatches().contains (p)) {
                return Colours::fetchColour (Colours::listBoxTextHighlighted);
            }
        }
    }

    return Logger::getColourForType (Logger::getType (e));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------