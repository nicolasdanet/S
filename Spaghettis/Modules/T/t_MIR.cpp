
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// ====================================

/* Testing MIR features. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test160__enharmony() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaEnharmony, 160, "MICA - Enharmony")

    for (int i = 0; i < 128; ++i) {
    //
    mica::Concept chromatic = mica::map (i, mica::Flat);

    TTT_EXPECT (chromatic.isCyclic());
    TTT_EXPECT (chromatic.length() >= 2);
    TTT_EXPECT (chromatic == item (chromatic, 0));
    
    for (int j = 0; j < chromatic.length(); ++j) {
        mica::Concept enharmonic = item (chromatic, j);
        TTT_EXPECT (mica::map (enharmonic, mica::MIDIKeyNumber).getNumerator() == i);
    }
    //
    }
    
    for (int i = 0; i < 128; ++i) {
    //
    mica::Concept chromatic = mica::map (i, mica::Sharp);
    
    TTT_EXPECT (chromatic.isCyclic());
    TTT_EXPECT (chromatic.length() >= 2);
    TTT_EXPECT (chromatic == item (chromatic, 0));
    
    for (int j = 0; j < chromatic.length(); ++j) {
        mica::Concept enharmonic = item (chromatic, j);
        TTT_EXPECT (mica::map (enharmonic, mica::MIDIKeyNumber).getNumerator() == i);
    }
    //
    }
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test161__intervals() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaIntervals, 161, "MICA - Intervals")

    using namespace mica;           /* Avoid typing but could be ambiguous. */
    
    using mica::MIR::Interval;      /* Ditto. */
    
    /* Regression. */
    
    {
    //
    TTT_EXPECT (Interval::withNotes (G3, D4).getDirection()                             == Ascending);
    TTT_EXPECT (Interval::withNotes (G3, D3).getDirection()                             == Descending);
    TTT_EXPECT (Interval::withNotes (G3, G3).getDirection()                             == Unison);
    TTT_EXPECT (Interval::withNotes (FSharp3, GFlat3).getDirection()                    == Unison);

    TTT_EXPECT (Interval::withNotes (C3, G4).getOctaves().getNumerator()                == 1);
    TTT_EXPECT (Interval::withNotes (G4, C3).getOctaves().getNumerator()                == 1);
    TTT_EXPECT (Interval::withNotes (C2, C3).getOctaves().getNumerator()                == 0);
    TTT_EXPECT (Interval::withNotes (C2, C2).getOctaves().getNumerator()                == 0);
    TTT_EXPECT (Interval::withNotes (C0, G2).getOctaves().getNumerator()                == 2);
        
    TTT_EXPECT (Interval::withNotes (G3, D4).getDistance()                              == Fifth);
    TTT_EXPECT (Interval::withNotes (G3, D3).getDistance()                              == Fourth);
    TTT_EXPECT (Interval::withNotes (G3, G3).getDistance()                              == Unison);
    TTT_EXPECT (Interval::withNotes (G3, G4).getDistance()                              == Octave);
    TTT_EXPECT (Interval::withNotes (G3, DFlat4).getDistance()                          == Fifth);
    TTT_EXPECT (Interval::withNotes (G3, CSharp4).getDistance()                         == Fourth);

    TTT_EXPECT (Interval::withNotes (G3, D4).getQuality()                               == Perfect);
    TTT_EXPECT (Interval::withNotes (B3, G3).getQuality()                               == Major);
    TTT_EXPECT (Interval::withNotes (G3, DFlat4).getQuality()                           == Diminished);
    TTT_EXPECT (Interval::withNotes (G3, CSharp4).getQuality()                          == Augmented);
    TTT_EXPECT (Interval::withNotes (G3, FSharp4).getQuality()                          == Major);
    TTT_EXPECT (Interval::withNotes (AFlat8, BSharp8).getQuality()                      == DoublyAugmented);
    TTT_EXPECT (Interval::withNotes (A8, CSharp9).getQuality()                          == Major);
    TTT_EXPECT (Interval::withNotes (A8, DFlat9).getQuality()                           == Diminished);
    TTT_EXPECT (Interval::withNotes (AFlat8, EDoubleFlat9).getQuality()                 == Diminished);
    
    TTT_EXPECT (Interval::withNotes (F2, C3).getName()                                  == PerfectFifth);
    TTT_EXPECT (Interval::withNotes (BFlat2, F2).getName()                              == PerfectFourth);
    TTT_EXPECT (Interval::withNotes (F2, E2).getName()                                  == MinorSecond);
    TTT_EXPECT (Interval::withNotes (F2, E3).getName()                                  == MajorSeventh);
    TTT_EXPECT (Interval::withNotes (BFlat2, E2).getName()                              == DiminishedFifth);
    TTT_EXPECT (Interval::withNotes (D4, F4).getName()                                  == MinorThird);
    TTT_EXPECT (Interval::withNotes (F4, GFlat4).getName()                              == MinorSecond);
        
    TTT_EXPECT (Interval::withName (PerfectFifth).appliedTo (F3)                        == C4);
    TTT_EXPECT (Interval::withName (MinorThird).appliedTo (A0)                          == C1);
    TTT_EXPECT (Interval::withName (AugmentedFourth).appliedTo (B5)                     == ESharp6);
    TTT_EXPECT (Interval::withName (MinorSecond).appliedTo (C4)                         == DFlat4);
    TTT_EXPECT (Interval::withName (DoublyAugmentedSecond).appliedTo (AFlat2)           == BSharp2);
    TTT_EXPECT (Interval::withName (MajorThird).appliedTo (A1)                          == CSharp2);
    TTT_EXPECT (Interval::withName (DiminishedFourth).appliedTo (A7)                    == DFlat8);
    TTT_EXPECT (Interval::withName (TriplyDiminishedFifth).appliedTo (ASharp6)          == EDoubleFlat7);
    
    TTT_EXPECT (Interval::withName (PerfectOctave, Descending).appliedTo (GFlat3)       == GFlat2);
    TTT_EXPECT (Interval::withName (MajorThird, Descending).appliedTo (ASharp0)         == FSharp0);
    TTT_EXPECT (Interval::withName (DiminishedSeventh, Descending).appliedTo (B5)       == CDoubleSharp5);
    TTT_EXPECT (Interval::withName (MajorSixth, Descending).appliedTo (CDoubleFlat4)    == ETripleFlat3);
    TTT_EXPECT (Interval::withName (MinorThird, Ascending, 1).appliedTo (G3)            == BFlat4);
    TTT_EXPECT (Interval::withName (MajorThird, Descending, 1).appliedTo (D3)           == BFlat1);
    TTT_EXPECT (Interval::withName (MajorSecond, Ascending, 1).appliedTo (FFlat1)       == GFlat2);
    TTT_EXPECT (Interval::withName (MinorSecond, Descending, 1).appliedTo (CSharp5)     == BSharp3);
    //
    }
    
    /* Edge cases (e.g. Doubly-Diminished Second...). */
    
    {
    //
    Interval a = Interval::withNotes (GFlat3, FDoubleSharp3);
    Interval b = Interval::withNotes (FDoubleSharp3, GFlat3);
    Interval c = Interval::withNotes (C4, CFlat4);
    Interval d = Interval::withNotes (CFlat4, CSharp4);
    
    TTT_EXPECT (a.getDirection()        == Descending);
    TTT_EXPECT (a.getDistance()         == Second);
    TTT_EXPECT (a.getQuality()          == DoublyDiminished);
    TTT_EXPECT (b.getDirection()        == Ascending);
    TTT_EXPECT (b.getDistance()         == Second);
    TTT_EXPECT (b.getQuality()          == DoublyDiminished);
    TTT_EXPECT (c.getDirection()        == Descending);
    TTT_EXPECT (c.getDistance()         == Unison);
    TTT_EXPECT (c.getQuality()          == Augmented);
    TTT_EXPECT (d.getDirection()        == Ascending);
    TTT_EXPECT (d.getDistance()         == Unison);
    TTT_EXPECT (d.getQuality()          == DoublyAugmented);
        
    Interval invalid;
        
    TTT_EXPECT (invalid.isValid()       == false);
    TTT_EXPECT (invalid.getName()       == Undefined);
    TTT_EXPECT (invalid.getOctaves()    == Undefined);
    TTT_EXPECT (invalid.getDistance()   == Undefined);
    TTT_EXPECT (invalid.getQuality()    == Undefined);
    TTT_EXPECT (invalid.getDirection()  == Undefined);
    TTT_EXPECT (invalid.isSimple()      == true);
    TTT_EXPECT (invalid.isCompound()    == false);
    TTT_EXPECT (invalid.isEnharmonic()  == false);
    TTT_EXPECT (invalid.appliedTo (G3)  == Undefined);
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test162__tuple() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaTuple, 162, "MICA - Tuple")

        /* Initialize. */
    
        {
        //
        mica::MIR::Tuple<3> tuple;
        
        TTT_EXPECT (tuple[0] == mica::Undefined);
        TTT_EXPECT (tuple[1] == mica::Undefined);
        TTT_EXPECT (tuple[2] == mica::Undefined);
        
        tuple[0] = tuple[1] = tuple[2] = mica::Dorian;
        
        TTT_EXPECT (tuple[0] == mica::Dorian);
        
        tuple = mica::MIR::Tuple<3> ( { mica::Flat, mica::Natural, mica::Sharp } );
        
        TTT_EXPECT (tuple[0] == mica::Flat);
        TTT_EXPECT (tuple[1] == mica::Natural);
        TTT_EXPECT (tuple[2] == mica::Sharp);
        //
        }
    
        /* Initialize with an Array. */
    
        {
        //
        mica::Concept letters (mica::Letters);
        
        prim::Array<mica::Concept> t;
        
        for (int i = 0; i < letters.length(); ++i) { t.add (item (letters, i)); }
        
        const int offset = 2;
        
        mica::MIR::Tuple<3> tuple (t, offset);
        
        TTT_EXPECT (tuple[0] == mica::C);
        TTT_EXPECT (tuple[1] == mica::D);
        TTT_EXPECT (tuple[2] == mica::E);
        //
        }
    
        /* Hash. */
    
        {
        //
        mica::MIR::Tuple<3> a ( { mica::A, mica::B, mica::C } );
        mica::MIR::Tuple<3> b ( { mica::C, mica::A, mica::B } );
        mica::MIR::Tuple<2> c ( { mica::A, mica::B } );
        mica::MIR::Tuple<2> d ( { mica::B, mica::A } );
        
        TTT_EXPECT (a.hash() != b.hash());
        TTT_EXPECT (c.hash() != d.hash());
        TTT_EXPECT (a.hash() != c.hash());
        //
        }
        
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test163__utils() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaUtils, 163, "MICA - Utils")

    prim::Table<mica::Concept> accidentals;
    
    accidentals = mica::MIR::Utils::getAccidentalsByLetters (mica::ThreeSharps);
    
    TTT_EXPECT (accidentals[mica::A] == mica::Natural);
    TTT_EXPECT (accidentals[mica::B] == mica::Natural);
    TTT_EXPECT (accidentals[mica::C] == mica::Sharp);
    TTT_EXPECT (accidentals[mica::D] == mica::Natural);
    TTT_EXPECT (accidentals[mica::E] == mica::Natural);
    TTT_EXPECT (accidentals[mica::F] == mica::Sharp);
    TTT_EXPECT (accidentals[mica::G] == mica::Sharp);
    
    accidentals = mica::MIR::Utils::getAccidentalsByLetters (mica::TwoFlats);
    
    TTT_EXPECT (accidentals[mica::A] == mica::Natural);
    TTT_EXPECT (accidentals[mica::B] == mica::Flat);
    TTT_EXPECT (accidentals[mica::C] == mica::Natural);
    TTT_EXPECT (accidentals[mica::D] == mica::Natural);
    TTT_EXPECT (accidentals[mica::E] == mica::Flat);
    TTT_EXPECT (accidentals[mica::F] == mica::Natural);
    TTT_EXPECT (accidentals[mica::G] == mica::Natural);
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test164__spell() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaSpell, 164, "MICA - Spell")

    using mica::MIR::Interval;
    
    {
    //
    mica::MIR::Spell cMajor;
    
    TTT_EXPECT (cMajor.isValid());
    
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::F4, mica::C4))            == 0);
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::F4, mica::F5))            == 0);
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::C4, mica::B4))            == 0);
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::B4, mica::A4))            == 0);
    
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::C4, mica::EFlat4))        == 1);
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::C4, mica::FSharp4))       == 2);
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::ESharp4, mica::C4))       == 5);
    TTT_EXPECT (cMajor.getPenalty (Interval::withNotes (mica::ESharp4, mica::BSharp3))  == 6);

    mica::MIR::Spell bMajor (mica::BMajor);
    
    TTT_EXPECT (bMajor.isValid());
    
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::F4, mica::C4))            == 2);
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::F4, mica::F5))            == 2);
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::C4, mica::B4))            == 1);
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::B4, mica::A4))            == 1);
    
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::C4, mica::EFlat4))        == 2);
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::C4, mica::FSharp4))       == 2);
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::ESharp4, mica::C4))       == 6);
    TTT_EXPECT (bMajor.getPenalty (Interval::withNotes (mica::ESharp4, mica::BSharp3))  == 6);
    //
    }
    
    {
    //
    TTT_EXPECT (mica::MIR::Spell::keyIsValid (mica::CMajor));
    TTT_EXPECT (mica::MIR::Spell::keyIsValid (mica::EFlatPhrygian));
    TTT_EXPECT (mica::MIR::Spell::keyIsValid (mica::GSharpLocrian));
    
    TTT_EXPECT (!mica::MIR::Spell::keyIsValid (mica::Undefined));
    TTT_EXPECT (!mica::MIR::Spell::keyIsValid (mica::B));
    TTT_EXPECT (!mica::MIR::Spell::keyIsValid (mica::Sharp));
    //
    }
    
    {
    //
    const int midi[] = 
        { 
            60, 
            63, 
            67, 
            68, 
            59, 
            67, 
            66, 
            65, 
            64, 
            63, 
            62 
        };
        
    prim::Array<int> numbers;
    
    for (int i = 0; i < prim::sizeOfArray (midi); ++i) { numbers.add (midi[i]); }
    
    prim::Array<mica::Concept> result = (mica::MIR::Spell (mica::CMajor).getSpelling (numbers));
    
    TTT_EXPECT (result.size() == prim::sizeOfArray (midi));
    
    TTT_EXPECT (result[0]  == mica::C4);
    TTT_EXPECT (result[1]  == mica::EFlat4);
    TTT_EXPECT (result[2]  == mica::G4);
    TTT_EXPECT (result[3]  == mica::AFlat4);
    TTT_EXPECT (result[4]  == mica::B3);
    TTT_EXPECT (result[5]  == mica::G4);
    TTT_EXPECT (result[6]  == mica::FSharp4);
    TTT_EXPECT (result[7]  == mica::F4);
    TTT_EXPECT (result[8]  == mica::E4);
    TTT_EXPECT (result[9]  == mica::DSharp4);
    TTT_EXPECT (result[10] == mica::D4);
    
    result = mica::MIR::Spell (mica::BMajor).getSpelling (numbers);

    TTT_EXPECT (result.size() == prim::sizeOfArray (midi));
    
    TTT_EXPECT (result[0]  == mica::C4);
    TTT_EXPECT (result[1]  == mica::DSharp4);
    TTT_EXPECT (result[2]  == mica::G4);
    TTT_EXPECT (result[3]  == mica::GSharp4);
    TTT_EXPECT (result[4]  == mica::B3);
    TTT_EXPECT (result[5]  == mica::G4);
    TTT_EXPECT (result[6]  == mica::FSharp4);
    TTT_EXPECT (result[7]  == mica::F4);
    TTT_EXPECT (result[8]  == mica::E4);
    TTT_EXPECT (result[9]  == mica::DSharp4);
    TTT_EXPECT (result[10] == mica::D4);
    //
    }
        
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
