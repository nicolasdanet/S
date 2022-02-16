
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

juce::String getWindow (t_glist* glist)
{
    const int x = rectangle_getTopLeftX (glist_getWindow (glist));
    const int y = rectangle_getTopLeftY (glist_getWindow (glist));
    const int w = rectangle_getWidth (glist_getWindow (glist));
    const int h = rectangle_getHeight (glist_getWindow (glist));

    return juce::Rectangle<int> (x, y, w, h).toString();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setAttributesForObject (Group& group, const juce::String& type, t_object* o)
{
    static Delegate::Cache delegate;
    
    group.addParameter (Tags::Type,
        NEEDS_TRANS ("Type"),
        NEEDS_TRANS ("Object type"),
        juce::String (type),
        delegate);
    
    group.addParameter (Tags::Name,
        NEEDS_TRANS ("Name"),
        NEEDS_TRANS ("Class name"),
        juce::String (class_getNameAsString (pd_class (o))),
        delegate);
    
    group.addParameter (Tags::Buffer,
        NEEDS_TRANS ("Buffer"),
        NEEDS_TRANS ("Content of the box"),
        juce::String (getContentBuffer (o)),
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
        NEEDS_TRANS ("X"),
        NEEDS_TRANS ("Abscissa of position"),
        object_getX (o),
        delegate);
    
    group.addParameter (Tags::Y,
        NEEDS_TRANS ("Y"),
        NEEDS_TRANS ("Ordinate of position"),
        object_getY (o),
        delegate);
    
    group.addParameter (Tags::Width,
        NEEDS_TRANS ("Width"),
        NEEDS_TRANS ("Width in characters"),
        object_getWidth (o),
        delegate);
    
    group.addParameter (Tags::Selected,
        NEEDS_TRANS ("Selected"),
        NEEDS_TRANS ("Selected state"),
        static_cast<bool> (object_getSelected (o)),
        delegate);
}

void setAttributesForPatch (Group& group, t_glist* g)
{
    static Delegate::Cache delegate;
    
    group.addParameter (Tags::Title,
        NEEDS_TRANS ("Title"),
        NEEDS_TRANS ("Patch name"),
        juce::String (glist_getName (g)->s_name),
        delegate);
    
    group.addParameter (Tags::EditView,
        NEEDS_TRANS ("Edit View"),
        NEEDS_TRANS ("Edit window geometry"),
        juce::String (getWindow (g)),
        delegate);
    
    if (glist_isRoot (g)) {
    //
    group.addParameter (Tags::RunView,
        NEEDS_TRANS ("Run View"),
        NEEDS_TRANS ("Run window geometry"),
        juce::String (getWindow (g)),
        delegate);
    
    group.addParameter (Tags::Path,
        NEEDS_TRANS ("Path"),
        NEEDS_TRANS ("File path"),
        juce::String (getPatchFile (g).getFullPathName()),
        delegate);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void setAttributes (Tree& tree, t_object* o)
{
    const bool isPatch = object_isCanvas (o);
    const bool hasView = class_hasViewFunction (pd_class (o));
    
    Group group (tree.addGroup (Tags::Attributes, true));
    
    setAttributesForObject (group, (isPatch ? "patch" : (hasView ? "graphic" : "box")), o);
    
    if (isPatch) { setAttributesForPatch (group, cast_glist (o)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void setParameters (Tree& tree, t_object* o)
{
    Group group (tree.addGroup (Tags::Parameters));

    if (class_hasViewFunction (pd_class (o))) { (*class_getViewFunction (pd_class (o))) (o, group); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Description Description::view (const Unique& u, struct _object* o)
{
    juce::ValueTree t (Ids::OBJECT);
    
    t.setProperty (Ids::identifier, core::Unique::Converter::toVar (u.getIdentifier()), nullptr);
    
    if (o) {
    //
    Tree tree (Ids::DATA);
    
    setAttributes (tree, o);
    setParameters (tree, o);
    
    t.appendChild (tree, nullptr);
    //
    }
    
    return Description (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::var Description::getAttribute (const juce::ValueTree& t, const juce::String &name)
{
    return Tree (t.getChildWithName (Ids::DATA)).getValue (Tags::Attributes, name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
