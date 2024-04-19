
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ParametersView::ParametersView (const data::Data& data, const ParametersBase& base) :
    data_ (data),
    base_ (base),
    expanded_ (0),
    expandedLast_ (0)
{
    ViewCommon::buildPanel (data_, base_, *this);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font ParametersView::getFont() const
{
    return base_.getFont();
}

int ParametersView::getNumberOfPanels() const
{
    return panel_.getNumPanels();
}

juce::ConcertinaPanel& ParametersView::getConcertinaPanel()
{
    return panel_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool ParametersView::isPanelExpanded (int i)
{
    return (i == expanded_);
}

void ParametersView::expandPanel (int index)
{
    const int i = juce::jmax (0, index);
    
    if (getNumberOfPanels() > 1) {
    //
    if (expanded_ == i) { jassert (expandedLast_ != i); expandPanel (expandedLast_); }
    else {
        expandedLast_   = expanded_;
        expanded_       = i;
        panel_.expandPanelFully (panel_.getPanel (i), true);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParametersView::resizeConcertinaPanel (const juce::Rectangle<int>& bounds)
{
    panel_.setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParametersView::addPanel (juce::PropertyPanel* p)
{
    const int i = getNumberOfPanels();
    
    auto h = std::make_unique<ParametersViewHeader> (p->getName(), i, this);
    
    panel_.addPanel (-1, p, true);
    panel_.setCustomPanelHeader (p, h.release(), true);
    panel_.setPanelHeaderSize (p, base_.getRequiredHeight() + 6);
    
    expanded_ = i;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
