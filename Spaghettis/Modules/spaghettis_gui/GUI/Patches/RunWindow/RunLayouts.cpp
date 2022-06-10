
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

juce::Grid::TrackInfo getRowTrack()
{
    return juce::Grid::TrackInfo (juce::Grid::Px (GridLayout::height_));
}

juce::Grid::TrackInfo getColumnTrack()
{
    return juce::Grid::TrackInfo (juce::Grid::Px (GridLayout::width_));
}

juce::Array<juce::Grid::TrackInfo> getTracks (int n, const juce::Grid::TrackInfo& track)
{
    juce::Array<juce::Grid::TrackInfo> t;
    
    t.insertMultiple (-1, track, n);
    
    return t;
}

juce::Array<juce::Grid::TrackInfo> getRows (const juce::Rectangle<int>& bounds)
{
    const int n = bounds.getHeight() / (GridLayout::gap_ + GridLayout::height_);
    
    return getTracks (n, getRowTrack());
}

juce::Array<juce::Grid::TrackInfo> getColumns (const juce::Rectangle<int>& bounds)
{
    const int n = bounds.getWidth() / (GridLayout::gap_ + GridLayout::width_);
    
    return getTracks (n, getColumnTrack());
}

juce::Array<juce::GridItem> getGridItems (const GridLayout::LayoutContainer& viewed)
{
    juce::Array<juce::GridItem> items;
    
    for (const auto& [o, bounds] : viewed) {
    //
    const int w = bounds.getWidth();
    const int h = bounds.getHeight();
    const int wSpace = GridLayout::gap_ + GridLayout::width_;
    const int hSpace = GridLayout::gap_ + GridLayout::height_;
    const juce::GridItem::Span wSpan (static_cast<int> (w / wSpace) + 1);
    const juce::GridItem::Span hSpan (static_cast<int> (h / hSpace) + 1);

    o->setVisible (true);
    
    items.add (juce::GridItem (o).withArea (hSpan, wSpan).withSize (w, h));
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
    
    grid.justifyItems       = juce::Grid::JustifyItems::center;
    grid.alignItems         = juce::Grid::AlignItems::center;
    grid.justifyContent     = juce::Grid::JustifyContent::start;
    grid.alignContent       = juce::Grid::AlignContent::start;
    grid.autoFlow           = juce::Grid::AutoFlow::columnDense;
    grid.templateRows       = getRows (bounds);
    grid.templateColumns    = getColumns (bounds);
    grid.autoColumns        = getColumnTrack();
    grid.autoRows           = getRowTrack();
    grid.rowGap             = juce::Grid::Px (gap_);
    grid.columnGap          = juce::Grid::Px (gap_);
    grid.items              = getGridItems (viewed_);
    
    grid.performLayout (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
