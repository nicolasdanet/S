
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN_ALLOW (BenchmarkCosine, "Benchmark - Cosine")

    double benchmark_dummy = 0.0;
    
    int i;
    
    t_rand48 seed; PD_RAND48_INIT (seed);

    /* Claude Heiland-Allen's polynomial approximation. */
    
    ttt_timeTrigger();
    
        for (i = 0; i < TEST_LOOP_BENCHMARK; i++) {
        //
        benchmark_dummy += cosf9 ((float)(PD_RAND48_DOUBLE (seed)));
        //
        }
    
        TTT_DO_NOT_OPTIMIZE (benchmark_dummy);
    
    double t1 = ttt_timeTrigger();
    
    /* JUCE's Padé approximant. */
    
    ttt_timeTrigger();
    
        for (i = 0; i < TEST_LOOP_BENCHMARK; i++) {
        //
        benchmark_dummy += cosApproximant ((float)(PD_RAND48_DOUBLE (seed) * PD_PI));
        //
        }
        
        TTT_DO_NOT_OPTIMIZE (benchmark_dummy);
    
    double t2 = ttt_timeTrigger();
    
    /* Miller Puckette's LUT. */
    
    ttt_timeTrigger();
    
        for (i = 0; i < TEST_LOOP_BENCHMARK; i++) {
        //
        benchmark_dummy += dsp_getCosineAtLUT ((double)(PD_RAND48_DOUBLE (seed) * COSINE_TABLE_SIZE));
        //
        }
        
        TTT_DO_NOT_OPTIMIZE (benchmark_dummy);
    
    double t3 = ttt_timeTrigger();
    
    /* Standard function. */
    
    ttt_timeTrigger();
    
        for (i = 0; i < TEST_LOOP_BENCHMARK; i++) {
        //
        benchmark_dummy += cosf ((float)(PD_RAND48_DOUBLE (seed) * PD_TWO_PI));
        //
        }
        
        TTT_DO_NOT_OPTIMIZE (benchmark_dummy);
    
    double t4 = ttt_timeTrigger();
    
    // ttt_stdout (TTT_COLOR_BLUE, "COS: %f", t1);
    // ttt_stdout (TTT_COLOR_BLUE, "PAD: %f", t2);
    // ttt_stdout (TTT_COLOR_BLUE, "LUT: %f", t3);
    // ttt_stdout (TTT_COLOR_BLUE, "STD: %f", t4);
    
    /* Pure Data's LUT is the best? */
    
    TTT_EXPECT (t3 < t1);
    // TTT_EXPECT (t3 < t2);
    TTT_EXPECT (t3 < t4);

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
