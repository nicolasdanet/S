
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

bool getVisible (t_object* o)
{
    t_glist *owner = object_getOwner (o);
    
    /* To avoid ugly jumps objects are invisibles first when pasting. */
    /* They are updated right after once done. */
    
    if (owner && glist_isPasting (owner)) { return false; }
    
    /* Subpatches are created in two steps. */
    /* First a dummy one, in order to let objects contained inside to be added. */
    /* And again with the properly set values. */
    /* Hide it during the process. */

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

void setObjectAttributesClass (data::Group& group, t_object* o, const Tags& t)
{
    static data::DelegateCache delegate;
    
    if (t.contains (Tag::Class)) {
        group.addParameter (Tag::Class,
            NEEDS_TRANS ("Class"),
            NEEDS_TRANS ("Class of the object"),
            object_isDummy (o) ? juce::String() : makeString (class_getNameAsString (pd_class (o))),
            delegate);
    }
}

void setObjectAttributesForObject (data::Group& group, t_object* o, const Tags& t)
{
    static data::DelegateCache delegate;
    
    if (t.contains (Tag::Content)) {
        group.addParameter (Tag::Content,
            NEEDS_TRANS ("Content"),
            NEEDS_TRANS ("Content of the box's buffer"),
            object_getBufferAsString (o),
            delegate).setHidden (true);
    }
    
    if (t.contains (Tag::Inlets)) {
        group.addParameter (Tag::Inlets,
            NEEDS_TRANS ("Inlets"),
            NEEDS_TRANS ("List of inlets types"),
            object_getTypeOfInlets (o),
            delegate);
    }
    
    if (t.contains (Tag::Outlets)) {
        group.addParameter (Tag::Outlets,
            NEEDS_TRANS ("Outlets"),
            NEEDS_TRANS ("List of outlets types"),
            object_getTypeOfOutlets (o),
            delegate);
    }
    
    if (t.contains (Tag::X)) {
        group.addParameter (Tag::X,
            NEEDS_TRANS ("Position X"),
            NEEDS_TRANS ("Box ordinate"),
            object_getX (o),
            delegate);
    }
    
    if (t.contains (Tag::Y)) {
        group.addParameter (Tag::Y,
            NEEDS_TRANS ("Position Y"),
            NEEDS_TRANS ("Box abscissa"),
            object_getY (o),
            delegate);
    }
    
    if (t.contains (Tag::Selected)) {
        group.addParameter (Tag::Selected,
            NEEDS_TRANS ("Selected"),
            NEEDS_TRANS ("Is selected state"),
            static_cast<bool> (object_isSelected (o)),
            delegate).setHidden (true);
    }
    
    if (t.contains (Tag::Visible)) {
        group.addParameter (Tag::Visible,
            NEEDS_TRANS ("Visible"),
            NEEDS_TRANS ("Is visible state"),
            getVisible (o),
            delegate).setHidden (true);
    }
}

void setObjectAttributesForPatch (data::Group& group, t_object* o, const Tags& t)
{
    static data::DelegateCache delegate;
        
    t_glist* g = cast_glist (o);
    
    if (t.contains (Tag::Title)) {
        group.addParameter (Tag::Title,
            NEEDS_TRANS ("Title"),
            NEEDS_TRANS ("Patch name"),
            makeString (symbol_getName (glist_getName (g))),
            delegate);
    }
    
    if (t.contains (Tag::EditView)) {
        group.addParameter (Tag::EditView,
            NEEDS_TRANS ("Edit View"),
            NEEDS_TRANS ("Edit window geometry"),
            getEditView (g),
            delegate);
    }
    
    if (t.contains (Tag::OffsetX)) {
        group.addParameter (Tag::OffsetX,
            NEEDS_TRANS ("Offset X"),
            NEEDS_TRANS ("Edit window abscissa offset"),
            glist_getOffsetX (g),
            delegate);
    }
    
    if (t.contains (Tag::OffsetY)) {
        group.addParameter (Tag::OffsetY,
            NEEDS_TRANS ("Offset Y"),
            NEEDS_TRANS ("Edit window ordinate offset"),
            glist_getOffsetY (g),
            delegate);
    }
    
    if (t.contains (Tag::Zoom)) {
        group.addParameter (Tag::Zoom,
            NEEDS_TRANS ("Zoom"),
            NEEDS_TRANS ("Edit window zoom"),
            glist_getZoom (g),
            delegate);
    }

    if (t.contains (Tag::Inspector)) {
        group.addParameter (Tag::Inspector,
            NEEDS_TRANS ("Inspector"),
            NEEDS_TRANS ("Edit window has inspector"),
            static_cast<bool> (glist_hasInspector (g)),
            delegate).setHidden (true);
    }
    
    if (t.contains (Tag::InspectorWidth)) {
        group.addParameter (Tag::InspectorWidth,
            NEEDS_TRANS ("Inspector Width"),
            NEEDS_TRANS ("Edit window inspector width"),
            glist_getInspectorWidth (g),
            delegate).setHidden (true);
    }
    
    if (!glist_isRoot (g)) { setObjectAttributesForObject (group, o, t); }
    else {
    //
    if (t.contains (Tag::Path)) {
        group.addParameter (Tag::Path,
            NEEDS_TRANS ("Path"),
            NEEDS_TRANS ("File path"),
            getPatchFile (g).getFullPathName(),
            delegate);
    }
    //
    }
    
    if (t.contains (Tag::Abstraction)) {
        group.addParameter (Tag::Abstraction,
            NEEDS_TRANS ("Abstraction"),
            NEEDS_TRANS ("Is this an abstraction"),
            static_cast<bool> (glist_isAbstraction (g)),
            delegate);
    }
    
    if (t.contains (Tag::Undo)) {
        group.addParameter (Tag::Undo,
            NEEDS_TRANS ("Undo"),
            NEEDS_TRANS ("Undoable action"),
            undomanager_getUndoLabel (glist_getUndoManager (g)),
            delegate).setHidden (true);
    }
    
    if (t.contains (Tag::Redo)) {
        group.addParameter (Tag::Redo,
            NEEDS_TRANS ("Redo"),
            NEEDS_TRANS ("Redoable action"),
            undomanager_getRedoLabel (glist_getUndoManager (g)),
            delegate).setHidden (true);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setObjectAttributes (data::Data& data, t_object* o, const Tags& t)
{
    data::Group group (data.addGroup (Tag::Attributes));
    
    setObjectAttributesClass (group, o, t);
    
    if (object_isCanvas (o)) { setObjectAttributesForPatch (group, o, t); }
    else {
        setObjectAttributesForObject (group, o, t);
    }
    
    for (auto parameter : group) { parameter.setEditable (false); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setObjectParametersIncluded (data::Group& group, t_object* o, const Tags& t)
{
    static data::DelegateCache delegate;
        
    if (t.contains (Tag::Included)) {
        group.addParameter (Tag::Included,
            NEEDS_TRANS ("Included"),
            NEEDS_TRANS ("Is widget included in run view"),
            static_cast<bool> (object_isIncluded (o)),
            delegate);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setObjectParameters (data::Data& data, t_object* o, const Tags& t)
{
    t_class* c = pd_class (o);
    
    if (class_hasParametersFunction (c)) {
    //
    data::Group group (data.addGroup (Tag::Parameters));
    
    (*class_getParametersGetter (c)) (o, group, t);
    
    if (class_canBeIncluded (c)) { setObjectParametersIncluded (group, o, t); }
    
    const bool isAbstraction = glist_isAbstractionOrInside (object_getOwner (o));
    
    for (auto parameter : group) {
        if (isAbstraction) { parameter.setEditable (false, data::ParameterScope::local); }
        else {
            parameter.setEditable (true);
        }
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree getObject (const UniquePath& u, t_object* o, const Tags& t)
{
    juce::ValueTree tree (object_isCanvas (o) ? Id::PATCH : Id::OBJECT);
    
    tree.setProperty (Id::identifier,   data::Cast::toVar (u.getIdentifier()), nullptr);
    tree.setProperty (Id::locked,       data::Cast::toVar (u.isInsideAbstraction()), nullptr);

    if (o) {
    //
    data::Data data (Id::DATA);
    
    if (t.hasAttributes()) { setObjectAttributes (data, o, t); }
    if (t.hasParameters()) { setObjectParameters (data, o, t); }
    
    tree.appendChild (data.asValueTree(), nullptr);
    //
    }
    
    return tree;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Report Report::object (const UniquePath& u, t_object* o, const Tags& t)
{
    return Report (getObject (u, o, t));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void setLineAttributes (data::Data& data, int m, int n, bool b)
{
    static data::DelegateCache delegate;
    
    data::Group group (data.addGroup (Tag::Attributes));
    
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

juce::ValueTree getLine (const UniquePath& u, t_object* src, int m, t_object* dest, int n, bool b)
{
    juce::ValueTree t (Id::LINE);
    
    t.setProperty (Id::identifier,  data::Cast::toVar (u.getIdentifier()), nullptr);
    t.setProperty (Id::locked,      data::Cast::toVar (u.isInsideAbstraction()), nullptr);
    t.setProperty (Id::source,      data::Cast::toVar (object_getUnique (src)), nullptr);
    t.setProperty (Id::destination, data::Cast::toVar (object_getUnique (dest)), nullptr);
    
    data::Data data (Id::DATA); setLineAttributes (data, m, n, b);
    
    t.appendChild (data.asValueTree(), nullptr);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Report Report::lineAdded (const UniquePath& u, t_object* src, int m, t_object* dest, int n)
{
    jassert (!u.isRoot()); return Report (getLine (u, src, m, dest, n, true));
}
Report Report::lineChanged (const UniquePath& u, t_object* src, int m, t_object* dest, int n)
{
    jassert (!u.isRoot()); return Report (getLine (u, src, m, dest, n, false));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
