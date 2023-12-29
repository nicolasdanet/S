
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

DragHandler::DragHandler (PatchView* view) : view_ (view), isForwarding_ (false), isDraggingView_ (false)
{
}

DragHandler::~DragHandler()
{
    if (isDraggingView_) {
        if (auto view = getEditView()) {
            view->handleMouseDragAbort();
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditView* DragHandler::getEditView() const
{
    return PatchView::asEditView (getView());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool DragHandler::forwardMouseDown (const juce::MouseEvent& e, PainterPolicy* p)
{
    isForwarding_   = Mouse::isCommandClick (e);
    isDraggingView_ = false;

    if (isForwarding_) { if (p) { p->mouseDown (e); } }
    
    return isForwarding_;
}

void DragHandler::forwardMouseDrag (const juce::MouseEvent& e, PainterPolicy* p, DragFlag flag)
{
    if (isForwarding_) { if (p) { p->mouseDrag (e); } }
    else {
        isDraggingView_ = true; if (auto view = getEditView()) { view->handleMouseDrag (e, flag); }
    }
}

void DragHandler::forwardMouseUp (const juce::MouseEvent& e, PainterPolicy* p)
{
    if (isForwarding_) { if (p) { p->mouseUp (e); } }
    else {
        if (auto view = getEditView()) { view->handleMouseUp (e); }
    }
    
    isDraggingView_ = false;
    isForwarding_   = false;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
