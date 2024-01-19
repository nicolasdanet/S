
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_soundfile.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define SOUNDFILE_CHUNK_HEADER      12
#define SOUNDFILE_SUBCHUNK_HEADER   8

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error subchunk_traverseStart (t_headerhelper *t)
{
    if (t->h_bytesSet <= SOUNDFILE_CHUNK_HEADER) { return PD_ERROR; }
    else {
        t->h_onset     = 4;
        t->h_chunkSize = 0;
    }
    
    return PD_ERROR_NONE; 
}

static t_error subchunk_traverseNext (int f, t_headerhelper *t, t_audioproperties *args)
{
    t_error err = PD_ERROR_NONE;
    
    int next = t->h_onset + t->h_chunkSize + SOUNDFILE_SUBCHUNK_HEADER;

    if (!(err = soundfile_helperRead (f, t, (off_t)next))) {
    //
    if (t->h_bytesSet < SOUNDFILE_SUBCHUNK_HEADER) { err = PD_ERROR; }
    else {
    //
    int swap = args->ap_needToSwap;

    t->h_onset = next;
    
    t->h_ID[0] = t->h_c[0];
    t->h_ID[1] = t->h_c[1];
    t->h_ID[2] = t->h_c[2];
    t->h_ID[3] = t->h_c[3];
    t->h_ID[4] = 0;
    
    t->h_chunkSize = (int)soundfile_swap4Pointer (t->h_c + 4, swap);
    //
    }
    //
    }
    
    return err;
}

static char *subchunk_getID (t_headerhelper *t)
{
    return t->h_ID;
}

