
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/  

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The house contains all the constants in order to construct symbols. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace belle { 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class House {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    House()
    {
        joie_.importBinary (JoieBellefont);
        gentium_.importBinary (GentiumBasicBoldItalicBellefont);
        petrucci_.importBinary (PetrucciBellefont);
    }

public:
    House (const House&) = delete;
    House& operator = (const House&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    const Typeface& getJoie() const
    {
        return joie_;
    }

    const Typeface& getGentium() const
    {
        return gentium_;
    }
    
    const Typeface& getPetrucci() const
    {
        return petrucci_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static const House& getInstance();
 
public:
    static const int    kStaffLines;
    static const double kStaffLineThickness; 
    static const double kStaffSpaceWidth;
    static const double kStaffHeight;
    static const double kStaffDistance;
    static const double kSpacingMinimum;
    static const double kSpacingEmpty;
    static const double kDensityUnit;
    static const int    kDensityThreshold;
    static const int    kDensityStep;
    static const double kDensityExtra;
    static const double kMarginMinimum;
    static const double kAdjustMinimum;
    static const double kBraceHeight; 
    static const double kBraceWidthRelative;
    static const double kBraceDistance;
    static const double kBarlineThickness;
    static const double kBarlineDotDistance;
    static const double kBarlineDotSize;
    static const double kDoubleBarlineDistance;
    static const double kDoubleBarlineThickness;
    static const double kBarlineDoubleDistance;
    static const double kBarlineDoubleThickness;
    static const double kAccidentalDistance;
    static const double kAccidentalExtra;
    static const double kNoteHeadAngle;
    static const double kNoteHeadHeight;
    static const double kNoteHeadWidthPrecise;
    static const double kNoteHeadWidth;
    static const double kNoteStemHeight;
    static const double kNoteStemThickness;
    static const double kNoteStemCapRelative;
    static const double kNoteHollowAngle;
    static const double kNoteHollowScale;
    static const double kNoteHollowAngleWhole;
    static const double kNoteHollowScaleWhole;
    static const double kTieHeight;
    static const double kTieThickness;
    static const double kTieOptical;
    static const double kTieAdjustThresholdWidth;
    static const double kTieAdjustThresholdHead;
    static const double kTieAdjustMinimum;
    static const double kBeamThickness;
    static const double kBeamDistance;
    static const double kBeamShort;
    static const double kBeamSlantMaximum;
    static const int    kBeamMaximum;
    static const int    kBeamPatternMaximum;
    static const double kTupletBracketDistance;
    static const double kTupletBracketExtra;
    static const double kTupletBracketThickness;
    static const double kTupletBracketHook;
    static const double kTupletBracketAngleMaximum;
    static const double kTupletNumeralDistance;
    static const int    kTupletDefault;
    static const double kFlagDistance;
    static const double kLedgerExtent;
    static const double kRhythmicDotSize;
    static const double kRhythmicDotDistance;
    static const double kRhythmicDotExtra; 
    static const int    kRhythmicDotMaximum;
    static const Color  kColorDefault;
    static const Color  kColorSelected;
    static const Ratio  kSpacingBarline;
    static const Ratio  kSpacingTimeSignature;
    static const Affine kAffineGentium;
    static const Affine kAffinePetrucci;
    static const Affine kAffineJoie;
    static const Affine kAffineJoieFlag;
    
private:
    Typeface joie_;
    Typeface gentium_;
    Typeface petrucci_;

private:
    PRIM_LEAK_DETECTOR (House)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const int    House::kStaffLines                     = 5;                                    /* Constant. */
const double House::kStaffLineThickness             = 0.14;
const double House::kStaffSpaceWidth                = 1.0;
const double House::kStaffHeight                    = kStaffSpaceWidth * 4.0;
const double House::kStaffDistance                  = 6.0;
const double House::kSpacingMinimum                 = 0.6;
const double House::kSpacingEmpty                   = 0.1;
const double House::kDensityUnit                    = 3.0;
const int    House::kDensityThreshold               = 6;
const int    House::kDensityStep                    = 3;
const double House::kDensityExtra                   = 0.5;
const double House::kMarginMinimum                  = 2.0;
const double House::kAdjustMinimum                  = 0.15;
const double House::kBraceHeight                    = kStaffDistance + (kStaffHeight * 2.0);
const double House::kBraceWidthRelative             = 0.8;
const double House::kBraceDistance                  = 2.0;
const double House::kBarlineThickness               = 0.2;
const double House::kBarlineDotDistance             = 0.8;
const double House::kBarlineDotSize                 = 0.5;
const double House::kDoubleBarlineThickness         = 0.15;
const double House::kDoubleBarlineDistance          = 0.4;
const double House::kBarlineDoubleDistance          = 0.8;
const double House::kBarlineDoubleThickness         = 0.6;
const double House::kAccidentalDistance             = 0.25;
const double House::kAccidentalExtra                = 0.2;
const double House::kNoteHeadAngle                  = 0.11 * kPi;
const double House::kNoteHeadHeight                 = 1.0;                                  /* Constant. */
const double House::kNoteHeadWidthPrecise           = 1.36;
const double House::kNoteHeadWidth                  = 1.4;
const double House::kNoteStemHeight                 = 3.5;
const double House::kNoteStemThickness              = 0.14;
const double House::kNoteStemCapRelative            = 0.8;
const double House::kNoteHollowAngle                = 0.20 * kPi;
const double House::kNoteHollowScale                = 0.48;
const double House::kNoteHollowAngleWhole           = 0.75 * kPi;
const double House::kNoteHollowScaleWhole           = 0.55;
const double House::kTieHeight                      = 0.35;
const double House::kTieThickness                   = 0.1;
const double House::kTieOptical                     = 0.15;
const double House::kTieAdjustThresholdWidth        = 15.0;
const double House::kTieAdjustThresholdHead         = 2.0;
const double House::kTieAdjustMinimum               = 0.15;
const double House::kBeamThickness                  = 0.55;
const double House::kBeamDistance                   = 0.14;
const double House::kBeamShort                      = 1.0;
const double House::kBeamSlantMaximum               = 2.0;
const int    House::kBeamMaximum                    = 6;
const int    House::kBeamPatternMaximum             = 5;
const double House::kTupletBracketDistance          = 0.75;
const double House::kTupletBracketExtra             = 0.3;
const double House::kTupletBracketThickness         = 0.1;
const double House::kTupletBracketHook              = 0.5;
const double House::kTupletBracketAngleMaximum      = 0.2;
const double House::kTupletNumeralDistance          = 1.25;
const int    House::kTupletDefault                  = 3;
const double House::kFlagDistance                   = 0.95;
const double House::kLedgerExtent                   = 0.2;
const double House::kRhythmicDotSize                = 0.4;
const double House::kRhythmicDotDistance            = 0.6;
const double House::kRhythmicDotExtra               = 0.2; 
const int    House::kRhythmicDotMaximum             = 4;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

const Color  House::kColorDefault                   = Colors::black();
const Color  House::kColorSelected                  = Colors::orange();
const Ratio  House::kSpacingBarline                 = Ratio (1, 16);
const Ratio  House::kSpacingTimeSignature           = Ratio (1, 4);
const Affine House::kAffineGentium                  = Affine::scale (Vector (2.0, 2.0));
const Affine House::kAffinePetrucci                 = Affine::scale (Vector (4.0, 4.0));
const Affine House::kAffineJoie                     = Affine::scale (Vector (4.0, 4.0));
const Affine House::kAffineJoieFlag                 = Affine::scale (Vector (1.0, 0.7));
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

const House& House::getInstance()
{
    static House h;
    return h; 
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // BELLE_COMPILE_INLINE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace belle

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
