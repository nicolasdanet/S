
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool RunLayout::isEmpty() const
{
    return viewed_.empty();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

auto isSameObject (ObjectComponent* o)
{
    return [o](const RunLayoutElement& e)
    {
        return (e.getObject() == o);
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
    auto r = std::find_if (viewed_.begin(), viewed_.end(), isSameObject (o));
    
    if (r != viewed_.end()) { r->setBounds (bounds.withZeroOrigin()); }
    else {
        viewed_.emplace_back (o, bounds.withZeroOrigin());
    }
}

void RunLayout::remove (ObjectComponent* o)
{
    o->setVisible (false);
    
    viewed_.erase (std::remove_if (viewed_.begin(), viewed_.end(), isSameObject (o)), viewed_.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::vector<ObjectComponent*> RunLayout::getAllObjects() const
{
    return std::vector<ObjectComponent*> (viewed_.begin(), viewed_.end());
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

juce::Array<juce::Grid::TrackInfo> getRows (const juce::Rectangle<int>& bounds)
{
    const int n = bounds.getHeight() / (RunLayout::hGap_ + RunLayout::height_);
        
    return getTracks (juce::jmax (n, 1), getRowTrack());
}

juce::Array<juce::Grid::TrackInfo> getColumns (const juce::Rectangle<int>& bounds)
{
    const int n = bounds.getWidth() / (RunLayout::wGap_ + RunLayout::width_);
    
    return getTracks (juce::jmax (n, 1), getColumnTrack());
}

juce::Array<juce::GridItem> getGridItems (const std::vector<RunLayoutElement>& viewed)
{
    juce::Array<juce::GridItem> items;
    
    for (const auto& e : viewed) {
    //
    const auto o      = e.getObject();
    const auto bounds = e.getBounds();
    
    const int h = bounds.getHeight();
    const int w = bounds.getWidth();
    const int m = getRowSpan (h);
    const int n = getColumnSpan (w);
    
    const juce::GridItem::Span rSpan (m);
    const juce::GridItem::Span cSpan (n);
    
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

/* https://developer.mozilla.org/en-US/docs/Web/CSS/justify-content */

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
