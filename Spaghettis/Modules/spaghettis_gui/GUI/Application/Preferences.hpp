
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Preferences {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Preferences() : tree_ (Preferences::getDefault())
    {
        if (tree_.isValid() && tree_.hasType (Ids::PREFERENCES)) {
        //
        for (const auto& child : tree_) {
        //
        if (child.hasType (Ids::SECTION) && child.hasProperty (Ids::name)) {
            DBG (child.getProperty (Ids::name).toString());
        }
        //
        }
        //
        }
    }
    
    ~Preferences() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static juce::ValueTree getDefault()
    {
        juce::ValueTree v { Ids::PREFERENCES, {}, {
        //
        { Ids::SECTION, {{ Ids::name, "Editing" }},
            {
                { Ids::PARAMETER, {
                    { Ids::item,  "snapToGrid" },
                    { Ids::text,  NEEDS_TRANS ("Snap to grid") },
                    { Ids::type,  "boolean" },
                    { Ids::value, true }
                }},
                { Ids::PARAMETER, {
                    { Ids::item, "gridSize" },
                    { Ids::text,  NEEDS_TRANS ("Grid size") },
                    { Ids::type, "integer" },
                    { Ids::value, 12 }
                }}
            }
        },
        { Ids::SECTION, {{ Ids::name, "Fonts" }},
            {
                { Ids::PARAMETER, {
                    { Ids::item, "defaultFontSize" },
                    { Ids::text,  NEEDS_TRANS ("Default font size") },
                    { Ids::type, "integer" },
                    { Ids::value, 12 }
                }}
            }
        }
        //
        }};

        return v;
    }

private:
    juce::ValueTree tree_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Preferences)
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

