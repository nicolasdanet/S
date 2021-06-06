
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "BinaryData.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Icons {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum IconsIds : int {
    add                 = 1,
    clear,
    error,
    notification,
    place,
    remove,
    synchronize,
    sortUp,
    sortDown
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct IconsShared {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using DrawableContainer = std::vector<std::unique_ptr<juce::Drawable>>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    IconsShared()
    {
        addIcon (BinaryData::add_svg,                   BinaryData::add_svgSize);
        addIcon (BinaryData::delete_forever_svg,        BinaryData::delete_forever_svgSize);
        addIcon (BinaryData::warning_amber_svg,         BinaryData::warning_amber_svgSize);
        addIcon (BinaryData::notifications_svg,         BinaryData::notifications_svgSize);
        addIcon (BinaryData::place_svg,                 BinaryData::place_svgSize);
        addIcon (BinaryData::remove_svg,                BinaryData::remove_svgSize);
        addIcon (BinaryData::sync_svg,                  BinaryData::sync_svgSize);
        addIcon (BinaryData::text_rotate_up_svg,        BinaryData::text_rotate_up_svgSize);
        addIcon (BinaryData::text_rotation_down_svg,    BinaryData::text_rotation_down_svgSize);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const
    {
        return getIconRaw (itemId, off_);
    }

    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const
    {
        return getIconRaw (itemId, on_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    std::unique_ptr<juce::Drawable> getIconRaw (int itemId, const DrawableContainer& c) const
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < c.size());
        jassert (c[i] != nullptr);
        
        return c[i]->createCopy();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addIconRaw (const void* data, const size_t n, juce::Colour colour, DrawableContainer& c)
    {
        std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, n));

        t->replaceColour (juce::Colours::black, colour);
        
        c.push_back (std::move (t));
    }
    
    void addIcon (const void* data, const size_t n)
    {
        addIconRaw (data, n, Spaghettis()->getColour (Colours::toolbarIcon), off_);
        addIconRaw (data, n, Spaghettis()->getColour (Colours::toolbarIcon), on_);
    }

private:
    DrawableContainer on_;
    DrawableContainer off_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

