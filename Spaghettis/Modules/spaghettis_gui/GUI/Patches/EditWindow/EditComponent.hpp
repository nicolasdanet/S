
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
        
        zoomUpdate();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void zoomIn()
    {
        editPort_.zoomIn();     zoomUpdate();
    }
    
    void zoomOut()
    {
        editPort_.zoomOut();    zoomUpdate();
    }
    
    void zoomReset()
    {
        editPort_.zoomReset();  zoomUpdate();
    }

private:
    void zoomUpdate()
    {
        DBG (editPort_.getZoom());
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
    ZoomComponent zoom_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

