
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (Soundfile, "Soundfile - Define")

    TTT_EXPECT (SOUNDFILE_HELPER_SIZE > 16);
    TTT_EXPECT (SOUNDFILE_HELPER_SIZE > SOUNDFILE_HEADER_WAVE);
    TTT_EXPECT (SOUNDFILE_HELPER_SIZE > SOUNDFILE_HEADER_AIFF);
    TTT_EXPECT (SOUNDFILE_HELPER_SIZE > SOUNDFILE_HEADER_NEXT);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
