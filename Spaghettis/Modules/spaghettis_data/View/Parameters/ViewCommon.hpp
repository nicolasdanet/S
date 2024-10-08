
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ParametersView;
class PresetsView;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct ViewCommon {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void paintHeader (juce::Graphics& g,
                const juce::Rectangle<int>& bounds,
                const juce::Font& font,
                const juce::String& name,
                int numberOfPanels,
                bool expanded);

static void buildPanel (const data::Data&, const PropertyLookAndFeel&, ParametersView&);
static void buildPanel (const std::vector<PresetElement>&, const PropertyLookAndFeel&, PresetsView&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

