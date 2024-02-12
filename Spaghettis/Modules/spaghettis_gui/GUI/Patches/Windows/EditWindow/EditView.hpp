
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

class EditView : public PatchView, private juce::AsyncUpdater {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit EditView (const PatchBase&);
    
    ~EditView();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isDragging() const;
    bool isAbstractionOrInside() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool hasUndo() const;
    bool hasRedo() const;
    bool hasPaste() const;
    bool hasSelected() const;
    bool hasSelectedObject() const;
    bool hasOnlyOnePatchSelected() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getUndoAction() const;
    juce::String getRedoAction() const;
    int getNumberOfSelectedObjects() const;
    int getNumberOfSelectedLines() const;
    ObjectComponent* getSelectedObject() const;
    LineComponent* getSelectedLine() const;
    ObjectComponent* getObjectComponent (core::UniqueId) const override;
    LineComponent* getLineComponent (core::UniqueId) const;
    
public:
    std::optional<core::Point::Real> getMousePosition() const;

public:
    Synchronizer getSynchronized() const;
    EditPort* getPort() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setPort (EditPort*);
    void setMouseCursorRecursive (const juce::MouseCursor&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void dragAbort();

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
    void hideLocator (const juce::MouseEvent&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void handleMouseDown (const juce::MouseEvent&);
    void handleMouseDrag (const juce::MouseEvent&, DragFlag);
    void handleMouseUp (const juce::MouseEvent&);

private:
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void selectAll();
    void deselectAll();
    void select (const juce::Rectangle<int>&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void dragStart();
    void drag (core::Vector::Real);
    void resize (core::Vector::Real);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void moveBack (ObjectComponent* c = nullptr);
    void moveFront (ObjectComponent* c = nullptr);
    void snapToGrid (ObjectComponent* c = nullptr);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void cut();
    void copy();
    void paste();
    void duplicate();
    void remove();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void undo();
    void redo();

public:
    void encapsulate();
    void deencapsulate();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void requireMaker (bool);
    void openMaker (core::Point::Scaled);
    void handleMaker (core::Point::Real, const juce::String&);
    void dismissMaker();
    
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

public:
    void show (ObjectComponent*, const juce::Rectangle<int>&) override;
    void hide (ObjectComponent*) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    float getScale() const override;
    
public:
    void setScale (float f);

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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    Maker maker_;
    bool isAbstraction_;
    bool isLocked_;
    data::Cached<juce::String> undo_;
    data::Cached<juce::String> redo_;
    data::Cached<juce::Colour> patchBackgroundColour_;
    Table<ObjectComponent> objects_;
    Table<LineComponent> lines_;
    float scale_;
    std::unique_ptr<DragStrategy> drag_;
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

