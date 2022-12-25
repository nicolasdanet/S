
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class EditInspector;
class EditPort;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class EditView : public View, private juce::AsyncUpdater {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend class EditPort;
friend class EditHand;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit EditView (Patch&, const juce::ValueTree&);
    
    ~EditView();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void setPort (EditPort* owner)
    {
        port_ = owner;
    }
    
    EditPort* getPort() const
    {
        jassert (port_ != nullptr); return port_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void attach (EditInspector*);
    void detach (EditInspector*);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool hasDragAction() const;
    
private:
    std::optional<juce::Point<int>> getRealMousePosition() const;
    std::optional<juce::Point<int>> getRealPositionOfSelectedObjects (juce::Point<int>) const;
    juce::Rectangle<int> getRealVisibleArea() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void selectAll();
    void deselectAll();
    void select (const juce::Rectangle<int>&);

public:
    bool hasSelected() const;
    bool hasSelectedObject() const;
    bool hasOnlyOnePatchSelected() const;
    bool hasPaste() const;
    core::Item getItemForInspector() const;
    
private:
    bool selectObjects (const juce::Rectangle<int>&);
    bool selectLines (const juce::Rectangle<int>&);
    int getNumberOfSelectedObjects() const;
    int getNumberOfSelectedLines() const;
    ObjectComponent* getSelectedObject() const;
    LineComponent* getSelectedLine() const;
    void setPaste();

public:
    void dragObjectsStart();
    void dragObjects (juce::Point<int>);

public:
    void moveBack();
    void moveFront();
    void snapToGrid();

public:
    void undo();
    void redo();
    bool hasUndo() const;
    bool hasRedo() const;
    juce::String getUndoAction() const;
    juce::String getRedoAction() const;

public:
    void cut();
    void copy();
    void paste();
    void duplicate();
    void remove();
    
public:
    void encapsulate();
    void deencapsulate();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void handleAsyncUpdate() override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    ObjectComponent* getObjectComponent (core::UniqueId) const override;
    LineComponent* getLineComponent (core::UniqueId) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void show (ObjectComponent*, const juce::Rectangle<int>&) override;
    void hide (ObjectComponent*) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    float getScale() const override;
    
private:
    void setScale (float f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Rectangle<int> getPaintedAreaFromBounds (const juce::Rectangle<int>&) override;
    juce::Rectangle<int> getBoundsFromPaintedArea (const juce::Rectangle<int>&) override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
    void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
    void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override;
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
    
private:
    void addComponent (const juce::ValueTree&);
    void removeComponent (const juce::ValueTree&);
    void initialize (const juce::ValueTree&);

private:
    void updateOrder();

private:
    core::Cached<juce::String> undo_;
    core::Cached<juce::String> redo_;
    core::Cached<juce::Colour> patchBackgroundColour_;
    Table<ObjectComponent> objects_;
    Table<LineComponent> lines_;
    float scale_;
    std::unique_ptr<DragAction> drag_;
    EditInspector* inspector_;
    EditPort* port_;
    
private:
    inline static bool hasPaste_ = false;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditView)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

