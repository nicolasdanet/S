
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Resizer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Resizer (juce::Component& c) :
        owner_ (c),
        separator_ (&c, &constrainer_, juce::ResizableEdgeComponent::leftEdge)
    {
        constrainer_.setMinimumWidth (minimumWidth_);
        separator_.setAlwaysOnTop (true);
        owner_.addAndMakeVisible (separator_);
    }
    
    ~Resizer() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    constexpr int getDefaultWidth()
    {
        return defaultWidth_;
    }

    constexpr int getMinimumWidth()
    {
        return minimumWidth_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void update()
    {
        separator_.setBounds (owner_.getLocalBounds().withWidth (edgeWidth_));
    }

private:
    juce::Component& owner_;
    juce::ComponentBoundsConstrainer constrainer_;
    juce::ResizableEdgeComponent separator_;

private:
    static constexpr int defaultWidth_ = 300;
    static constexpr int minimumWidth_ = 150;
    static constexpr int edgeWidth_    = 4;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resizer)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

