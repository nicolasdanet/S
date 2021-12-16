
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
// MARK: -

struct HundredTwentyEighth : public MusicBase::Chord {

public:
    HundredTwentyEighth() : MusicBase::Chord (Ratio (1, 128)) 
    {
    }
};

struct SixtyFourth : public MusicBase::Chord {

public:
    SixtyFourth() : MusicBase::Chord (Ratio (1, 64)) 
    {
    }
};

struct ThirtySecond : public MusicBase::Chord {

public:
    ThirtySecond() : MusicBase::Chord (Ratio (1, 32)) 
    {
    }
};

struct Sixteenth : public MusicBase::Chord {

public:
    Sixteenth() : MusicBase::Chord (Ratio (1, 16)) 
    {
    }
};

struct Eighth : public MusicBase::Chord {

public:
    Eighth() : MusicBase::Chord (Ratio (1, 8)) 
    {
    }
};

struct Quarter : public MusicBase::Chord {

public:
    Quarter() : MusicBase::Chord (Ratio (1, 4)) 
    {
    }
};

struct Half : public MusicBase::Chord {

public:
    Half() : MusicBase::Chord (Ratio (1, 2)) 
    {
    }
};

struct Whole : public MusicBase::Chord {

public:
    Whole() : MusicBase::Chord (Ratio (1, 1)) 
    {
    }
};

struct DoubleWhole : public MusicBase::Chord {

public:
    DoubleWhole() : MusicBase::Chord (Ratio (2, 1)) 
    {
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct HundredTwentyEighthDotted : public MusicBase::Chord {

public:
    HundredTwentyEighthDotted() : MusicBase::Chord (Ratio (3, 256)) 
    {
    }
};

struct SixtyFourthDotted : public MusicBase::Chord {

public:
    SixtyFourthDotted() : MusicBase::Chord (Ratio (3, 128)) 
    {
    }
};

struct ThirtySecondDotted : public MusicBase::Chord {

public:
    ThirtySecondDotted() : MusicBase::Chord (Ratio (3, 64)) 
    {
    }
};

struct SixteenthDotted : public MusicBase::Chord {

public:
    SixteenthDotted() : MusicBase::Chord (Ratio (3, 32)) 
    {
    }
};

struct EighthDotted : public MusicBase::Chord {

public:
    EighthDotted() : MusicBase::Chord (Ratio (3, 16)) 
    {
    }
};

struct QuarterDotted : public MusicBase::Chord {

public:
    QuarterDotted() : MusicBase::Chord (Ratio (3, 8)) 
    {
    }
};

struct HalfDotted : public MusicBase::Chord {

public:
    HalfDotted() : MusicBase::Chord (Ratio (3, 4)) 
    {
    }
};

struct WholeDotted : public MusicBase::Chord {

public:
    WholeDotted() : MusicBase::Chord (Ratio (3, 2)) 
    {
    }
};

struct DoubleWholeDotted : public MusicBase::Chord {

public:
    DoubleWholeDotted() : MusicBase::Chord (Ratio (3, 1)) 
    {
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
