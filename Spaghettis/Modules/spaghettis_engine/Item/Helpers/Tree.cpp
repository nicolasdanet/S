
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String getRepresentation (const core::Object& object)
{
    juce::String s (object.get<juce::String> (Tag::Attributes, Tag::Class));
    
    s += object.get<int> (Tag::Attributes, Tag::X);
    s += object.get<int> (Tag::Attributes, Tag::Y);

    return s;
}

juce::String getPathAsHexString (juce::String path)
{
    return juce::String::toHexString (path.toRawUTF8(), static_cast<int> (path.getNumBytesAsUTF8()), 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String Tree::computePath (const juce::ValueTree& tree)
{
    juce::StringArray path;
    
    path.add (getRepresentation (core::Object (tree)));
    
        juce::ValueTree child (tree);
        juce::ValueTree parent (child.getParent());
        
        while (parent.isValid()) {
        //
        core::Patch patch (parent);
        
        path.insert (0, juce::String (patch.getIndexOfObject (core::Object (child))));
        
        child  = parent;
        parent = parent.getParent();
        //
        }
    
    return getPathAsHexString (path.joinIntoString ("/"));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Tree::getItemIfChangedPropertyEquals (const juce::ValueTree& t, juce::String key)
{
    if (t.hasType (Id::PARAMETER) && (data::Parameter (t).getKey() == key)) {
    //
    const juce::ValueTree root (t.getParent().getParent().getParent());
    
    jassert (Item (root).isObject());
    
    return root;
    //
    }
    
    return juce::ValueTree();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
