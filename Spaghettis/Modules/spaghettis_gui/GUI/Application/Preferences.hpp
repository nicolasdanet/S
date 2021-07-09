
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
        if (isValid (tree_)) {
        //
        for (const auto& section : tree_) {
        //
        if (isValidSection (section)) {
        
            DBG (section.getProperty (Ids::name).toString());
            
            for (const auto& parameter : section) {
            
                if (isValidParameter (parameter)) {

                    DBG (parameter.getProperty (Ids::item).toString());
                    
                }
            }
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

    static bool isValid (const juce::ValueTree& t)
    {
        return (t.isValid() && t.hasType (Ids::PREFERENCES));
    }
    
    static bool isValidSection (const juce::ValueTree& t)
    {
        return (t.hasType (Ids::SECTION) && t.hasProperty (Ids::name));
    }
    
    static bool isValidParameter (const juce::ValueTree& t)
    {
        return (t.hasType (Ids::PARAMETER)
                    && t.hasProperty (Ids::item)
                    && t.hasProperty (Ids::text)
                    && t.hasProperty (Ids::type)
                    && t.hasProperty (Ids::value));
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

