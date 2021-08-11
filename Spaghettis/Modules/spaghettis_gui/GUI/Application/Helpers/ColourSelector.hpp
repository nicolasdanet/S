
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourSpaceMarker : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourSpaceMarker()
    {
        setInterceptsMouseClicks (false, false);
    }

    ~ColourSpaceMarker() = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        const float w = static_cast<float> (getWidth());
        const float h = static_cast<float> (getHeight());
        
        g.setColour (juce::Colour::greyLevel (0.1f));
        g.drawEllipse (1.0f, 1.0f, w - 2.0f, h - 2.0f, 1.0f);
        g.setColour (juce::Colour::greyLevel (0.9f));
        g.drawEllipse (2.0f, 2.0f, w - 4.0f, h - 4.0f, 1.0f);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSpaceMarker)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
/*
class ColourSpace : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ColourSpaceView (ColourSelector& cs, float& hue, float& sat, float& val, int edgeSize)
        : owner (cs), h (hue), s (sat), v (val), edge (edgeSize)
    {
        addAndMakeVisible (marker);
        setMouseCursor (MouseCursor::CrosshairCursor);
    }

    void paint (Graphics& g) override
    {
        if (colours.isNull())
        {
            auto width = getWidth() / 2;
            auto height = getHeight() / 2;
            colours = Image (Image::RGB, width, height, false);

            Image::BitmapData pixels (colours, Image::BitmapData::writeOnly);

            for (int y = 0; y < height; ++y)
            {
                auto val = 1.0f - (float) y / (float) height;

                for (int x = 0; x < width; ++x)
                {
                    auto sat = (float) x / (float) width;
                    pixels.setPixelColour (x, y, Colour (h, sat, val, 1.0f));
                }
            }
        }

        g.setOpacity (1.0f);
        g.drawImageTransformed (colours,
                                RectanglePlacement (RectanglePlacement::stretchToFit)
                                    .getTransformToFit (colours.getBounds().toFloat(),
                                                        getLocalBounds().reduced (edge).toFloat()),
                                false);
    }

    void mouseDown (const MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const MouseEvent& e) override
    {
        auto sat =        (float) (e.x - edge) / (float) (getWidth()  - edge * 2);
        auto val = 1.0f - (float) (e.y - edge) / (float) (getHeight() - edge * 2);

        owner.setSV (sat, val);
    }

    void updateIfNeeded()
    {
        if (lastHue != h)
        {
            lastHue = h;
            colours = {};
            repaint();
        }

        updateMarker();
    }

    void resized() override
    {
        colours = {};
        updateMarker();
    }

private:
    ColourSelector& owner;
    float& h;
    float& s;
    float& v;
    float lastHue = 0;
    const int edge;
    Image colours;
    ColourSpaceMarker marker;

    void updateMarker()
    {
        auto markerSize = jmax (14, edge * 2);
        auto area = getLocalBounds().reduced (edge);

        marker.setBounds (Rectangle<int> (markerSize, markerSize)
                            .withCentre (area.getRelativePoint (s, 1.0f - v)));
    }

    JUCE_DECLARE_NON_COPYABLE (ColourSpaceView)
};
*/
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourSlider : public juce::Slider {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourSlider (const juce::String& name) : juce::Slider (name)
    {
        setRange (0.0, 255.0, 1.0);
    }
    
    ~ColourSlider() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getTextFromValue (double value) override
    {
        return juce::String::toHexString (static_cast<int> (value)).toUpperCase().paddedLeft ('0', 2);
    }

    double getValueFromText (const juce::String& text) override
    {
        return static_cast<double> (text.getHexValue32());
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourSelector : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourSelector (const juce::Value& v) : value_ (v)
    {
    
    }
    
    ~ColourSelector() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::Value value_;
    //std::unique_ptr<ColourSpace> colourSpace_;
    //std::unique_ptr<HueSelector> hueSelector_;
    std::array<std::unique_ptr<juce::Slider>, 4> sliders_;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSelector)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

