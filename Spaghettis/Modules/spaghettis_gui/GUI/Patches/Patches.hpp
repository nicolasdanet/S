
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Patches {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Patches()  = default;
    ~Patches() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void addObject (const core::Unique& u, const core::Description& v)
    {
        jassert (u.isValid());

        if (u.isRoot()) { createPatch (u, v); }
        else {
            perform (u, [&] (const std::shared_ptr<Patch>& p) { p->addObject (u, v); });
        }
    }

    void removeObject (const core::Unique& u)
    {
        jassert (u.isValid());

        if (u.isRoot()) { closePatch (u, false); }
        else {
            perform (u, [&] (const std::shared_ptr<Patch>& p) { p->removeObject (u); });
        }
    }

    void renameObject (const core::Unique& u, core::Unique::Identifier i)
    {
        jassert (u.isValid());
    }

    void changeObject (const core::Unique& u, const core::Description& v)
    {
        jassert (u.isValid());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setDirty (const core::Unique& u, bool isDirty)
    {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->setDirty (isDirty); });
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void closeAllPatches();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void showSaveRequest (const std::shared_ptr<Patch>& p);
    
public:
    void handleSaveRequest (const core::Unique& u, bool save);
    
    bool isAllSaveRequestsDone()
    {
        return requests_.empty();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void createPatch (const core::Unique& u, const core::Description& v);
    void closePatch (const core::Unique& u, bool notify = true);
    
private:
    std::shared_ptr<Patch> fetchPatch (const core::Unique& u) const;
    void removePatch (const core::Unique& u);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static auto hasEqualRoot (const core::Unique& u)
    {
        return [i = u.getRoot()] (const std::shared_ptr<Patch>& p)
        {
            return (p->getUnique().getIdentifier() == i);
        };
    }
    
    static auto toUnique()
    {
        return [] (const std::shared_ptr<Patch>& p)
        {
            return p->getUnique();
        };
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    template <class T> void perform (const core::Unique& u, T f) const
    {
        auto r = std::find_if (roots_.cbegin(), roots_.cend(), hasEqualRoot (u));
    
        if (r != roots_.cend()) { f (*r); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    std::vector<std::shared_ptr<Patch>> roots_;
    std::vector<std::shared_ptr<Patch>> requests_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Patches)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

