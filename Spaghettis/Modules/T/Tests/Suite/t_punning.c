
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (MemoryPunning, "Memory - Punning")
    
    {
        t_pun32 z; pun32_setFloat (&z, 3.14159274101);
    
        TTT_EXPECT (pun32_getInteger (&z) == 0x40490fdb);
    }
    
    {
        t_pun64 z;
            
        pun64_setDouble (&z, DSP_UNITBIT + 0.5);
        TTT_EXPECT ((pun64_getLeastSignificantBytes (&z) == 0x80000000));
        TTT_EXPECT ((pun64_getMostSignificantBytes (&z) == DSP_UNITBIT_MSB));
        
        pun64_setDouble (&z, COSINE_UNITBIT);
        TTT_EXPECT ((pun64_getMostSignificantBytes (&z) == COSINE_UNITBIT_MSB));
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
