
/* Copyright (c) 2021 Nicolas Danet. */

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

juce::String getContentBuffer (struct _object *o)
{
    juce::String s;
    
    if (o) {
    //
    t_buffer *b = object_getBuffer (o);
    
    if (b && buffer_getSize (b)) {
    //
    char *t = atom_atomsToString (buffer_getSize (b), buffer_getAtoms (b));
    
    s = juce::String (t);
    
    PD_MEMORY_FREE (t);
    //
    }
    //
    }
    
    return s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Description::Description (struct _object *o) : t_ (Ids::OBJECT)
{
    if (o) {
    //
    const bool isPatch = object_isCanvas (o);
    const bool hasView = class_hasViewFunction (pd_class (o));
    
    const juce::String type (isPatch ? "patch" : (hasView ? "graphic" : "box"));
    
    t_.setProperty (Ids::type,      juce::var (type), nullptr);
    t_.setProperty (Ids::name,      juce::var (juce::String (class_getNameAsString (pd_class (o)))), nullptr);
    t_.setProperty (Ids::buffer,    juce::var (getContentBuffer (o)), nullptr);
    t_.setProperty (Ids::inlets,    juce::var (object_getNumberOfInlets (o)), nullptr);
    t_.setProperty (Ids::outlets,   juce::var (object_getNumberOfOutlets (o)), nullptr);
    t_.setProperty (Ids::x,         juce::var (object_getX (o)), nullptr);
    t_.setProperty (Ids::y,         juce::var (object_getY (o)), nullptr);
    t_.setProperty (Ids::width,     juce::var (object_getWidth (o)), nullptr);
    t_.setProperty (Ids::selected,  juce::var (object_getSelected (o)), nullptr);
    
    if (isPatch) {
    //
    t_glist *g = cast_glist (o);
    
    t_.setProperty (Ids::title,     juce::var (glist_getName (g)->s_name), nullptr);
    
    if (glist_isRoot (g)) {
    //
    t_.setProperty (Ids::dirty,     juce::var (glist_isDirty (g)), nullptr);
    //
    }
    //
    }
    
    if (hasView) {
    //
    juce::ValueTree p (Ids::PARAMETERS); (*class_getViewFunction (pd_class (o))) (o, p);
    
    t_.appendChild (p, nullptr);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
