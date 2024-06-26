
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourSpace::mouseDrag (const juce::MouseEvent& e)
{
    const float x = static_cast<float> (e.x - edge_);
    const float y = static_cast<float> (e.y - edge_);
    const float w = static_cast<float> (getWidth() - (edge_ * 2));
    const float h = static_cast<float> (getHeight() - (edge_ * 2));
    
    const float saturation = x / w;
    const float value = 1.0f - (y / h);

    owner_.setSV (saturation, value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
