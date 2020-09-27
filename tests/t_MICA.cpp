
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Testing the MICA library. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
void test150__standard() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaStandard, 150, "MICA - Standard")

    {
    //
    mica::Concept c;
    TTT_EXPECT (c.toString() == std::string ("(Undefined)"));
    
    c = mica::Concept ("");
    TTT_EXPECT (c == mica::Undefined);
    
    c = mica::Concept ("Le sentier longeait la falaise.");
    TTT_EXPECT (c == mica::Undefined);
    TTT_EXPECT (mica::Concept::asHex (c) == std::string ("00000000000000000000000000000000"));
    
    c = mica::Concept ("D Mixolydian");
    TTT_EXPECT (c.toString() == std::string ("D Mixolydian"));
    TTT_EXPECT (mica::Concept::asHex (c) == std::string ("0b103de4673b3dcb9e47229298d9d463"));
    
    c = mica::C;
    TTT_EXPECT (c.toString() == std::string ("C"));
    TTT_EXPECT (mica::Concept::asHex (c) == std::string ("5e3d3a92ee26396d8c7df2b2592be252"));
    
    c = mica::Concept (-1234);
    
    TTT_EXPECT (c.toString()                   == std::string ("(Integer)"));
    TTT_EXPECT (c.getNumerator()               == -1234);
    TTT_EXPECT (c.getDenominator()             == 1);
    
    c = mica::Concept (prim::Ratio (1234, -2468));
    
    TTT_EXPECT (c.toString()                   == std::string ("(Ratio)"));
    TTT_EXPECT (c.getNumerator()               == -1);
    TTT_EXPECT (c.getDenominator()             == 2);
    //
    }
    
    {
    //
    mica::Concept c;
    
    TTT_EXPECT (c                              == mica::Undefined);
    TTT_EXPECT (c.isValid()                    == true);
    TTT_EXPECT (c.toString()                   == std::string ("(Undefined)"));
    TTT_EXPECT (c.getDenominator()             == 0);
    TTT_EXPECT (c.getNumerator()               == 0);
    TTT_EXPECT (c.toRatio().isUndefined()      == true);
    TTT_EXPECT (c.isUndefined()                == true);
    TTT_EXPECT (c.isInteger()                  == false);
    TTT_EXPECT (c.isNumber()                   == false);
    TTT_EXPECT (c.isCyclic()                   == false);
    TTT_EXPECT (c.isSequence()                 == false);
    TTT_EXPECT (c.length()                     == 0);
    //
    }

    /* Using raw UUID for testing purpose only. */
    
    {
    //
    mica::UUID bad1 = { 0xbebefadacacafadeULL, 0xcacafadebebefadaULL };
    mica::UUID bad2 = { 0x0000000000000064ULL, 0x0000000000000032ULL };
    
    TTT_EXPECT (bad1 == mica::UUID::withHex (std::string ("bebefadacacafadecacafadebebefada")));
    TTT_EXPECT (bad2 == mica::UUID::withHex (std::string ("00000000000000640000000000000032")));
    
    mica::Concept wrong1 (bad1);
    mica::Concept wrong2 (bad2);
    
    TTT_EXPECT (wrong1.isValid()               == false);
    TTT_EXPECT (wrong1.toString()              == std::string ("(Wrong)"));
    TTT_EXPECT (wrong1.getDenominator()        == 0);
    TTT_EXPECT (wrong1.getNumerator()          == 0);
    TTT_EXPECT (wrong1.toRatio().isUndefined() == true);
    TTT_EXPECT (wrong1.isUndefined()           == false);
    TTT_EXPECT (wrong1.isInteger()             == false);
    TTT_EXPECT (wrong1.isNumber()              == false);
    TTT_EXPECT (wrong1.isCyclic()              == false);
    TTT_EXPECT (wrong1.isSequence()            == false);
    TTT_EXPECT (wrong1.length()                == 0);
    
    TTT_EXPECT (wrong2.isValid()               == false);
    TTT_EXPECT (wrong2.toString()              == std::string ("(Wrong)"));
    TTT_EXPECT (wrong2.getDenominator()        == 0);
    TTT_EXPECT (wrong2.getNumerator()          == 0);
    TTT_EXPECT (wrong2.toRatio().isUndefined() == true);
    TTT_EXPECT (wrong2.isUndefined()           == false);
    TTT_EXPECT (wrong2.isInteger()             == false);
    TTT_EXPECT (wrong2.isNumber()              == false);
    TTT_EXPECT (wrong2.isCyclic()              == false);
    TTT_EXPECT (wrong2.isSequence()            == false);
    TTT_EXPECT (wrong2.length()                == 0);
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
void test151__database() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaDatabase, 151, "MICA - Database")

    {
    //
    mica::Concept c;
    
    c = mica::map (mica::A, mica::B);
    
    TTT_EXPECT (c == mica::WholeStep);
    TTT_EXPECT (c.toString() == std::string ("Whole Step"));
    
    c = mica::map (mica::B, mica::A);
    
    TTT_EXPECT (c == mica::WholeStep);
    TTT_EXPECT (c.toString() == std::string ("Whole Step"));

    c = mica::map (mica::D, mica::Flat);
    
    TTT_EXPECT (c == mica::DFlat);
    TTT_EXPECT (c.toString() == std::string ("D Flat"));
    
    c = mica::map (mica::D, mica::Flat, 4);
    
    TTT_EXPECT (c == mica::DFlat4);
    TTT_EXPECT (c.toString() == std::string ("D4 Flat"));
    
    c = mica::map (mica::D, 4);
    
    TTT_EXPECT (c == mica::D4);
    TTT_EXPECT (c.toString() == std::string ("D4"));
    
    c = mica::map (mica::DFlat4, mica::DiatonicPitch);
    
    TTT_EXPECT (c == mica::D4);
    TTT_EXPECT (c.toString() == std::string ("D4"));
    
    c = mica::map (mica::DFlat4, mica::ChromaticNote);
    
    TTT_EXPECT (c == mica::DFlat);
    TTT_EXPECT (c.toString() == std::string ("D Flat"));
    
    c = mica::map (mica::DFlat4, mica::Accidental);
    
    TTT_EXPECT (c == mica::Flat);
    TTT_EXPECT (c.toString() == std::string ("Flat"));
    
    c = mica::map (mica::DFlat4, mica::Letter);
    
    TTT_EXPECT (c == mica::D);
    TTT_EXPECT (c.toString() == std::string ("D"));

    c = mica::map (2, mica::TrebleClef);
    
    TTT_EXPECT (c == mica::D5);
    TTT_EXPECT (c.toString() == std::string ("D5"));
    
    c = mica::map (mica::D, mica::Flat, 6);
    
    TTT_EXPECT (c == mica::DFlat6);
    TTT_EXPECT (c.toString() == std::string ("D6 Flat"));
    
    c = mica::map (mica::F, mica::Natural, 0);
    
    TTT_EXPECT (c == mica::F0);
    TTT_EXPECT (c.toString() == std::string ("F0"));
    
    c = mica::map (mica::C, mica::DoubleSharp, -1);
    
    TTT_EXPECT (c == mica::CDoubleSharp_1);
    TTT_EXPECT (c.toString() == std::string ("C-1 Double Sharp"));
    
    c = mica::map (mica::DFlat, mica::Major);
    
    TTT_EXPECT (c == mica::DFlatMajor);
    TTT_EXPECT (c.toString() == std::string ("D Flat Major"));
    
    c = mica::map (mica::DFlat, mica::Dorian);
    
    TTT_EXPECT (c == mica::DFlatDorian);
    TTT_EXPECT (c.toString() == std::string ("D Flat Dorian"));
    
    c = mica::map (mica::DFlatMajor, mica::KeySignature);
    
    TTT_EXPECT (c == mica::FiveFlats);
    TTT_EXPECT (c.toString() == std::string ("Five Flats"));
    
    c = mica::map (mica::DFlatDorian, mica::KeySignature);
    
    TTT_EXPECT (c == mica::SevenFlats);
    TTT_EXPECT (c.toString() == std::string ("Seven Flats"));
    
    c = mica::map (mica::DFlatMajor, mica::Mode);
    
    TTT_EXPECT (c == mica::Major);
    TTT_EXPECT (c.toString() == std::string ("Major"));
    
    c = mica::map (mica::DFlatDorian, mica::Mode);
    
    TTT_EXPECT (c == mica::Dorian);
    TTT_EXPECT (c.toString() == std::string ("Dorian"));
  
    c = mica::map (mica::Fourth, mica::Perfect);
    
    TTT_EXPECT (c == mica::PerfectFourth);
    TTT_EXPECT (c.toString() == std::string ("Perfect Fourth"));
    
    c = mica::map (mica::PerfectFourth, mica::Distance);
    
    TTT_EXPECT (c == mica::Fourth);
    TTT_EXPECT (c.toString() == std::string ("Fourth"));
    
    c = mica::map (mica::PerfectFourth, mica::Quality);
    
    TTT_EXPECT (c == mica::Perfect);
    TTT_EXPECT (c.toString() == std::string ("Perfect"));
    //
    }
    
    {
    //
    TTT_EXPECT (mica::Concept (mica::Letters).isSequence()                     == true);
    TTT_EXPECT (mica::Concept (mica::Letters).isCyclic()                       == true);
    TTT_EXPECT (mica::Concept (mica::Letters).length()                         == 7);
    
    TTT_EXPECT (mica::map (mica::DFlat4, mica::Octave).getNumerator()          == 4);
    TTT_EXPECT (mica::map (mica::DFlat4, mica::MIDIKeyNumber).getNumerator()   == 61);
    TTT_EXPECT (mica::map (mica::D4, mica::TrebleClef).getNumerator()          == -5);
    
    TTT_EXPECT (mica::index (mica::Letters, mica::C).getNumerator()            == 2);
    TTT_EXPECT (mica::index (mica::Letters, mica::C, mica::G).getNumerator()   == 4);
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
void test152__signature() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaSignature, 152, "MICA - Signature")

    {
    //
    mica::Concept tonic = mica::Concept ("F Sharp");
    mica::Concept mode  = mica::Concept ("Minor");
    mica::Concept key   = mica::map (tonic, mode);
    
    TTT_EXPECT (key.toString() == std::string ("F Sharp Minor"));
    TTT_EXPECT (mica::map (key, mica::KeySignature).toString() == std::string ("Three Sharps"));
    
    mode = mica::Concept ("abcdefghijklmnop");
    key  = mica::map (tonic, mode);
    
    TTT_EXPECT (key == mica::Undefined);
    TTT_EXPECT (mica::map (key, mica::KeySignature) == mica::Undefined);
    //
    }
    
    {
    //
    mica::Concept keySignature = mica::FiveSharps;
    mica::Concept accidental = mica::map (keySignature, mica::Accidental);
    mica::Concept clef = mica::map (mica::TrebleClef, accidental);
    
    TTT_EXPECT (keySignature.length() == 5);
    TTT_EXPECT (accidental == mica::Sharp);
    TTT_EXPECT (clef == mica::TrebleClefSharps);

    TTT_EXPECT (mica::item (clef, 0).getNumerator() == 4);
    TTT_EXPECT (mica::item (clef, 1).getNumerator() == 1);
    TTT_EXPECT (mica::item (clef, 2).getNumerator() == 5);
    TTT_EXPECT (mica::item (clef, 3).getNumerator() == 2);
    TTT_EXPECT (mica::item (clef, 4).getNumerator() == -1);
    //
    }
    
    {
    //
    TTT_EXPECT (mica::Concept ("Key Signatures").length() == 15);
    
    TTT_EXPECT (mica::item (mica::KeySignatures, 0)  == mica::SevenFlats);
    TTT_EXPECT (mica::item (mica::KeySignatures, 7)  == mica::NoAccidentals);
    TTT_EXPECT (mica::item (mica::KeySignatures, 14) == mica::SevenSharps);
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
void test153__MIDI() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaMIDI, 153, "MICA - MIDI")

    for (int i = 0; i < 128; ++i) {
        mica::Concept chromatic = mica::map (i, mica::Flat);
        TTT_EXPECT (mica::map (chromatic, mica::MIDIKeyNumber).getNumerator() == i);
    }
    
    for (int i = 0; i < 128; ++i) {
        mica::Concept chromatic = mica::map (i, mica::Sharp);
        TTT_EXPECT (mica::map (chromatic, mica::MIDIKeyNumber).getNumerator() == i);
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
void test154__scales() {
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MicaScales, 154, "MICA - Scales")

    mica::Concept major = mica::map (mica::Major, mica::Steps);
    
    TTT_EXPECT (mica::map (mica::Ionian, mica::Steps) == major);
    
    TTT_EXPECT (major.length() == 7);
    
    TTT_EXPECT (mica::item (major, 0).getNumerator() == 2);
    TTT_EXPECT (mica::item (major, 1).getNumerator() == 2);
    TTT_EXPECT (mica::item (major, 2).getNumerator() == 1);
    TTT_EXPECT (mica::item (major, 3).getNumerator() == 2);
    TTT_EXPECT (mica::item (major, 4).getNumerator() == 2);
    TTT_EXPECT (mica::item (major, 5).getNumerator() == 2);
    TTT_EXPECT (mica::item (major, 6).getNumerator() == 1);
    
    TTT_EXPECT (mica::item (mica::Steps, 1) == mica::HalfStep);
    TTT_EXPECT (mica::item (mica::Steps, 2) == mica::WholeStep);
            
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if 0
}
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
