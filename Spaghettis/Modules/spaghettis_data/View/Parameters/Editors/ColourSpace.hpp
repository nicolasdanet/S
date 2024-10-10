
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

