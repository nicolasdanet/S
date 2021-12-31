
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
    const bool isPatch = object_isCanvas (o);
    const bool hasView = class_hasViewFunction (pd_class (o));
    
    const juce::String type (isPatch ? "patch" : (hasView ? "graphic" : "box"));
    
    t.setProperty (Ids::type,       juce::var (type), nullptr);
    t.setProperty (Ids::name,       juce::var (juce::String (class_getNameAsString (pd_class (o)))), nullptr);
    t.setProperty (Ids::buffer,     juce::var (getContentBuffer (o)), nullptr);
    t.setProperty (Ids::inlets,     juce::var (object_getNumberOfInlets (o)), nullptr);
    t.setProperty (Ids::outlets,    juce::var (object_getNumberOfOutlets (o)), nullptr);
    t.setProperty (Ids::x,          juce::var (object_getX (o)), nullptr);
    t.setProperty (Ids::y,          juce::var (object_getY (o)), nullptr);
    t.setProperty (Ids::width,      juce::var (object_getWidth (o)), nullptr);
    t.setProperty (Ids::selected,   juce::var (object_getSelected (o)), nullptr);
    
    if (isPatch) {
    
        juce::ValueTree p (Ids::PARAMETERS);
        
        t_glist* g = cast_glist (o);
        
        p.setProperty (Ids::title, juce::var (glist_getName (g)->s_name), nullptr);
        
        const int x = rectangle_getTopLeftX (glist_getWindow (g));
        const int y = rectangle_getTopLeftY (glist_getWindow (g));
        const int w = rectangle_getWidth (glist_getWindow (g));
        const int h = rectangle_getHeight (glist_getWindow (g));
    
        p.setProperty (Ids::x,      juce::var (x), nullptr);
        p.setProperty (Ids::y,      juce::var (y), nullptr);
        p.setProperty (Ids::width,  juce::var (w), nullptr);
        p.setProperty (Ids::height, juce::var (h), nullptr);
        
        if (glist_isRoot (g)) {
            p.setProperty (Ids::path, juce::var (getPatchFile (g).getFullPathName()), nullptr);
        }
        
        t.appendChild (p, nullptr);
    }
    
    if (hasView) {
    
        juce::ValueTree p (Ids::PARAMETERS);
        
        (*class_getViewFunction (pd_class (o))) (o, p);
        
        t.appendChild (p, nullptr);
    }
    //
    }
    
    return Description (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
