
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (Time, "Time - Time")

    t_nano ns, elapsed;
    t_time time1, time2;

    ns = 1000000000ULL;
    
    time_set (&time1);
    
    time2 = time1;
    
    TTT_EXPECT (time_elapsedNanoseconds (&time1, &time2, &elapsed) != PD_ERROR_NONE);
    TTT_EXPECT (elapsed == 0);

    time_addNanoseconds (&time2, ns);

    TTT_EXPECT (time_elapsedNanoseconds (&time2, &time1, &elapsed) != PD_ERROR_NONE);
    TTT_EXPECT (time_elapsedNanoseconds (&time1, &time2, &elapsed) == PD_ERROR_NONE);
    TTT_EXPECT (elapsed == ns);

    time_set (&time1);
    nano_sleep (ns);
    time_set (&time2);

    TTT_EXPECT (time_elapsedNanoseconds (&time1, &time2, &elapsed) == PD_ERROR_NONE);
    
    TTT_EXPECT (PD_NANOSECONDS_TO_SECONDS (elapsed) > 0.99);
    TTT_EXPECT (PD_NANOSECONDS_TO_SECONDS (elapsed) < 1.01);

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
