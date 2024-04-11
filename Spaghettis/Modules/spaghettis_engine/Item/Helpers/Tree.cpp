
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace PresetsConstants
{
    constexpr static const char* const AddressSeparator = "#";
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String getRepresentation (const core::Object& object)
{
    juce::String s (object.get<juce::String> (Tag::Attributes, Tag::Class));
    
    s += PresetsConstants::AddressSeparator;
    s += object.get<int> (Tag::Attributes, Tag::X);
    s += PresetsConstants::AddressSeparator;
    s += object.get<int> (Tag::Attributes, Tag::Y);

    return s;
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
    
    return path.joinIntoString (PresetsConstants::AddressSeparator);
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
