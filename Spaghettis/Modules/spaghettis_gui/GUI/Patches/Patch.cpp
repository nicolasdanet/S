
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree getChildWithIdentifier (const juce::ValueTree& t, core::Unique::Identifier i)
{
    return t.getChildWithProperty (Ids::identifier, core::Unique::Converter::toVar (i));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::setDirty (bool isDirty)
{
    dirty_ = isDirty; setDirtyFlagIfRequired();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::addObject (const core::Unique& u, const core::Description& v)
{
    juce::ValueTree parent (getParent (u));
    juce::ValueTree object (getChildWithIdentifier (parent, u.getIdentifier()));
    
    if (object.isValid()) {
        object.copyPropertiesAndChildrenFrom (v, nullptr);
    } else {
        parent.appendChild (v, nullptr);
    }
}

void Patch::changeObject (const core::Unique& u, const core::Description& v)
{
    
}

void Patch::removeObject (const core::Unique& u)
{
    juce::ValueTree parent (getParent (u));
    juce::ValueTree object (getChildWithIdentifier (parent, u.getIdentifier()));
    
    if (object.isValid()) {
        parent.removeChild (object, nullptr);
    } else {
        jassertfalse;       /* Is this possible? */
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Patch::getParent (const core::Unique& u) const
{
    juce::ValueTree t (tree_);
    
    if (u.hasPath()) {
        for (const auto& i : u.getPath()) { t = getChildWithIdentifier (t, i); }
    }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::openMainWindow()
{
    mainWindow_ = std::make_unique<EditWindow> (*this, tree_);
}

void Patch::closeWindowButtonPressed (PatchWindow* window)
{
    jassert (mainWindow_.get() == window);
    
    Spaghettis()->getPatches().requestClosePatch (unique_, CloseType::yesNoCancel);
}

void Patch::releaseAllWindows()
{
    mainWindow_ = nullptr;
}

juce::Component* Patch::getMainWindow() const
{
    return dynamic_cast<juce::Component*> (mainWindow_.get());
}

void Patch::setDirtyFlagIfRequired()
{
    if (mainWindow_ && mainWindow_->isRoot()) { mainWindow_->setDirtyFlag (dirty_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
