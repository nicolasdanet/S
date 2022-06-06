
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

juce::Grid::TrackInfo getTrack()
{
    return juce::Grid::TrackInfo (juce::Grid::Px (GridLayout::cellSize_));
}

juce::Array<juce::Grid::TrackInfo> getTracks (int n)
{
    juce::Array<juce::Grid::TrackInfo> t;
    
    t.insertMultiple (-1, getTrack(), n);
    
    return t;
}

juce::Array<juce::Grid::TrackInfo> getRows (const juce::Rectangle<int>& bounds)
{
    return getTracks (bounds.getHeight() / GridLayout::cellSpace_);
}

juce::Array<juce::Grid::TrackInfo> getColumns (const juce::Rectangle<int>& bounds)
{
    return getTracks (bounds.getWidth() / GridLayout::cellSpace_);
}

juce::Array<juce::GridItem> getGridItems (const GridLayout::LayoutContainer& viewed)
{
    juce::Array<juce::GridItem> items;
    
    for (const auto& [o, bounds] : viewed) {
    //
    const juce::GridItem::Span wSpan (static_cast<int> (bounds.getWidth()  / GridLayout::cellSpace_) + 1);
    const juce::GridItem::Span hSpan (static_cast<int> (bounds.getHeight() / GridLayout::cellSpace_) + 1);

    o->setVisible (true);
    
    items.add (juce::GridItem (o).withArea (hSpan, wSpan));
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
    juce::Grid grid;
    
    grid.justifyItems       = juce::Grid::JustifyItems::stretch;
    grid.alignItems         = juce::Grid::AlignItems::stretch;
    grid.justifyContent     = juce::Grid::JustifyContent::start;
    grid.alignContent       = juce::Grid::AlignContent::start;
    grid.autoFlow           = juce::Grid::AutoFlow::rowDense;
    grid.templateRows       = getRows (bounds);
    grid.templateColumns    = getColumns (bounds);
    grid.autoColumns        = getTrack();
    grid.autoRows           = getTrack();
    grid.rowGap             = juce::Grid::Px (cellGap_);
    grid.columnGap          = juce::Grid::Px (cellGap_);
    grid.items              = getGridItems (viewed_);
    
    grid.performLayout (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
