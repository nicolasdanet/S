
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
    return [o](const RunLayout::LayoutElement& e)
    {
        return (std::get<RunLayout::LAYOUT_POINTER> (e) == o);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunLayout::add (ObjectComponent* o, const juce::Rectangle<int>& bounds)
{
    auto r = std::find_if (viewed_.begin(), viewed_.end(), isObject (o));
    
    if (r != viewed_.end()) { std::get<RunLayout::LAYOUT_BOUNDS> (*r) = bounds.withZeroOrigin(); }
    else {
        viewed_.emplace_back (o, bounds.withZeroOrigin());
    }
}

void RunLayout::remove (ObjectComponent* o)
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
    return juce::Grid::TrackInfo (juce::Grid::Px (RunLayout::height_));
}

juce::Grid::TrackInfo getColumnTrack()
{
    return juce::Grid::TrackInfo (juce::Grid::Px (RunLayout::width_));
}

juce::Array<juce::Grid::TrackInfo> getTracks (int n, const juce::Grid::TrackInfo& track)
{
    juce::Array<juce::Grid::TrackInfo> t;
    
    t.insertMultiple (-1, track, n);
    
    return t;
}

juce::Array<juce::Grid::TrackInfo> getRows (const juce::Rectangle<int>& bounds)
{
    const int n = bounds.getHeight() / (RunLayout::gap_ + RunLayout::height_);
    
    return getTracks (n, getRowTrack());
}

juce::Array<juce::Grid::TrackInfo> getColumns (const juce::Rectangle<int>& bounds)
{
    const int n = bounds.getWidth() / (RunLayout::gap_ + RunLayout::width_);
    
    return getTracks (n, getColumnTrack());
}

juce::GridItem::Span getRowSpan (int h)
{
    const int hSpace = RunLayout::gap_ + RunLayout::height_;
    
    return juce::GridItem::Span (static_cast<int> (h / hSpace) + 1);
}

juce::GridItem::Span getColumnSpan (int w)
{
    const int wSpace = RunLayout::gap_ + RunLayout::width_;
    
    return juce::GridItem::Span (static_cast<int> (w / wSpace) + 1);
}

juce::Array<juce::GridItem> getGridItems (const RunLayout::LayoutContainer& viewed)
{
    juce::Array<juce::GridItem> items;
    
    for (const auto& [o, bounds] : viewed) {
    //
    const int w = bounds.getWidth();
    const int h = bounds.getHeight();

    o->setVisible (true);
    
    items.add (juce::GridItem (o).withArea (getRowSpan (h), getColumnSpan (w)).withSize (w, h));
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

void RunLayout::arrange (const juce::Rectangle<int>& bounds)
{
    juce::Grid grid;
    
    grid.justifyItems       = juce::Grid::JustifyItems::start;
    grid.alignItems         = juce::Grid::AlignItems::start;
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
