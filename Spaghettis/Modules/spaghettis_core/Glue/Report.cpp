
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
    const int x = rectangle_getTopLeftX (glist_getEditView (glist));
    const int y = rectangle_getTopLeftY (glist_getEditView (glist));
    const int w = rectangle_getWidth (glist_getEditView (glist));
    const int h = rectangle_getHeight (glist_getEditView (glist));

    return juce::Rectangle<int> (x, y, w, h);
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
    
    group.addParameter (Tags::Class,
        NEEDS_TRANS ("Class"),
        NEEDS_TRANS ("Class of the object"),
        juce::String (class_getNameAsString (pd_class (o))),
        delegate);
        
    group.addParameter (Tags::Buffer,
        NEEDS_TRANS ("Buffer"),
        NEEDS_TRANS ("Content of the box"),
        getContentBuffer (o),
        delegate);
    
    group.addParameter (Tags::Inlets,
        NEEDS_TRANS ("Inlets"),
        NEEDS_TRANS ("List of inlets"),
        object_getTypeOfInlets (o),
        delegate);
    
    group.addParameter (Tags::Outlets,
        NEEDS_TRANS ("Outlets"),
        NEEDS_TRANS ("List of outlets"),
        object_getTypeOfOutlets (o),
        delegate);
        
    group.addParameter (Tags::X,
        NEEDS_TRANS ("Position X"),
        NEEDS_TRANS ("Box ordinate"),
        Canvas::addOffset (object_getX (o)),
        delegate);
    
    group.addParameter (Tags::Y,
        NEEDS_TRANS ("Position Y"),
        NEEDS_TRANS ("Box abscissa"),
        Canvas::addOffset (object_getY (o)),
        delegate);
    
    group.addParameter (Tags::Selected,
        NEEDS_TRANS ("Selected"),
        NEEDS_TRANS ("Is selected state"),
        static_cast<bool> (object_isSelected (o)),
        delegate);
    
    group.addParameter (Tags::Visible,
        NEEDS_TRANS ("Visible"),
        NEEDS_TRANS ("Is visible state"),
        getVisible (o),
        delegate);
}

void setObjectAttributesForPatch (Group& group, t_object* o)
{
    static DelegateCache delegate;
        
    t_glist* g = cast_glist (o);
    
    group.addParameter (Tags::Title,
        NEEDS_TRANS ("Title"),
        NEEDS_TRANS ("Patch name"),
        juce::String (symbol_getName (glist_getName (g))),
        delegate);
    
    group.addParameter (Tags::EditView,
        NEEDS_TRANS ("Edit View"),
        NEEDS_TRANS ("Edit window geometry"),
        getEditView (g),
        delegate);
    
    if (!glist_isRoot (g)) { setObjectAttributesForObject (group, o); }
    else {
    //
    group.addParameter (Tags::RunView,
        NEEDS_TRANS ("Run View"),
        NEEDS_TRANS ("Run window geometry"),
        getEditView (g),
        delegate);
    
    group.addParameter (Tags::Path,
        NEEDS_TRANS ("Path"),
        NEEDS_TRANS ("File path"),
        getPatchFile (g).getFullPathName(),
        delegate);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setObjectAttributes (Data& data, t_object* o)
{
    Group group (data.addGroup (Tags::Attributes, true));
    
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
    
    Group group (data.addGroup (Tags::Parameters));
    
    group.addParameter (Tags::Label,
        NEEDS_TRANS ("Label"),
        NEEDS_TRANS ("Parameter name in run view"),
        juce::String (symbol_getName (object_getLabel (o))),
        delegate);
    
    (*class_getParametersFunction (c)) (o, group);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree getObject (const UniquePath& u, struct _object* o, bool attributes, bool parameters)
{
    juce::ValueTree t (object_isCanvas (o) ? Ids::PATCH : Ids::OBJECT);
    
    t.setProperty (Ids::identifier, Cast::toVar (u.getIdentifier()), nullptr);
    
    if (o) {
    //
    Data data (Ids::DATA);
    
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

juce::ValueTree getLine (const UniquePath& u, struct _object* src, int m, struct _object* dest, int n)
{
    juce::ValueTree t (Ids::LINE);
    
    t.setProperty (Ids::identifier,     Cast::toVar (u.getIdentifier()), nullptr);
    t.setProperty (Ids::source,         Cast::toVar (object_getUnique (src)), nullptr);
    t.setProperty (Ids::outlet,         m, nullptr);
    t.setProperty (Ids::destination,    Cast::toVar (object_getUnique (dest)), nullptr);
    t.setProperty (Ids::inlet,          n, nullptr);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Report Report::line (const UniquePath& u, struct _object* src, int m, struct _object* dest, int n)
{
    jassert (!u.isRoot()); return Report (getLine (u, src, m, dest, n));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
