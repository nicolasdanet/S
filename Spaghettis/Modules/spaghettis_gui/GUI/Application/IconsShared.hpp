
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
    sync,
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
        addIcon (BinaryData::add_black_24dp_svg,                BinaryData::add_black_24dp_svgSize);
        addIcon (BinaryData::delete_forever_black_24dp_svg,     BinaryData::delete_forever_black_24dp_svgSize);
        addIcon (BinaryData::error_outline_black_24dp_svg,      BinaryData::error_outline_black_24dp_svgSize);
        addIcon (BinaryData::notifications_black_24dp_svg,      BinaryData::notifications_black_24dp_svgSize);
        addIcon (BinaryData::place_black_24dp_svg,              BinaryData::place_black_24dp_svgSize);
        addIcon (BinaryData::remove_black_24dp_svg,             BinaryData::remove_black_24dp_svgSize);
        addIcon (BinaryData::sync_black_24dp_svg,               BinaryData::sync_black_24dp_svgSize);
        addIcon (BinaryData::text_rotate_up_black_24dp_svg,     BinaryData::text_rotate_up_black_24dp_svgSize);
        addIcon (BinaryData::text_rotation_down_black_24dp_svg, BinaryData::text_rotation_down_black_24dp_svgSize);
    }
 
    std::unique_ptr<juce::Drawable> getIcon (int itemId) const
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < drawable_.size());
        jassert (drawable_[i] != nullptr);
        
        return drawable_[i]->createCopy();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addIcon (const void* data, const size_t numBytes)
    {
        std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, numBytes));

        if (!t->replaceColour (juce::Colours::black, Spaghettis()->getColour (Colours::toolbarIcon))) {
            jassertfalse;
        }
        
        drawable_.push_back (std::move (t));
    }
    
private:
    DrawableContainer drawable_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

