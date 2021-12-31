
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PatchWindow : public BaseWindow {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PatchWindow (Patch& owner, const juce::ValueTree& content) :
        BaseWindow (Patch::getParameter (content, Ids::title).toString()),
        owner_ (owner)
    {
        setContentOwned (new PatchComponent (getPropertiesKeyName()), true);
        
        const int x = Patch::getParameter (content, Ids::x);
        const int y = Patch::getParameter (content, Ids::y);
        const int w = Patch::getParameter (content, Ids::width);
        const int h = Patch::getParameter (content, Ids::height);
        
        makeVisible (juce::Rectangle<int> (x, y, w, h));
    }

    ~PatchWindow() = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void closeButtonPressed() override
    {
        owner_.closeWindowButtonPressed (this);
    }

private:
    Patch& owner_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchWindow)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
