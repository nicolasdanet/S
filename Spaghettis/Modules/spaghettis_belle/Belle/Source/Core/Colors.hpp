
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using namespace prim;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Color {

friend class Colors;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Color() : r_ (0.0f), g_ (0.0f), b_ (0.0f), alpha_ (1.0f) 
    {
    }
    
    Color (float r, float g, float b, float a) : r_ (r), g_ (g), b_ (b), alpha_ (a) 
    {
    }

public:
    Color (const Color&) = default;
    Color (Color&&) = default;
    Color& operator = (const Color&) = default;
    Color& operator = (Color&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    float getRed() const
    {
        return r_;
    }
    
    float getGreen() const
    {
        return g_;
    }
    
    float getBlue() const
    {
        return b_;
    }
    
    float getAlpha() const
    {
        return alpha_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Color (int32 r, int32 g, int32 b) : r_ (r / 255.0f), g_ (g / 255.0f), b_ (b / 255.0f), alpha_ (1.0f)
    {
    }
    
private:
    float r_;
    float g_;
    float b_;
    float alpha_;

private:
    PRIM_LEAK_DETECTOR (Color)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.w3.org/TR/SVG/types.html#ColorKeywords > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Colors {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static const Color empty()                  { return Color (0.0f, 0.0f, 0.0f, 0.0f); }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static const Color aliceblue()              { return Color (240, 248, 255); }
    static const Color antiquewhite()           { return Color (250, 235, 215); }
    static const Color aqua()                   { return Color (0,   255, 255); }
    static const Color aquamarine()             { return Color (127, 255, 212); }
    static const Color azure()                  { return Color (240, 255, 255); }
    static const Color beige()                  { return Color (245, 245, 220); }
    static const Color bisque()                 { return Color (255, 228, 196); }
    static const Color black()                  { return Color (0,   0,   0);   }
    static const Color blanchedalmond()         { return Color (255, 235, 205); }
    static const Color blue()                   { return Color (0,   0,   255); }
    static const Color blueviolet()             { return Color (138, 43,  226); }
    static const Color brown()                  { return Color (165, 42,  42);  }
    static const Color burlywood()              { return Color (222, 184, 135); }
    static const Color cadetblue()              { return Color (95,  158, 160); }
    static const Color chartreuse()             { return Color (127, 255, 0);   }
    static const Color chocolate()              { return Color (210, 105, 30);  }
    static const Color coral()                  { return Color (255, 127, 80);  }
    static const Color cornflowerblue()         { return Color (100, 149, 237); }
    static const Color cornsilk()               { return Color (255, 248, 220); }
    static const Color crimson()                { return Color (220, 20,  60);  }
    static const Color cyan()                   { return Color (0,   255, 255); }
    static const Color darkblue()               { return Color (0,   0,   139); }
    static const Color darkcyan()               { return Color (0,   139, 139); }
    static const Color darkgoldenrod()          { return Color (184, 134, 11);  }
    static const Color darkgray()               { return Color (169, 169, 169); }
    static const Color darkgreen()              { return Color (0,   100, 0);   }
    static const Color darkgrey()               { return Color (169, 169, 169); }
    static const Color darkkhaki()              { return Color (189, 183, 107); }
    static const Color darkmagenta()            { return Color (139, 0,   139); }
    static const Color darkolivegreen()         { return Color (85,  107, 47);  }
    static const Color darkorange()             { return Color (255, 140, 0);   }
    static const Color darkorchid()             { return Color (153, 50,  204); }
    static const Color darkred()                { return Color (139, 0,   0);   }
    static const Color darksalmon()             { return Color (233, 150, 122); }
    static const Color darkseagreen()           { return Color (143, 188, 143); }
    static const Color darkslateblue()          { return Color (72,  61,  139); }
    static const Color darkslategray()          { return Color (47,  79,  79);  }
    static const Color darkslategrey()          { return Color (47,  79,  79);  }
    static const Color darkturquoise()          { return Color (0,   206, 209); }
    static const Color darkviolet()             { return Color (148, 0,   211); }
    static const Color deeppink()               { return Color (255, 20,  147); }
    static const Color deepskyblue()            { return Color (0,   191, 255); }
    static const Color dimgray()                { return Color (105, 105, 105); }
    static const Color dimgrey()                { return Color (105, 105, 105); }
    static const Color dodgerblue()             { return Color (30,  144, 255); }
    static const Color firebrick()              { return Color (178, 34,  34);  }
    static const Color floralwhite()            { return Color (255, 250, 240); }
    static const Color forestgreen()            { return Color (34,  139, 34);  }
    static const Color fuchsia()                { return Color (255, 0,   255); }
    static const Color gainsboro()              { return Color (220, 220, 220); }
    static const Color ghostwhite()             { return Color (248, 248, 255); }
    static const Color gold()                   { return Color (255, 215, 0);   }
    static const Color goldenrod()              { return Color (218, 165, 32);  }
    static const Color gray()                   { return Color (128, 128, 128); }
    static const Color grey()                   { return Color (128, 128, 128); }
    static const Color green()                  { return Color (0,   128, 0);   }
    static const Color greenyellow()            { return Color (173, 255, 47);  }
    static const Color honeydew()               { return Color (240, 255, 240); }
    static const Color hotpink()                { return Color (255, 105, 180); }
    static const Color indianred()              { return Color (205, 92,  92);  }
    static const Color indigo()                 { return Color (75,  0,   130); }
    static const Color ivory()                  { return Color (255, 255, 240); }
    static const Color khaki()                  { return Color (240, 230, 140); }
    static const Color lavender()               { return Color (230, 230, 250); }
    static const Color lavenderblush()          { return Color (255, 240, 245); }
    static const Color lawngreen()              { return Color (124, 252, 0);   }
    static const Color lemonchiffon()           { return Color (255, 250, 205); }
    static const Color lightblue()              { return Color (173, 216, 230); }
    static const Color lightcoral()             { return Color (240, 128, 128); }
    static const Color lightcyan()              { return Color (224, 255, 255); }
    static const Color lightgoldenrodyellow()   { return Color (250, 250, 210); }
    static const Color lightgray()              { return Color (211, 211, 211); }
    static const Color lightgreen()             { return Color (144, 238, 144); }
    static const Color lightgrey()              { return Color (211, 211, 211); }
    static const Color lightpink()              { return Color (255, 182, 193); }
    static const Color lightsalmon()            { return Color (255, 160, 122); }
    static const Color lightseagreen()          { return Color (32,  178, 170); }
    static const Color lightskyblue()           { return Color (135, 206, 250); }
    static const Color lightslategray()         { return Color (119, 136, 153); }
    static const Color lightslategrey()         { return Color (119, 136, 153); }
    static const Color lightsteelblue()         { return Color (176, 196, 222); }
    static const Color lightyellow()            { return Color (255, 255, 224); }
    static const Color lime()                   { return Color (0,   255, 0);   }
    static const Color limegreen()              { return Color (50,  205, 50);  }
    static const Color linen()                  { return Color (250, 240, 230); }
    static const Color magenta()                { return Color (255, 0,   255); }
    static const Color maroon()                 { return Color (128, 0,   0);   }
    static const Color mediumaquamarine()       { return Color (102, 205, 170); }
    static const Color mediumblue()             { return Color (0,   0,   205); }
    static const Color mediumorchid()           { return Color (186, 85,  211); }
    static const Color mediumpurple()           { return Color (147, 112, 219); }
    static const Color mediumseagreen()         { return Color (60,  179, 113); }
    static const Color mediumslateblue()        { return Color (123, 104, 238); }
    static const Color mediumspringgreen()      { return Color (0,   250, 154); }
    static const Color mediumturquoise()        { return Color (72,  209, 204); }
    static const Color mediumvioletred()        { return Color (199, 21,  133); }
    static const Color midnightblue()           { return Color (25,  25,  112); }
    static const Color mintcream()              { return Color (245, 255, 250); }
    static const Color mistyrose()              { return Color (255, 228, 225); }
    static const Color moccasin()               { return Color (255, 228, 181); }
    static const Color navajowhite()            { return Color (255, 222, 173); }
    static const Color navy()                   { return Color (0,   0,   128); }
    static const Color oldlace()                { return Color (253, 245, 230); }
    static const Color olive()                  { return Color (128, 128, 0);   }
    static const Color olivedrab()              { return Color (107, 142, 35);  }
    static const Color orange()                 { return Color (255, 165, 0);   }
    static const Color orangered()              { return Color (255, 69,  0);   }
    static const Color orchid()                 { return Color (218, 112, 214); }
    static const Color palegoldenrod()          { return Color (238, 232, 170); }
    static const Color palegreen()              { return Color (152, 251, 152); }
    static const Color paleturquoise()          { return Color (175, 238, 238); }
    static const Color palevioletred()          { return Color (219, 112, 147); }
    static const Color papayawhip()             { return Color (255, 239, 213); }
    static const Color peachpuff()              { return Color (255, 218, 185); }
    static const Color peru()                   { return Color (205, 133, 63);  }
    static const Color pink()                   { return Color (255, 192, 203); }
    static const Color plum()                   { return Color (221, 160, 221); }
    static const Color powderblue()             { return Color (176, 224, 230); }
    static const Color purple()                 { return Color (128, 0,   128); }
    static const Color red()                    { return Color (255, 0,   0);   }
    static const Color rosybrown()              { return Color (188, 143, 143); }
    static const Color royalblue()              { return Color (65,  105, 225); }
    static const Color saddlebrown()            { return Color (139, 69,  19);  }
    static const Color salmon()                 { return Color (250, 128, 114); }
    static const Color sandybrown()             { return Color (244, 164, 96);  }
    static const Color seagreen()               { return Color (46,  139, 87);  }
    static const Color seashell()               { return Color (255, 245, 238); }
    static const Color sienna()                 { return Color (160, 82,  45);  }
    static const Color silver()                 { return Color (192, 192, 192); }
    static const Color skyblue()                { return Color (135, 206, 235); }
    static const Color slateblue()              { return Color (106, 90,  205); }
    static const Color slategray()              { return Color (112, 128, 144); }
    static const Color slategrey()              { return Color (112, 128, 144); }
    static const Color snow()                   { return Color (255, 250, 250); }
    static const Color springgreen()            { return Color (0,   255, 127); }
    static const Color steelblue()              { return Color (70,  130, 180); }
    static const Color tan()                    { return Color (210, 180, 140); }
    static const Color teal()                   { return Color (0,   128, 128); }
    static const Color thistle()                { return Color (216, 191, 216); }
    static const Color tomato()                 { return Color (255, 99,  71);  }
    static const Color turquoise()              { return Color (64,  224, 208); }
    static const Color violet()                 { return Color (238, 130, 238); }
    static const Color wheat()                  { return Color (245, 222, 179); }
    static const Color white()                  { return Color (255, 255, 255); }
    static const Color whitesmoke()             { return Color (245, 245, 245); }
    static const Color yellow()                 { return Color (255, 255, 0);   }
    static const Color yellowgreen()            { return Color (154, 205, 50);  }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
