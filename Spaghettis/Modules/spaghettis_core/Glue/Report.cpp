
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

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

juce::Rectangle<int> getWindow (t_glist* glist)
{
    const int x = rectangle_getTopLeftX (glist_getWindow (glist));
    const int y = rectangle_getTopLeftY (glist_getWindow (glist));
    const int w = rectangle_getWidth (glist_getWindow (glist));
    const int h = rectangle_getHeight (glist_getWindow (glist));

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

void setAttributesObject (Group& group, t_object* o)
{
    static DelegateCache delegate;
    
    group.addParameter (Tags::Buffer,
        NEEDS_TRANS ("Buffer"),
        NEEDS_TRANS ("Content of the box"),
        getContentBuffer (o),
        delegate);
    
    group.addParameter (Tags::Inlets,
        NEEDS_TRANS ("Inlets"),
        NEEDS_TRANS ("Number of inlets"),
        object_getNumberOfInlets (o),
        delegate);
    
    group.addParameter (Tags::Outlets,
        NEEDS_TRANS ("Outlets"),
        NEEDS_TRANS ("Number of outlets"),
        object_getNumberOfOutlets (o),
        delegate);
        
    group.addParameter (Tags::X,
        NEEDS_TRANS ("Position X"),
        NEEDS_TRANS ("Box ordinate"),
        object_getX (o),
        delegate);
    
    group.addParameter (Tags::Y,
        NEEDS_TRANS ("Position Y"),
        NEEDS_TRANS ("Box abscissa"),
        object_getY (o),
        delegate);
    
    group.addParameter (Tags::Width,
        NEEDS_TRANS ("Width"),
        NEEDS_TRANS ("Box width in characters"),
        object_getWidth (o),
        delegate);
    
    group.addParameter (Tags::Selected,
        NEEDS_TRANS ("Selected"),
        NEEDS_TRANS ("Selected state"),
        static_cast<bool> (object_getSelected (o)),
        delegate);
    
    group.addParameter (Tags::Visible,
        NEEDS_TRANS ("Visible"),
        NEEDS_TRANS ("Is visible state"),
        getVisible (o),
        delegate);
        
    group.addParameter (Tags::Class,
        NEEDS_TRANS ("Class"),
        NEEDS_TRANS ("Class of the object"),
        juce::String (class_getNameAsString (pd_class (o))),
        delegate);
}

void setAttributesPatch (Group& group, t_object* o)
{
    static DelegateCache delegate;
        
    t_glist* g = cast_glist (o);
    
    group.addParameter (Tags::Title,
        NEEDS_TRANS ("Title"),
        NEEDS_TRANS ("Patch name"),
        juce::String (glist_getName (g)->s_name),
        delegate);
    
    group.addParameter (Tags::EditView,
        NEEDS_TRANS ("Edit View"),
        NEEDS_TRANS ("Edit window geometry"),
        getWindow (g),
        delegate);
    
    if (!glist_isRoot (g)) { setAttributesObject (group, o); }
    else {
    //
    group.addParameter (Tags::RunView,
        NEEDS_TRANS ("Run View"),
        NEEDS_TRANS ("Run window geometry"),
        getWindow (g),
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

void setAttributes (Data& data, t_object* o)
{
    Group group (data.addGroup (Tags::Attributes, true));
    
    if (object_isCanvas (o)) { setAttributesPatch (group, o); }
    else {
        setAttributesObject (group, o);
    }
}

void setParameters (Data& data, t_object* o)
{
    t_class* c = pd_class (o);
    
    if (class_hasParametersFunction (c)) {
    //
    Group group (data.addGroup (Tags::Parameters)); (*class_getParametersFunction (c)) (o, group);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree getReport (const UniquePath& u, struct _object* o, bool attributes, bool parameters)
{
    juce::ValueTree t (Ids::OBJECT);
    
    t.setProperty (Ids::identifier, cast::toVar (u.getIdentifier()), nullptr);
    
    if (o) {
    //
    Data data (Ids::DATA);
    
    if (attributes) { setAttributes (data, o); }
    if (parameters) { setParameters (data, o); }
    
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
    return Report (getReport (u, o, true,  true));
}

Report Report::parameters (const UniquePath& u, struct _object* o)
{
    return Report (getReport (u, o, false, true));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
