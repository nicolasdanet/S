
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

