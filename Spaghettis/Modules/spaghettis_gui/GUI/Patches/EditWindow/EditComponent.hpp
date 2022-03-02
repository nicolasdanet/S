
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class EditComponent :   protected EditFactoryHelper,    /* MUST be the first. */
                        public    BaseComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit EditComponent (Patch& owner, const juce::ValueTree& content) :
        EditFactoryHelper (this),
        BaseComponent (getIconsFactory()),
        owner_ (owner),
        editView_ (content),
        backgroundColour_ (Spaghettis()->getColour (Tags::Colors, Tags::PatchBackground))
    {
        addAndMakeVisible (editView_);
        
        setDefaultSize (this, 600, 300);
    }
    
    ~EditComponent() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
    }
    
    void resized() override
    {
        juce::Rectangle<int> bounds (setBarsBoundsAndGetRemaining());
        
        editView_.setBounds (bounds);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Patch& getPatch() const
    {
        return owner_;
    }
    
private:
    Patch& owner_;
    
private:
    EditView editView_;
    core::Cached<juce::Colour> backgroundColour_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

