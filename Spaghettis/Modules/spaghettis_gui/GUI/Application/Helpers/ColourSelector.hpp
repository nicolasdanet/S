
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

class ColourSpace : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourSelector;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    ColourSpace (ColourSelector& owner, float& h, float& s, float& v) :
        owner_ (owner),
        h_ (h),
        s_ (s),
        v_ (v),
        lastHue_ (0.0f)
    {
        addAndMakeVisible (marker_);
        setMouseCursor (juce::MouseCursor::CrosshairCursor);
    }

    ~ColourSpace() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        createBackgroundImageIfRequired();

        g.setOpacity (1.0f);
        g.drawImageTransformed (background_,
            juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit).getTransformToFit (background_.getBounds().toFloat(),
            getLocalBounds().reduced (edge_).toFloat()),
            false);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        // auto sat =        (float) (e.x - edge_) / (float) (getWidth()  - edge_ * 2);
        // auto val = 1.0f - (float) (e.y - edge_) / (float) (getHeight() - edge_ * 2);
        // owner.setSV (sat, val);
    }

    void update()
    {
        if (lastHue_ != h_) { lastHue_ = h_; resetBackgroundImage(); repaint(); }

        updateMarker();
    }

    void resized() override
    {
        resetBackgroundImage(); updateMarker();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void resetBackgroundImage()
    {
        background_ = juce::Image();
    }
    
    void createBackgroundImageIfRequired()
    {
        if (background_.isNull()) {
        //
        const int width  = getWidth()  / 2;
        const int height = getHeight() / 2;
        
        background_ = juce::Image (juce::Image::RGB, width, height, false);

        juce::Image::BitmapData pixels (background_, juce::Image::BitmapData::writeOnly);

        for (int y = 0; y < height; ++y) {
            const float value = 1.0f - y / static_cast<float> (height);
            for (int x = 0; x < width; ++x) {
                const float saturation = x / static_cast<float> (width);
                pixels.setPixelColour (x, y, juce::Colour (h_, saturation, value, 1.0f));
            }
        }
        //
        }
    }
    
    void updateMarker()
    {
        juce::Rectangle<int> area = getLocalBounds().reduced (edge_);
        
        auto pt = area.getRelativePoint (s_, 1.0f - v_);
        
        juce::Rectangle<int> r = juce::Rectangle<int> (edge_ * 2, edge_ * 2);
        
        marker_.setBounds (r.withCentre (pt));
    }
    
private:
    ColourSelector& owner_;
    float& h_;
    float& s_;
    float& v_;
    float lastHue_;
    juce::Image background_;
    ColourSpaceMarker marker_;

private:
    const int edge_ = 7;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSpace)
};

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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSlider)
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
    float h_;
    float s_;
    float v_;
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

