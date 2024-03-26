
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MouseHandler::MouseHandler (PatchView* view) : view_ (view), isForwarding_ (false), isDraggingView_ (false)
{
}

MouseHandler::~MouseHandler()
{
    if (isDraggingView_) { if (auto view = getEditView()) { view->dragAbort(); } }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PatchView* MouseHandler::getView() const
{
    return view_;
}

EditView* MouseHandler::getEditView() const
{
    return dynamic_cast<EditView*> (view_);
}

RunView* MouseHandler::getRunView() const
{
    return dynamic_cast<RunView*> (view_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool MouseHandler::forwardMouseDown (const juce::MouseEvent& e, PainterStrategy* p)
{
    isForwarding_   = Mouse::isCommandClick (e);
    isDraggingView_ = false;

    if (isForwarding_) { if (p) { p->mouseDown (e); } }
    
    return isForwarding_;
}

void MouseHandler::forwardMouseDrag (const juce::MouseEvent& e, PainterStrategy* p, DragFlag flag)
{
    if (isForwarding_) { if (p) { p->mouseDrag (e); } }
    else {
    //
    isDraggingView_ = true;
        
    if (auto view = getEditView()) { view->handleMouseDrag (e, flag); }
    //
    }
}

void MouseHandler::forwardMouseUp (const juce::MouseEvent& e, PainterStrategy* p)
{
    if (isForwarding_) { if (p) { p->mouseUp (e); } }
    else {
    //
    if (auto view = getEditView()) { view->handleMouseUp (e); }
    //
    }
    
    isDraggingView_ = false;
    isForwarding_   = false;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
