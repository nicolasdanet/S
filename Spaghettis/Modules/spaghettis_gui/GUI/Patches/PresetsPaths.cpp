
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void addIfIncluded (const juce::ValueTree& child, juce::StringPairArray& paths, bool absoluteHasKey)
{
    if (Tree::isObject (child)) {
        const core::Object object (child);
        if (object.isGraphic() && object.get<bool> (Tag::Parameters, Tag::Included)) {
            juce::String item (data::Cast::toVar<core::UniqueId> (object.getIdentifier()).toString());
            DBG (item);
        }
    }
}
    
void fetchPathsRecursive (const juce::ValueTree& tree, juce::StringPairArray& paths, bool absoluteHasKey)
{
    for (const auto& child : tree) {
        addIfIncluded (child, paths, absoluteHasKey);
        fetchPathsRecursive (child, paths, absoluteHasKey);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PresetsPaths::initialize (const juce::ValueTree& root, juce::StringPairArray& paths, bool absoluteHasKey)
{
    fetchPathsRecursive (root, paths, absoluteHasKey);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

AbsoluteToLocal::AbsoluteToLocal (const juce::ValueTree& root)
{
    PresetsPaths::initialize (root, paths_, true);
}

juce::String AbsoluteToLocal::getItemWithPath (const juce::String&) const
{
    return juce::String ("");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LocalToAbsolute::LocalToAbsolute (const juce::ValueTree& root)
{
    PresetsPaths::initialize (root, paths_, false);
}

juce::String LocalToAbsolute::getPathWithItem (const juce::String&) const
{
    return juce::String ("");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
