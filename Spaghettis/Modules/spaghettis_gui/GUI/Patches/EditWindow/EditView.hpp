
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class EditView :    public  juce::Component,
                    private juce::ValueTree::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit EditView (const juce::ValueTree& tree) :
        tree_ (tree),
        background_ (Spaghettis()->getCachedColour (Tags::PatchBackground)),
        objects_ (*this),
        lines_ (*this)
    {
        tree_.addListener (this);
        background_.attach (PainterPolicy::repainter (this));
        initialize();
        BaseComponent::setDefaultWithSize (this);
    }
    
    ~EditView()
    {
        lines_.clear(); objects_.clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ObjectComponent* getObject (core::UniqueId);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
    void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;

private:
    void addComponent (const juce::ValueTree&);
    void removeComponent (const juce::ValueTree&);
    void initialize();
    
private:
    juce::ValueTree tree_;
    core::Cached<juce::Colour> background_;
    core::Table<core::Object, ObjectComponent> objects_;
    core::Table<core::Line, LineComponent> lines_;
        
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditView)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

