
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void addIfIncluded (const juce::ValueTree& child, juce::StringPairArray& paths, bool absoluteAsKey)
{
    if (Tree::isObject (child)) {
        const core::Object object (child);
        if (object.isGraphic() && object.get<bool> (Tag::Parameters, Tag::Included)) {
            juce::String item (data::Cast::toVar<core::UniqueId> (object.getIdentifier()).toString());
            juce::String path (Tree::computePath (child));
            if (absoluteAsKey) { paths.set (path, item); }
            else {
                paths.set (item, path);
            }
        }
    }
}
    
void fetchPathsRecursive (const juce::ValueTree& tree, juce::StringPairArray& paths, bool absoluteAsKey)
{
    for (const auto& child : tree) {
        addIfIncluded (child, paths, absoluteAsKey);
        fetchPathsRecursive (child, paths, absoluteAsKey);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::StringPairArray PresetsAddresses::get (const juce::ValueTree& root, bool absoluteAsKey)
{
    juce::StringPairArray paths;
    
    fetchPathsRecursive (root, paths, absoluteAsKey);
    
    return paths;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
