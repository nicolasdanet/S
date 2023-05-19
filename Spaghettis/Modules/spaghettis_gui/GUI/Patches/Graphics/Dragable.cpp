
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Dragable::Dragable (View* view) : view_ (view), isForwarding_ (false), isDraggingView_ (false)
{
}

Dragable::~Dragable()
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

EditView* Dragable::getEditView() const
{
    return View::asEditView (getView());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Dragable::forwardMouseDown (const juce::MouseEvent& e, PainterPolicy* p)
{
    isForwarding_       = Mouse::isCommandClick (e);
    isDraggingView_     = false;

    if (isForwarding_) { if (p) { p->mouseDown (e); } }
    
    return isForwarding_;
}

void Dragable::forwardMouseDrag (const juce::MouseEvent& e, PainterPolicy* p, DragFlag flag)
{
    if (isForwarding_) { if (p) { p->mouseDrag (e); } }
    else {
        isDraggingView_ = true; if (auto view = getEditView()) { view->handleMouseDrag (e, flag); }
    }
}

void Dragable::forwardMouseUp (const juce::MouseEvent& e, PainterPolicy* p)
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
