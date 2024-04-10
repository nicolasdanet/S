
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
            juce::String path (Tree::computePath (child));
            if (absoluteHasKey) { paths.set (path, item); }
            else {
                paths.set (item, path);
            }
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

juce::StringPairArray PresetsAddresses::get (const juce::ValueTree& root, bool absoluteHasKey)
{
    juce::StringPairArray paths;
    
    fetchPathsRecursive (root, paths, absoluteHasKey);
    
    return paths;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
