
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour ConsoleConverter::getColourFromElement (const LoggerMessage& e, bool isSelected)
{
    if (isSelected) {

        core::UniquePath p = e.getUniquePath();
        
        if (p.isValid()) {
            if (Spaghettis()->getPatches().contains (p)) {
                return Colours::fetchColour (Colours::listBoxTextHighlighted);
            }
        }
    }

    return Logger::getColourForType (e.getType());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------