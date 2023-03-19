
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

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
    snap
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::PopupMenu::Options getContextMenuOptions (ObjectComponent* c)
{
    return juce::PopupMenu::Options().withDeletionCheck (*c);
}

juce::PopupMenu getContextMenu (ObjectComponent* c)
{
    juce::PopupMenu m;
    
    m.addItem (Contextual::help,    NEEDS_TRANS ("Help"));
    m.addSeparator();
    m.addItem (Contextual::open,    NEEDS_TRANS ("Open"),       c->isPatch());
    m.addSeparator();
    m.addItem (Contextual::back,    NEEDS_TRANS ("Move Back"),  !c->isLocked());
    m.addItem (Contextual::front,   NEEDS_TRANS ("Move Front"), !c->isLocked());
    m.addItem (Contextual::snap,    NEEDS_TRANS ("Snap"),       !c->isLocked());
    
    return m;
}

auto contextMenuCallback (ObjectComponent* c, EditView* view)
{
    auto f = [v = WeakPointer<EditView> (view), o = WeakPointer<ObjectComponent> (c)] (int result)
    {
    //
    if (v.getComponent() && o.getComponent()) {
        switch (result) {
            case Contextual::help   : DBG ("HELP"); break;
            case Contextual::open   : v->getPatchRoot().openPatchWindow (o->getIdentifier());   break;
            case Contextual::back   : v->moveBack (o.getComponent());                           break;
            case Contextual::front  : v->moveFront (o.getComponent());                          break;
            case Contextual::snap   : v->snapToGrid (o.getComponent());                         break;
            default                 : break;
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

void ContextMenu::open (const juce::MouseEvent&, ObjectComponent* c)
{
    const juce::PopupMenu::Options options (getContextMenuOptions (c));
    
    juce::PopupMenu m (getContextMenu (c));
    
    m.showMenuAsync (options, contextMenuCallback (c, view_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