static int subchunk_getSize (t_headerhelper *t)
{
    return t->h_chunkSize;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* AIFF minimum subchunks required. */

static int subchunk_isCOMM (t_headerhelper *t)
{
    if (!strncmp (subchunk_getID (t), "COMM", 4)) {
    if (t->h_chunkSize == 18) {
    if (t->h_bytesSet  >= 18 + SOUNDFILE_SUBCHUNK_HEADER) {
        return 1;
    }
    }
    }
    
    return 0;
}

static int subchunk_isSSND (t_headerhelper *t)
{
    if (!strncmp (subchunk_getID (t), "SSND", 4)) {
    if (t->h_chunkSize >= 8) {
    if (t->h_bytesSet  >= 8 + SOUNDFILE_SUBCHUNK_HEADER) {
        return 1;
    }
    }
    }
    
    return 0;
}

static int subchunk_parseCOMM (t_headerhelper *t, t_audioproperties *args)
{
    char *p = t->h_c + SOUNDFILE_SUBCHUNK_HEADER;
        
    int numberOfChannels = (int)soundfile_swap2Pointer (p + 0, args->ap_needToSwap);
    int bitsPerSample    = (int)soundfile_swap2Pointer (p + 6, args->ap_needToSwap);
    double sampleRate    = soundfile_getAiff80BitFloat (p + 8);
    
    args->ap_sampleRate       = sampleRate;
    args->ap_bytesPerSample   = bitsPerSample / 8;
    args->ap_numberOfChannels = numberOfChannels;
    
    return 18 + SOUNDFILE_SUBCHUNK_HEADER;
}

static int subchunk_parseSSND (t_headerhelper *t, t_audioproperties *args)
{
    char *p = t->h_c + SOUNDFILE_SUBCHUNK_HEADER;
    
    int offset      = (int)soundfile_swap4Pointer (p + 0, args->ap_needToSwap);
    int blockAlign  = (int)soundfile_swap4Pointer (p + 4, args->ap_needToSwap);
    
    PD_ASSERT (offset == 0);        /* Not tested nor implemented for now. */
    PD_ASSERT (blockAlign == 0);    /* Ditto. */
    PD_UNUSED (offset);
    PD_UNUSED (blockAlign);
    
    args->ap_dataSizeInBytes = t->h_chunkSize - SOUNDFILE_SUBCHUNK_HEADER;
    
    return 8 + SOUNDFILE_SUBCHUNK_HEADER;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* WAVE minimum subchunks required. */

static int subchunk_isFMT (t_headerhelper *t)
{
    if (!strncmp (subchunk_getID (t), "fmt ", 4)) {
    if (t->h_chunkSize == 16) {
    if (t->h_bytesSet  >= 16 + SOUNDFILE_SUBCHUNK_HEADER) {
        return 1;
    }
    }
    }
    
    return 0;
}

static int subchunk_isDATA (t_headerhelper *t)
{
    if (!strncmp (subchunk_getID (t), "data", 4)) {
    if (t->h_chunkSize >= 0) {
    if (t->h_bytesSet  >= 0 + SOUNDFILE_SUBCHUNK_HEADER) {
        return 1;
    }
    }
    }
    
    return 0;
}

static int subchunk_parseFMT (t_headerhelper *t, t_audioproperties *args)
{
    char *p = t->h_c + SOUNDFILE_SUBCHUNK_HEADER;
    
    int audioFormat      = (int)soundfile_swap2Pointer (p + 0,  args->ap_needToSwap);
    int numberOfChannels = (int)soundfile_swap2Pointer (p + 2,  args->ap_needToSwap);
    int sampleRate       = (int)soundfile_swap4Pointer (p + 4,  args->ap_needToSwap);
    int bitsPerSample    = (int)soundfile_swap2Pointer (p + 14, args->ap_needToSwap);
    
    args->ap_sampleRate       = sampleRate;
    args->ap_numberOfChannels = numberOfChannels;
    
    PD_ASSERT (audioFormat == WAVE_FORMAT_PCM || audioFormat == WAVE_FORMAT_FLOAT);
    
    if (audioFormat == WAVE_FORMAT_PCM || audioFormat == WAVE_FORMAT_FLOAT) {
    //
    args->ap_bytesPerSample = bitsPerSample / 8;
    //
    }
    
    return 16 + SOUNDFILE_SUBCHUNK_HEADER;
}

static int subchunk_parseDATA (t_headerhelper *t, t_audioproperties *args)
{
    args->ap_dataSizeInBytes = t->h_chunkSize;
    
    return SOUNDFILE_SUBCHUNK_HEADER;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error subchunk_readFileHeaderWAVE (int f, t_headerhelper *t, t_audioproperties *args)
{
    t_error end = subchunk_traverseStart (t);
    t_error err = PD_ERROR_NONE;
    
    int hasFMT  = 0;
    int hasDATA = 0;
    int headerSize = SOUNDFILE_CHUNK_HEADER;
    
    while (!end && !err) {
    //
    if (!(end = subchunk_traverseNext (f, t, args))) {
    //
    if (subchunk_isFMT (t)) {
        if (hasFMT) { err = PD_ERROR; }
        else {
            headerSize += subchunk_parseFMT (t, args);
            hasFMT = 1;
        }
        
    } else if (subchunk_isDATA (t)) {
        if (hasDATA) { err = PD_ERROR; }
        else {
            headerSize += subchunk_parseDATA (t, args);
            hasDATA = 1;
        }
        
    } else if (!hasDATA) { headerSize += SOUNDFILE_SUBCHUNK_HEADER + subchunk_getSize (t); }
    //
    }
    //
    }
    
    err |= (hasFMT == 0);
    err |= (hasDATA == 0);
    err |= (args->ap_bytesPerSample < 2);
    err |= (args->ap_bytesPerSample > 4);
    
    args->ap_headerSize = headerSize;
    
    return err;
}

t_error subchunk_readFileHeaderAIFF (int f, t_headerhelper *t, t_audioproperties *args)
{
    t_error end = subchunk_traverseStart (t);
    t_error err = PD_ERROR_NONE;
    
    int hasCOMM = 0;
    int hasSSND = 0;
    int headerSize = SOUNDFILE_CHUNK_HEADER;
    
    while (!end && !err) {
    //
    if (!(end = subchunk_traverseNext (f, t, args))) {
    //
    if (subchunk_isCOMM (t)) {
        if (hasCOMM) { err = PD_ERROR; }
        else {
            headerSize += subchunk_parseCOMM (t, args);
            hasCOMM = 1;
        }
        
    } else if (subchunk_isSSND (t)) {
        if (hasSSND) { err = PD_ERROR; }
        else {
            headerSize += subchunk_parseSSND (t, args);
            hasSSND = 1;
        }
        
    } else if (!hasSSND) { headerSize += SOUNDFILE_SUBCHUNK_HEADER + subchunk_getSize (t); }
    //
    }
    //
    }
    
    err |= (hasCOMM == 0);
    err |= (hasSSND == 0);
    err |= (args->ap_bytesPerSample < 2);
    err |= (args->ap_bytesPerSample > 4);
    
    args->ap_headerSize = headerSize;
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
