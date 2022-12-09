
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String getContentBuffer (t_object* o)
{
    juce::String s;
    
    if (o) {
    //
    t_buffer* b = object_getBuffer (o);
    
    if (b && buffer_getSize (b)) {
    //
    char* t = atom_atomsToString (buffer_getSize (b), buffer_getAtoms (b));
    
    s = juce::String (t);
    
    PD_MEMORY_FREE (t);
    //
    }
    //
    }
    
    return s;
}

juce::File getPatchFile (t_glist* glist)
{
    const juce::String filename  (environment_getFileNameAsString (glist_getEnvironment (glist)));
    const juce::String directory (environment_getDirectoryAsString (glist_getEnvironment (glist)));

    return juce::File (directory).getChildFile (filename);
}

juce::Rectangle<int> getEditView (t_glist* glist)
{
    t_rectangle* view = glist_getEditView (glist);
    
    const int x = rectangle_getTopLeftX (view);
    const int y = rectangle_getTopLeftY (view);
    const int w = rectangle_getWidth (view);
    const int h = rectangle_getHeight (view);

    return juce::Rectangle<int> (x, y, w, h);
}

juce::Rectangle<int> getRunView (t_glist* glist)
{
    t_rectangle* view = glist_getRunView (glist);
    
    if (rectangle_isNothing (view)) { return getEditView (glist); }
    else {
    //
    const int x = rectangle_getTopLeftX (view);
    const int y = rectangle_getTopLeftY (view);
    const int w = rectangle_getWidth (view);
    const int h = rectangle_getHeight (view);

    return juce::Rectangle<int> (x, y, w, h);
    //
    }
}

/* Subpatches are created in two steps. */
/* First a dummy one, in order to let objects contained inside to be added. */
/* And again with the properly set values. */
/* Hide it during the process. */

