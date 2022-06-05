
/* Copyright (c) 2022 Jojo and others. */

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

auto isObject (ObjectComponent* o)
{
    return [o](const GridLayout::LayoutElement& e)
    {
        return (std::get<GridLayout::LAYOUT_POINTER> (e) == o);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void GridLayout::add (ObjectComponent* o, const juce::Rectangle<int>& bounds)
{
    auto r = std::find_if (viewed_.begin(), viewed_.end(), isObject (o));
    
    if (r != viewed_.end()) { std::get<GridLayout::LAYOUT_BOUNDS> (*r) = bounds.withZeroOrigin(); }
    else {
        viewed_.emplace_back (o, bounds.withZeroOrigin());
    }
}

void GridLayout::remove (ObjectComponent* o)
{
    o->setVisible (false);
    
    viewed_.erase (std::remove_if (viewed_.begin(), viewed_.end(), isObject (o)), viewed_.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Array<juce::GridItem> getGridItems (const GridLayout::LayoutContainer& viewed)
{
    using Item = juce::GridItem;
    
    juce::Array<juce::GridItem> items;
    
    for (const auto& [o, bounds] : viewed) {
    //
    const int w = bounds.getWidth();
    const int h = bounds.getHeight();
    const int wSpan = static_cast<int> (w / GridLayout::cellSize_) + 1;
    const int hSpan = static_cast<int> (h / GridLayout::cellSize_) + 1;

    items.add (Item (o).withArea (Item::Span (wSpan), Item::Span (hSpan)).withSize (w, h));
    
    o->setVisible (true);
    //
    }
    
    return items;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void GridLayout::arrange (const juce::Rectangle<int>& bounds)
{
    using Grid  = juce::Grid;
    using Track = juce::Grid::TrackInfo;
    
    Grid grid;
    
    grid.justifyItems       = Grid::JustifyItems::center;
    grid.alignItems         = Grid::AlignItems::center;
    grid.justifyContent     = Grid::JustifyContent::start;
    grid.alignContent       = Grid::AlignContent::start;
    grid.autoFlow           = Grid::AutoFlow::columnDense;
    grid.templateRows       = { Track (Grid::Px (cellSize_)) };
    grid.templateColumns    = { Track (Grid::Px (cellSize_)) };
    grid.autoColumns        = Track (Grid::Px (cellSize_));
    grid.autoRows           = Track (Grid::Px (cellSize_));
    grid.rowGap             = Grid::Px (1);
    grid.columnGap          = Grid::Px (1);
    grid.items              = getGridItems (viewed_);
    
    grid.performLayout (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
