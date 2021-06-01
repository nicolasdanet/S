
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

class IconsFactory : public juce::ToolbarItemFactory {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using DrawableContainer = std::vector<std::unique_ptr<juce::Drawable>>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    enum IconsFactoryIds : int {
        add     = 0,
        clear   = 1,
        remove  = 2,
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    IconsFactory()
    {
        addIcon (BinaryData::add_white_24dp_svg,    BinaryData::add_white_24dp_svgSize);
        addIcon (BinaryData::clear_white_24dp_svg,  BinaryData::clear_white_24dp_svgSize);
        addIcon (BinaryData::remove_white_24dp_svg, BinaryData::remove_white_24dp_svgSize);
    }
    
    ~IconsFactory() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    juce::ToolbarItemComponent* createItem (int itemId) override
    {
        switch (itemId) {
        //
        case add    : return createButtonFromBinaryData (itemId, "Add");
        case remove : return createButtonFromBinaryData (itemId, "Remove");
        case clear  : return createButtonFromBinaryData (itemId, "Clear");
        default     : break;
        //
        }

        return nullptr;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    DrawableContainer drawable_;
    
private:
    void addIcon (const void* data, const size_t numBytes)
    {
        drawable_.push_back (juce::Drawable::createFromImageData (data, numBytes));
    }
    
    juce::ToolbarButton* createButtonFromBinaryData (int itemId, const juce::String& text)
    {
        jassert (static_cast<DrawableContainer::size_type> (itemId) < drawable_.size());
        jassert (drawable_[itemId] != nullptr);
        
        return new juce::ToolbarButton (itemId, text, drawable_[itemId]->createCopy(), {});
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconsFactory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

