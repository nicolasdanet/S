
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

juce::String getContentBuffer (struct _object* o)
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

juce::ValueTree getAttributes (struct _object* o)
{
    juce::ValueTree a (Ids::ATTRIBUTES);
    
    const bool isPatch = object_isCanvas (o);
    const bool hasView = class_hasViewFunction (pd_class (o));
    
    const juce::String type (isPatch ? "patch" : (hasView ? "graphic" : "box"));
    
    a.setProperty (Ids::type,       juce::var (type), nullptr);
    a.setProperty (Ids::name,       juce::var (juce::String (class_getNameAsString (pd_class (o)))), nullptr);
    a.setProperty (Ids::buffer,     juce::var (getContentBuffer (o)), nullptr);
    a.setProperty (Ids::inlets,     juce::var (object_getNumberOfInlets (o)), nullptr);
    a.setProperty (Ids::outlets,    juce::var (object_getNumberOfOutlets (o)), nullptr);
    a.setProperty (Ids::x,          juce::var (object_getX (o)), nullptr);
    a.setProperty (Ids::y,          juce::var (object_getY (o)), nullptr);
    a.setProperty (Ids::width,      juce::var (object_getWidth (o)), nullptr);
    a.setProperty (Ids::selected,   juce::var (object_getSelected (o)), nullptr);
    
    if (isPatch) {
    //
    t_glist* g = cast_glist (o);
        
    a.setProperty (Ids::title,      juce::var (glist_getName (g)->s_name), nullptr);
    a.setProperty (Ids::window,     juce::var (getWindow (g)), nullptr);
    
    if (glist_isRoot (g)) {
        a.setProperty (Ids::path,   juce::var (getPatchFile (g).getFullPathName()), nullptr);
    }
    //
    }
    
    return a;
}

Tree getParameters (struct _object* o)
{
    Tree p (Ids::PARAMETERS);
    
    if (class_hasViewFunction (pd_class (o))) { (*class_getViewFunction (pd_class (o))) (o, p); }
    
    return p;
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
    t.appendChild (getAttributes (o), nullptr);
    t.appendChild (getParameters (o), nullptr);
    //
    }
    
    return Description (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const juce::var& Description::getAttribute (const juce::ValueTree& t, const juce::Identifier &name)
{
    return t.getChildWithName (Ids::ATTRIBUTES).getProperty (name);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
