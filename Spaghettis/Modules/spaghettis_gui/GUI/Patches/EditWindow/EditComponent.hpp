
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
    explicit EditComponent (Patch& owner, const juce::ValueTree& tree) :
        EditFactoryHelper (this),
        BaseComponent (getIconsFactory()),
        owner_ (owner),
        editView_ (tree),
        editPort_ (editView_)
    {
        addAndMakeVisible (editPort_);
        addChildComponent (zoomComponent_);
        
        addMenuBarCommand (Commands::zoomIn,    [this]() { zoomIn();    } );
        addMenuBarCommand (Commands::zoomOut,   [this]() { zoomOut();   } );
        addMenuBarCommand (Commands::zoomReset, [this]() { zoomReset(); } );
        
        setOpaque (true); setSize (600, 300);
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
        juce::Rectangle<int> bounds (setBoundsForBarsAndGetRemaining());
        
        editPort_.setBounds (bounds);
        
        updateZoomComponent();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void zoomIn()
    {
        editPort_.zoomIn();
    }
    
    void zoomOut()
    {
        editPort_.zoomOut();
    }
    
    void zoomReset()
    {
        editPort_.zoomReset();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void updateZoomComponent()
    {
        const int n = editPort_.getZoom();
        
        juce::Rectangle<int> b (getBoundsForToolbar());
        
        if (b.getWidth() < 300) { zoomComponent_.setVisible (false); }
        else {
        //
        zoomComponent_.setValue (n);
        zoomComponent_.setBounds (b.removeFromRight (80).reduced (10, 0));
        zoomComponent_.setVisible (true);
        //
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (this);
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
    EditPort editPort_;
    ZoomComponent zoomComponent_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

