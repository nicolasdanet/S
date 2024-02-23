
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PlacementsElement {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PlacementsElement (core::UniqueId u, core::Point::Real offset, int zoom, int width) :
        u_ (u),
        offset_ (offset),
        zoom_ (zoom),
        width_ (width)
    {
        static_assert (std::is_trivially_copyable_v<PlacementsElement> == true);
        static_assert (std::is_nothrow_move_constructible_v<PlacementsElement> == true);
        static_assert (std::is_nothrow_move_assignable_v<PlacementsElement> == true);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    core::UniqueId getUnique() const
    {
        return u_;
    }
    
    core::Point::Real getOffset() const
    {
        return offset_;
    }
    
    int getZoom() const
    {
        return zoom_;
    }
    
    int getInspectorWidth() const
    {
        return width_;
    }
    
    bool hasInspector() const
    {
        return getInspectorWidth() > 0;
    }
    
private:
    core::UniqueId u_;
    core::Point::Real offset_;
    int zoom_;
    int width_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

