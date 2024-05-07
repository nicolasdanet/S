
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
