
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ObjectComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class PatchView :   public PatchBase,
                    public juce::Component,
                    public juce::ValueTree::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PatchView (const PatchBase& base) : PatchBase (base)
    {
    }
    
    virtual ~PatchView() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    virtual ObjectComponent* getObjectComponent (core::UniqueId) const = 0;

public:
    virtual void show (ObjectComponent*, const juce::Rectangle<int>&) = 0;
    virtual void hide (ObjectComponent*) = 0;

public:
    virtual float getScale() const = 0;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchView)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

