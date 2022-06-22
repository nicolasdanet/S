
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

int getRowSpan (int h)
{
    return static_cast<int> (h / RunLayout::height_) + 1;
}

int getColumnSpan (int w)
{
    return static_cast<int> (w / RunLayout::width_) + 1;
}

/* https://forum.juce.com/t/br-juce-grid-hangs-performing-layout/51878/6 */

int getMaximumRowSpan (const RunLayout::LayoutContainer& viewed)
{
    int m = 1;
    
    for (const auto& [o, bounds] : viewed) { m = juce::jmax (m, getRowSpan (bounds.getHeight())); }
    
    return m;
}

int getMaximumColumnSpan (const RunLayout::LayoutContainer& viewed)
{
    int m = 1;
    
    for (const auto& [o, bounds] : viewed) { m = juce::jmax (m, getColumnSpan (bounds.getWidth())); }
    
    return m;
}

auto getRows (const juce::Rectangle<int>& bounds, const RunLayout::LayoutContainer& viewed)
{
    const int n = bounds.getHeight() / (RunLayout::hGap_ + RunLayout::height_);
        
    return getTracks (juce::jmax (n, getMaximumRowSpan (viewed)), getRowTrack());
}

auto getColumns (const juce::Rectangle<int>& bounds, const RunLayout::LayoutContainer& viewed)
{
    const int n = bounds.getWidth() / (RunLayout::wGap_ + RunLayout::width_);
    
    return getTracks (juce::jmax (n, getMaximumColumnSpan (viewed)), getColumnTrack());
}

juce::Array<juce::GridItem> getGridItems (const RunLayout::LayoutContainer& viewed)
{
    juce::Array<juce::GridItem> items;
    
    for (const auto& [o, bounds] : viewed) {
    //
    const int w = bounds.getWidth();
    const int h = bounds.getHeight();
    const juce::GridItem::Span rSpan (getRowSpan (h));
    const juce::GridItem::Span cSpan (getColumnSpan (w));
    
    o->setVisible (true);
    
    items.add (juce::GridItem (o).withArea (rSpan, cSpan).withSize (w, h));
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

int RunLayout::snapWidthToFitColumns (int w)
{
    const int width = getColumnSpan (w) * RunLayout::width_;
    
    return juce::jmax (w, width - 1);
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
    grid.autoFlow           = juce::Grid::AutoFlow::column;
    grid.templateRows       = getRows (bounds, viewed_);
    grid.templateColumns    = getColumns (bounds, viewed_);
    grid.autoColumns        = getColumnTrack();
    grid.autoRows           = getRowTrack();
    grid.rowGap             = juce::Grid::Px (hGap_);
    grid.columnGap          = juce::Grid::Px (wGap_);
    grid.items              = getGridItems (viewed_);
    
    grid.performLayout (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
