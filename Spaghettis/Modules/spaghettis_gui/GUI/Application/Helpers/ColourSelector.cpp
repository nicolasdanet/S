
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::callOutBoxBackground));
}
    
void ColourSelector::resized()
{
    colourSpace_->setBounds (getLocalBounds());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSelector::updateHSV()
{
    LookAndFeel::getColourFromValue (value_).getHSB (h_, s_, v_);
}

void ColourSelector::update()
{
    /*
    if (sliders[0] != nullptr)
    {
        sliders[0]->setValue ((int) colour.getRed(),   notification);
        sliders[1]->setValue ((int) colour.getGreen(), notification);
        sliders[2]->setValue ((int) colour.getBlue(),  notification);
        sliders[3]->setValue ((int) colour.getAlpha(), notification);
    }
    */
    
    colourSpace_->update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
