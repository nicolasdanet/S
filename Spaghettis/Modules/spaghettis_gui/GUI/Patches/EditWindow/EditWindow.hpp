
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class EditWindow : public PatchWindow {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit EditWindow (Patch& owner, const juce::ValueTree& content) : PatchWindow (owner, content)
    {
        setContentOwned (new EditComponent (getPropertiesKeyName()), true);
        
        const int x = Attributes::getParameter (content, Ids::x);
        const int y = Attributes::getParameter (content, Ids::y);
        const int w = Attributes::getParameter (content, Ids::width);
        const int h = Attributes::getParameter (content, Ids::height);
        
        makeVisible (juce::Rectangle<int> (x, y, w, h));
    }

    ~EditWindow() = default;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