bool getVisible (t_object* o)
{
    if (object_isCanvas (o)) {
    //
    t_glist* g = cast_glist (o);
    
    if (!glist_isRoot (g)) { return (buffer_getSize (object_getBuffer (o)) != 0) ? true : false; }
    //
    }
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setObjectAttributesForObject (Group& group, t_object* o)
{
    static DelegateCache delegate;
    
    group.addParameter (Tag::Class,
        NEEDS_TRANS ("Class"),
        NEEDS_TRANS ("Class of the object"),
        juce::String (class_getNameAsString (pd_class (o))),
        delegate).setEditable (false);
        
    group.addParameter (Tag::Content,
        NEEDS_TRANS ("Content"),
        NEEDS_TRANS ("Content of the box's buffer"),
        getContentBuffer (o),
        delegate).setEditable (false);
    
    group.addParameter (Tag::Inlets,
        NEEDS_TRANS ("Inlets"),
        NEEDS_TRANS ("List of inlets types"),
        object_getTypeOfInlets (o),
        delegate).setEditable (false);
    
    group.addParameter (Tag::Outlets,
        NEEDS_TRANS ("Outlets"),
        NEEDS_TRANS ("List of outlets types"),
        object_getTypeOfOutlets (o),
        delegate).setEditable (false);
        
    group.addParameter (Tag::X,
        NEEDS_TRANS ("Position X"),
        NEEDS_TRANS ("Box ordinate"),
        Coordinates::realToLocal (object_getX (o)),
        delegate).setHidden (true);
    
    group.addParameter (Tag::Y,
        NEEDS_TRANS ("Position Y"),
        NEEDS_TRANS ("Box abscissa"),
        Coordinates::realToLocal (object_getY (o)),
        delegate).setHidden (true);
    
    group.addParameter (Tag::Selected,
        NEEDS_TRANS ("Selected"),
        NEEDS_TRANS ("Is selected state"),
        static_cast<bool> (object_isSelected (o)),
        delegate).setHidden (true);
    
    group.addParameter (Tag::Visible,
        NEEDS_TRANS ("Visible"),
        NEEDS_TRANS ("Is visible state"),
        getVisible (o),
        delegate).setHidden (true);
}

void setObjectAttributesForPatch (Group& group, t_object* o)
{
    static DelegateCache delegate;
        
    t_glist* g = cast_glist (o);
    
    group.addParameter (Tag::Title,
        NEEDS_TRANS ("Title"),
        NEEDS_TRANS ("Patch name"),
        juce::String (symbol_getName (glist_getName (g))),
        delegate).setEditable (false);
    
    group.addParameter (Tag::EditView,
        NEEDS_TRANS ("Edit View"),
        NEEDS_TRANS ("Edit window geometry"),
        getEditView (g),
        delegate).setEditable (false);
    
    if (!glist_isRoot (g)) { setObjectAttributesForObject (group, o); }
    else {
    //
    group.addParameter (Tag::RunView,
        NEEDS_TRANS ("Run View"),
        NEEDS_TRANS ("Run window geometry"),
        getRunView (g),
        delegate).setEditable (false);
    
    group.addParameter (Tag::Path,
        NEEDS_TRANS ("Path"),
        NEEDS_TRANS ("File path"),
        getPatchFile (g).getFullPathName(),
        delegate).setEditable (false);
    //
    }
    
    group.addParameter (Tag::Undo,
        NEEDS_TRANS ("Undo"),
        NEEDS_TRANS ("Undoable action"),
        undomanager_getUndoLabel (glist_getUndoManager (g)),
        delegate).setHidden (true);
    
    group.addParameter (Tag::Redo,
        NEEDS_TRANS ("Redo"),
        NEEDS_TRANS ("Redoable action"),
        undomanager_getRedoLabel (glist_getUndoManager (g)),
        delegate).setHidden (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setObjectAttributes (Data& data, t_object* o)
{
    Group group (data.addGroup (Tag::Attributes));
    
    if (object_isCanvas (o)) { setObjectAttributesForPatch (group, o); }
    else {
        setObjectAttributesForObject (group, o);
    }
}

void setObjectParameters (Data& data, t_object* o)
{
    t_class* c = pd_class (o);
    
    if (class_hasParametersFunction (c)) {
    //
    static DelegateCache delegate;
    
    Group group (data.addGroup (Tag::Parameters));
    
    group.addParameter (Tag::Label,
        NEEDS_TRANS ("Label"),
        NEEDS_TRANS ("Parameter name in run view"),
        juce::String (symbol_getName (object_getLabel (o))),
        delegate);
    
    (*class_getParametersGetter (c)) (o, group);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree getObject (const UniquePath& u, struct _object* o, bool attributes, bool parameters)
{
    juce::ValueTree t (object_isCanvas (o) ? Id::PATCH : Id::OBJECT);
    
    t.setProperty (Id::identifier, Cast::toVar (u.getIdentifier()), nullptr);
    
    if (o) {
    //
    Data data (Id::DATA);
    
    if (attributes) { setObjectAttributes (data, o); }
    if (parameters) { setObjectParameters (data, o); }
    
    t.appendChild (data.asValueTree(), nullptr);
    //
    }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Report Report::object (const UniquePath& u, struct _object* o)
{
    return Report (getObject (u, o, true, true));
}

Report Report::objectAttributes (const UniquePath& u, struct _object* o)
{
    return Report (getObject (u, o, true, false));
}

Report Report::objectParameters (const UniquePath& u, struct _object* o)
{
    return Report (getObject (u, o, false, true));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void setLineAttributes (Data& data, int m, int n, bool b)
{
    static DelegateCache delegate;
    
    Group group (data.addGroup (Tag::Attributes));
    
    group.addParameter (Tag::Outlet,
        NEEDS_TRANS ("Outlet"),
        NEEDS_TRANS ("Index of source outlet"),
        m,
        delegate).setEditable (false);
    
    group.addParameter (Tag::Inlet,
        NEEDS_TRANS ("Inlet"),
        NEEDS_TRANS ("Index of destination inlet"),
        n,
        delegate).setEditable (false);
    
    if (b) {
    //
    group.addParameter (Tag::Selected,
        NEEDS_TRANS ("Selected"),
        NEEDS_TRANS ("Is selected state"),
        false,
        delegate).setHidden (true);
    //
    }
}

juce::ValueTree getLine (const UniquePath& u, struct _object* src, int m, struct _object* dest, int n, bool b)
{
    juce::ValueTree t (Id::LINE);
    
    t.setProperty (Id::identifier,     Cast::toVar (u.getIdentifier()), nullptr);
    t.setProperty (Id::source,         Cast::toVar (object_getUnique (src)), nullptr);
    t.setProperty (Id::destination,    Cast::toVar (object_getUnique (dest)), nullptr);
    
    Data data (Id::DATA); setLineAttributes (data, m, n, b);
    
    t.appendChild (data.asValueTree(), nullptr);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Report Report::lineAdded (const UniquePath& u, struct _object* src, int m, struct _object* dest, int n)
{
    jassert (!u.isRoot()); return Report (getLine (u, src, m, dest, n, true));
}
Report Report::lineChanged (const UniquePath& u, struct _object* src, int m, struct _object* dest, int n)
{
    jassert (!u.isRoot()); return Report (getLine (u, src, m, dest, n, false));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
