
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ContextMenu::ContextMenu (EditView* view) : view_ (view)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

enum Contextual {
    none    = 0,
    help,
    open,
    back,
    front,
    snap,
    add,
    include,
    exclude
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::PopupMenu getContextMenuForView (EditView* view)
{
    juce::PopupMenu m;
    
    m.addItem (Contextual::help, NEEDS_TRANS ("Help"));
    m.addSeparator();
    m.addItem (Contextual::add,  NEEDS_TRANS ("Add Object"), !view->isAbstractionOrInside());
    
    return m;
}

auto getContextMenuCallbackForView (EditView* view, const juce::MouseEvent& e)
{
    auto f = [v = WeakPointer<EditView> (view), pt = e.getPosition()] (int result)
    {
    //
    if (v.getComponent()) {
        switch (result) {
            case Contextual::help : Broadcast::openHelp (v->getIdentifier());               break;
            case Contextual::add  : v->makerOpen (core::Point::Scaled (pt, v->getScale())); break;
            default               : break;
        }
    }
    //
    };
        
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::PopupMenu getContextMenuForObject (ObjectComponent* c)
{
    juce::PopupMenu m;
    
    m.addItem (Contextual::help,        NEEDS_TRANS ("Help"));
    m.addSeparator();
    m.addItem (Contextual::open,        NEEDS_TRANS ("Open"),       c->isPatch());
    m.addSeparator();
    m.addItem (Contextual::include,     NEEDS_TRANS ("Include"),    !c->isLocked() && c->isGraphic());
    m.addItem (Contextual::exclude,     NEEDS_TRANS ("Exclude"),    !c->isLocked() && c->isGraphic());
    m.addSeparator();
    m.addItem (Contextual::back,        NEEDS_TRANS ("Move Back"),  !c->isLocked());
    m.addItem (Contextual::front,       NEEDS_TRANS ("Move Front"), !c->isLocked());
    m.addItem (Contextual::snap,        NEEDS_TRANS ("Snap"),       !c->isLocked());
    
    return m;
}

auto getContextMenuCallbackForObject (EditView* view, const juce::MouseEvent&, ObjectComponent* c)
{
    auto f = [v = WeakPointer<EditView> (view), o = WeakPointer<ObjectComponent> (c)] (int result)
    {
    //
    if (v.getComponent() && o.getComponent()) {
        switch (result) {
            case Contextual::help       : Broadcast::openHelp (o->getIdentifier());                 break;
            case Contextual::open       : v->getPatchRoot().showEditWindow (o->getIdentifier());    break;
            case Contextual::include    : v->include (o.getComponent());                            break;
            case Contextual::exclude    : v->exclude (o.getComponent());                            break;
            case Contextual::back       : v->moveBack (o.getComponent());                           break;
            case Contextual::front      : v->moveFront (o.getComponent());                          break;
            case Contextual::snap       : v->snapToGrid (o.getComponent());                         break;
            default                     : break;
        }
    }
    //
    };
        
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ContextMenu::open (const juce::MouseEvent& e)
{
    juce::PopupMenu m (getContextMenuForView (view_));
    
    m.showMenuAsync (juce::PopupMenu::Options(), getContextMenuCallbackForView (view_, e));
}

void ContextMenu::open (const juce::MouseEvent& e, ObjectComponent* c)
{
    juce::PopupMenu m (getContextMenuForObject (c));
    
    m.showMenuAsync (juce::PopupMenu::Options(), getContextMenuCallbackForObject (view_, e, c));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
