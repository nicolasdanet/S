
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
    struct Icons {
        enum IconsIds : int {
            add             = 1,
            remove          = 2,
            clear           = 3
        };
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    IconsFactory()
    {
        addIcon (BinaryData::add_black_24dp_svg,    BinaryData::add_black_24dp_svgSize);
        addIcon (BinaryData::remove_black_24dp_svg, BinaryData::remove_black_24dp_svgSize);
        addIcon (BinaryData::clear_black_24dp_svg,  BinaryData::clear_black_24dp_svgSize);
    }
    
    ~IconsFactory() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    juce::ToolbarItemComponent* createItem (int itemId) override
    {
        switch (itemId) {
        //
        case Icons::add     : return createButton (itemId, "Add");
        case Icons::remove  : return createButton (itemId, "Remove");
        case Icons::clear   : return createButton (itemId, "Clear");
        default             : break;
        //
        }

        return nullptr;
    }
    
    virtual void setCallback (int itemId, juce::ToolbarButton* button) = 0;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addIcon (const void* data, const size_t numBytes)
    {
        std::unique_ptr<juce::Drawable> t (juce::Drawable::createFromImageData (data, numBytes));
        
        bool done = t->replaceColour (juce::Colours::black, Spaghettis()->getColour (Colours::toolbarIcon));
        
        jassert (done);
        
        drawable_.push_back (std::move (t));
    }
    
    juce::ToolbarButton* createButton (int itemId, const juce::String& text)
    {
        int i = itemId - 1;
        
        jassert (i >= 0);
        jassert (static_cast<DrawableContainer::size_type> (i) < drawable_.size());
        jassert (drawable_[i] != nullptr);
        
        auto t = std::make_unique<juce::ToolbarButton> (itemId,
                    text,
                    drawable_[i]->createCopy(),
                    nullptr);
        
        if (t) { setCallback (itemId, t.get()); }
        else {
            jassertfalse;
        }
        
        return t.release();
    }

private:
    DrawableContainer drawable_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconsFactory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

