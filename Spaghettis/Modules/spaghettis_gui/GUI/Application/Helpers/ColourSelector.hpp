
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

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

class ColourSelector;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ColourSpace : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
        if (background_.isNull()) { createBackgroundImage (getLocalBounds().reduced (edge_)); }
        
        g.drawImageAt (background_, edge_, edge_, false);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        const float x = static_cast<float> (e.x - edge_);
        const float y = static_cast<float> (e.y - edge_);
        const float w = static_cast<float> (getWidth() - (edge_ * 2));
        const float h = static_cast<float> (getHeight() - (edge_ * 2));
        
        const float saturation  = x / w;
        const float value       = 1.0f - (y / h);
        
        DBG (juce::String (saturation) + " / " + juce::String (value));
        
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
    
    void createBackgroundImage (const juce::Rectangle<int>& r)
    {
        const int width  = r.getWidth();
        const int height = r.getHeight();
        
        background_ = juce::Image (juce::Image::RGB, width, height, false);

        juce::Image::BitmapData pixels (background_, juce::Image::BitmapData::writeOnly);

        for (int y = 0; y < height; ++y) {
            const float value = 1.0f - (y / static_cast<float> (height));
            for (int x = 0; x < width; ++x) {
                const float saturation = x / static_cast<float> (width);
                pixels.setPixelColour (x, y, juce::Colour (h_, saturation, value, 1.0f));
            }
        }
    }
    
    void updateMarker()
    {
        const juce::Rectangle<int> area = getLocalBounds().reduced (edge_);
        
        auto pt = area.getRelativePoint (s_, 1.0f - v_);
        
        marker_.setBounds (juce::Rectangle<int> (edge_ * 2, edge_ * 2).withCentre (pt));
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

class ColourSelector : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourSelector (const juce::Value& v) :
        value_ (v),
        h_ (0.0f),
        s_ (0.0f),
        v_ (0.0f),
        colourSpace_ (std::make_unique<ColourSpace> (*this, h_, s_, v_))
    {
        setSize (300, 280);
        
        updateHSV();
        
        addAndMakeVisible (colourSpace_.get());
        
        update();
    }
    
    ~ColourSelector() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateHSV();
    void update();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::Value value_;
    float h_;
    float s_;
    float v_;
    const std::unique_ptr<ColourSpace> colourSpace_;
    //std::unique_ptr<HueSelector> hueSelector_;
    //std::array<std::unique_ptr<juce::Slider>, 4> sliders_;

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

