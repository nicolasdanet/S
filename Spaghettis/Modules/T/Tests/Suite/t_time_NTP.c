
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (TimeStamps, "Time - NTP")

    t_nano ns, elapsed;
    t_stamp stamp1, stamp2;

    ns = 1000000000ULL;
    
    stamp_set (&stamp1);

    stamp2 = stamp1;

    TTT_EXPECT (stamp_elapsedNanoseconds (&stamp1, &stamp2, &elapsed) != PD_ERROR_NONE);
    TTT_EXPECT (elapsed == 0);

    stamp_addNanoseconds (&stamp2, ns);

    TTT_EXPECT (stamp_elapsedNanoseconds (&stamp2, &stamp1, &elapsed) != PD_ERROR_NONE);
    TTT_EXPECT (stamp_elapsedNanoseconds (&stamp1, &stamp2, &elapsed) == PD_ERROR_NONE);
    
    TTT_EXPECT (elapsed == (ns - 1));       /* Rounding error! */

    stamp_set (&stamp1);
    nano_sleep (ns);
    stamp_set (&stamp2);

    TTT_EXPECT (stamp_elapsedNanoseconds (&stamp1, &stamp2, &elapsed) == PD_ERROR_NONE);
    
    TTT_EXPECT (PD_NANOSECONDS_TO_SECONDS (elapsed) > 0.99);
    TTT_EXPECT (PD_NANOSECONDS_TO_SECONDS (elapsed) < 1.01);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (TimeAtoms, "Time - NTP Atoms")

    t_atom atoms[STAMP_SIZE];
    
    t_stamp stamp1, stamp2;
    
    int i;
    
    for (i = 0; i < TEST_LOOP; i++) {
    //
    stamp_setRandom (&stamp1);
    stamp_setImmediately (&stamp2);
    stamp_serialize (STAMP_SIZE, atoms, &stamp1);
    stamp_deserialize (STAMP_SIZE, atoms, &stamp2);
    
    TTT_EXPECT (stamp_areEquals (&stamp1, &stamp2));
    TTT_EXPECT (stamp_isImmediately (&stamp2) == 0);
    
    stamp_setRandom (&stamp1);
    stamp_setImmediately (&stamp2);
    stamp_serialize (STAMP_SIZE, atoms, &stamp2);
    stamp_deserialize (STAMP_SIZE, atoms, &stamp1);
    
    TTT_EXPECT (stamp_areEquals (&stamp1, &stamp2));
    TTT_EXPECT (stamp_isImmediately (&stamp1) == 1);
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (TimeSerialize, "Time - NTP Serialize")

    t_buffer *b = buffer_new(); buffer_resize (b, STAMP_SIZE);
    
    t_stamp stamp1, stamp2;
    
    stamp_setRandom (&stamp1);
    stamp_setImmediately (&stamp2);
    stamp_serialize (buffer_getSize (b), buffer_getAtoms (b), &stamp1);
    ttt_stdout (TTT_COLOR_BLUE, "%s", buffer_toString (b));
    ttt_stdout (TTT_COLOR_BLUE, "%lld", (stamp1 >> 32));
    ttt_stdout (TTT_COLOR_BLUE, "%lld", (stamp1 & 0xffffffffULL));
    
    buffer_reparse (b);
    
    stamp_deserialize (buffer_getSize (b), buffer_getAtoms (b), &stamp2);
    ttt_stdout (TTT_COLOR_BLUE, "%s", buffer_toString (b));
    ttt_stdout (TTT_COLOR_BLUE, "%lld", (stamp2 >> 32));
    ttt_stdout (TTT_COLOR_BLUE, "%lld", (stamp2 & 0xffffffffULL));
    
    TTT_EXPECT (stamp_areEquals (&stamp1, &stamp2));
    TTT_EXPECT (stamp_isImmediately (&stamp2) == 0);
    
    buffer_free (b);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
