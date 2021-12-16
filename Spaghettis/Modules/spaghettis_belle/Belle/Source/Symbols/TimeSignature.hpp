
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Time_signature > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Shapes {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class TimeSignature {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    TimeSignature() : special_ (0x0000)
    {
    }

public:
    TimeSignature (const TimeSignature&) = default;
    TimeSignature (TimeSignature&&) = default;
    TimeSignature& operator = (const TimeSignature&) = default;
    TimeSignature& operator = (TimeSignature&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addToPath (Path& path) const
    {
        if (special_) {
            path.addPath (House::getInstance().getJoie().getGlyph (special_)->getPath(), House::kAffineJoie);

        } else {
            Path beats (getBeats());
            Path value (getValue());
            
            /* Arrange on staff lines. */
            
            Affine vA = Affine::translation (Vector (0.0, House::kStaffHeight / 4.0));
            Affine vB = Affine::translation (Vector (0.0, -House::kStaffHeight / 4.0));

            /* Center horizontally if needed. */
            
            Vector hA, hB;
            
            if ((beats_.size() > 1) || (value_.size() > 1)) { 
                hA = Vector (beats.getBounds().getWidth() / 2.0, 0.0); 
                hB = Vector (value.getBounds().getWidth() / 2.0, 0.0);
            }
            
            path.addPath (beats, vA * House::kAffinePetrucci * Affine::translation (-hA));
            path.addPath (value, vB * House::kAffinePetrucci * Affine::translation (-hB));
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    Path getBeats() const
    {
        return getNumbers (beats_);
    }
    
    Path getValue() const
    {
        return getNumbers (value_);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static Path getNumbers (const Array<unicode>& codes)
    {
        const Typeface& typeface (House::getInstance().getPetrucci());
        
        if (codes.size() < 2) { return typeface.getGlyph ((codes.size() ? codes[0] : 0x0000))->getPath(); } 
        else {
        //
        Path path (typeface.getGlyph (codes[0])->getPath());
        
        for (int i = 1; i < codes.size(); ++i) {
            Path t (typeface.getGlyph (codes[i])->getPath());
            double d = Optics::nonCollidingHorizontalByOutlines (path, t);
            path.addPath (t, Affine::translation (Vector (d, 0.0)));
        }
        
        return path;
        //
        }
    }
    
protected:
    static Array<unicode> getUnicodes (int n)
    {
        Array<unicode> codes;
        
        while (n > 0) {
            codes.add (static_cast<unicode> ((n % 10) + 0xf030));
            n = static_cast<int> (n / 10.0);
        }
        
        codes.reverse();
        
        return codes;
    }

protected:
    unicode special_;
    Array<unicode> beats_;
    Array<unicode> value_;

private:
    PRIM_LEAK_DETECTOR (TimeSignature)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct RegularTime : public TimeSignature {

public:
    RegularTime (int beats, int value)
    {
        beats_ = TimeSignature::getUnicodes (beats);
        value_ = TimeSignature::getUnicodes (value);
    }
};

struct CutTime : public TimeSignature {

public:
    CutTime() 
    {
        special_ = 0x004d;
    }
};

struct CommonTime : public TimeSignature {

public:
    CommonTime() 
    {
        special_ = 0x004c;
    }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Shapes

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
