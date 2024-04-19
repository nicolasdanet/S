
/* Copyright (c) 2021 Spaghettis and others. */

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

juce::ValueTree getParent (const juce::ValueTree& root, const core::UniquePath& u)
{
    juce::ValueTree t (root);
    
    if (u.hasPath()) { for (const auto& i : u.getPath()) { t = Tree::getChild (t, i); } }
    
    return t;
}

bool hasChild (const juce::ValueTree& root, const core::UniquePath& u)
{
    juce::ValueTree parent (getParent (root, u));
    
    if (parent.isValid()) { return Tree::getChild (parent, u.getIdentifier()).isValid(); }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::add (const core::UniquePath& u, const core::Report& v)
{
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (rootTree_, u));
    
    if (v.isPatch()) {
        juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
        if (child.isValid()) {
            core::Patch (child).apply (v); return;      /* Two-step creation for subpatches. */
        }
    }
    
    jassert (Tree::getChild (parent, u.getIdentifier()).isValid() == false);
    
    parent.appendChild (v.asValueTree(), nullptr);
}

void PatchRoot::change (const core::UniquePath& u, const core::Report& v)
{
    juce::ValueTree parent (getParent (rootTree_, u));
    
    if (u.isRoot()) { core::Patch (parent).apply (v); }
    else {
    //
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));

    if (child.isValid()) {
        if (Tree::isLine (child)) { core::Line (child).apply (v); }
        else {
            core::Object (child).apply (v);
        }
    }
    //
    }
}

void PatchRoot::remove (const core::UniquePath& u)
{
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (rootTree_, u));
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
    
    if (child.isValid()) { parent.removeChild (child, nullptr); }
}

void PatchRoot::rename (const core::UniquePath& u, core::UniqueId i)
{
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (rootTree_, u));
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));

    if (child.isValid()) {
        core::Object (child).changeIdentifier (i);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::loadbangBegin (const core::UniquePath& u)
{
    jassert (u.isRoot()); getPresetsManager().loadbangBegin();
}

void PatchRoot::loadbangEnd (const core::UniquePath& u)
{
    jassert (u.isRoot()); getPresetsManager().loadbangEnd();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::setOrder (const core::UniquePath& u, const std::vector<core::UniqueId>& v)
{
    juce::ValueTree parent (getParent (rootTree_, u));
    
    if (u.isRoot()) { core::Patch (parent).sortObjects (v); }
    else {
    //
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
    
    if (child.isValid()) {
        core::Patch (child).sortObjects (v);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PatchRoot::locate (const core::UniquePath& u)
{
    if (contains (u)) {
    //
    const core::UniqueId parent = core::Patch (getParent (rootTree_, u)).getIdentifier();
    const core::UniqueId object = u.getIdentifier();
    
    showEditWindow (parent);
    
    if (auto p = fetchEditWindow (parent)) { return p->locate (object); }
    //
    }
    
    return false;
}

bool PatchRoot::contains (const core::UniquePath& u)
{
    jassert (!u.isRoot()); return hasChild (rootTree_, u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
