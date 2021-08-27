
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
    explicit ColourSlider() : juce::Slider()
    {
        const int w = 50;
        const int h = getTextBoxHeight();
        
        setRange (0.0, 255.0, 1.0);
        setTextBoxStyle (juce::Slider::TextBoxRight, false, w, h);
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
        
        g.setColour (juce::Colours::black);
        g.drawEllipse (1.0f, 1.0f, w - 2.0f, h - 2.0f, 1.0f);
        g.setColour (juce::Colours::white);
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
    ColourSpace (ColourSelector& owner, int edge, float& h, float& s, float& v) :
        owner_ (owner),
        edge_ (edge),
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

    void mouseDrag (const juce::MouseEvent& e) override;

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
        const juce::Rectangle<int> area (getLocalBounds().reduced (edge_));
        
        auto pt = area.getRelativePoint (s_, 1.0f - v_);
        
        marker_.setBounds (juce::Rectangle<int> (edge_ * 2, edge_ * 2).withCentre (pt));
    }
    
private:
    ColourSelector& owner_;
    int edge_;
    float& h_;
    float& s_;
    float& v_;
    float lastHue_;
    juce::Image background_;
    ColourSpaceMarker marker_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSpace)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class HueSelectorMarker : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit HueSelectorMarker()
    {
        setInterceptsMouseClicks (false, false);
    }

    ~HueSelectorMarker() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        const float w = static_cast<float> (getWidth());
        const float h = static_cast<float> (getHeight());

        juce::Path p;
        
        p.addTriangle (1.0f, 1.0f, w * 0.3f, h * 0.5f, 1.0f, h - 1.0f);
        p.addTriangle (w - 1.0f, 1.0f, w * 0.7f, h * 0.5f, w - 1.0f, h - 1.0f);

        g.setColour (juce::Colours::white);
        g.fillPath (p);

        g.setColour (juce::Colours::black);
        g.strokePath (p, juce::PathStrokeType (1.2f));
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HueSelectorMarker)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class HueSelector : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit HueSelector (ColourSelector& owner, int edge, float& h) : owner_ (owner), edge_ (edge), h_ (h)
    {
        addAndMakeVisible (marker_);
    }
    
    ~HueSelector() = default;
        
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        juce::ColourGradient c;
        
        c.isRadial = false;
        c.point1.setXY (0.0f, edge_);
        c.point2.setXY (0.0f, static_cast<float> (getHeight() - edge_));

        for (float i = 0.0f; i <= 1.0f; i += 0.02f) { c.addColour (i, juce::Colour (i, 1.0f, 1.0f, 1.0f)); }

        g.setGradientFill (c);
        g.fillRect (getLocalBounds().reduced (edge_));
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override;

    void update()
    {
        resized();
    }

    void resized() override
    {
        juce::Rectangle<int> area = getLocalBounds().reduced (edge_);

        auto pt = area.getRelativePoint (0.5f, h_);
        
        marker_.setBounds (juce::Rectangle<int> (getWidth(), edge_ * 2).withCentre (pt));
    }
    
private:
    ColourSelector& owner_;
    int edge_;
    float& h_;
    HueSelectorMarker marker_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HueSelector)
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
        a_ (0.0f),
        edge_ (6),
        colourSpace_ (std::make_unique<ColourSpace> (*this, edge_, h_, s_, v_)),
        hueSelector_ (std::make_unique<HueSelector> (*this, edge_, h_))
    {
        fetchColour(); updateHSV();
        
        std::get<0> (sliders_).reset (new ColourSlider());
        std::get<1> (sliders_).reset (new ColourSlider());
        std::get<2> (sliders_).reset (new ColourSlider());
        std::get<3> (sliders_).reset (new ColourSlider());

        for (auto& slider : sliders_) {
            slider->onValueChange = [this] { setColour(); };
        }
        
        addAndMakeVisible (colourSpace_.get());
        addAndMakeVisible (hueSelector_.get());

        addAndMakeVisible (std::get<0> (sliders_).get());
        addAndMakeVisible (std::get<1> (sliders_).get());
        addAndMakeVisible (std::get<2> (sliders_).get());
        addAndMakeVisible (std::get<3> (sliders_).get());
        
        setSize (300, 280);
        
        updateViews();
    }
    
    ~ColourSelector() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;
    void setHue (float);
    void setSV (float, float);
    void setColour();

private:
    void fetchColour();
    void pushColour();
    void updateHSV();
    void updateViews();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::Value value_;
    juce::Colour colour_;
    float h_;
    float s_;
    float v_;
    float a_;
    int edge_;
    const std::unique_ptr<ColourSpace> colourSpace_;
    const std::unique_ptr<HueSelector> hueSelector_;
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

