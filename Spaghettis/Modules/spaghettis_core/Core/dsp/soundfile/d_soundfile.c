
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

/* Basic audio files handling. */
/* Uncompressed 16-bit, 24-bit integer and 32-bit float. */
/* Mu-law, A-law formats and such NOT handled. */
/* Note that unsupported sub-chunks are not preserved at save. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Mu-law > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define SOUNDFILE_HEADER_WAVE           44
#define SOUNDFILE_HEADER_AIFF           54
#define SOUNDFILE_HEADER_NEXT           28

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __cplusplus )

static_assert (SOUNDFILE_HELPER_SIZE > 16);
static_assert (SOUNDFILE_HELPER_SIZE > SOUNDFILE_HEADER_WAVE);
static_assert (SOUNDFILE_HELPER_SIZE > SOUNDFILE_HEADER_AIFF);
static_assert (SOUNDFILE_HELPER_SIZE > SOUNDFILE_HEADER_NEXT);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error subchunk_readFileHeaderWAVE (int, t_headerhelper *, t_audioproperties *);
t_error subchunk_readFileHeaderAIFF (int, t_headerhelper *, t_audioproperties *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error soundfile_readFileParse (t_glist *dummy,
    t_symbol *s,
    int *ac,
    t_atom **av,
    t_audioproperties *args,
    t_object *owner)
{
    t_error err = PD_ERROR_NONE;
    
    int argc                = *ac;
    t_atom *argv            = *av;
    t_symbol *fileName      = &s_;
    t_symbol *fileExtension = &s_;
    int needToResize        = 0;
    int onset               = 0;
    int numberOfFrames      = SOUNDFILE_UNKNOWN;
    
    (void)dummy;
    
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (t == sym___arrobe__nextstep) { t = sym___arrobe__next; }
    
    if (argc > 1 && t == sym___arrobe__skip) {
        onset = (int)atom_getFloat (argv + 1);
        onset = PD_MAX (0, onset);
        argc -= 2; argv += 2;
        
    } else if (argc > 1 && t == sym___arrobe__frames) {
        numberOfFrames = (int)atom_getFloat (argv + 1);
        numberOfFrames = PD_MAX (0, numberOfFrames);
        needToResize = 1;     
        argc -= 2; argv += 2;
    
    } else if (t == sym___arrobe__resize)   {
        needToResize = 1;
        argc--; argv++;
        
    } else if (t == sym___arrobe__next)     {
        fileExtension = sym___point__snd;
        argc--; argv++;
        
    } else if (t == sym___arrobe__wave)     {
        fileExtension = sym___point__wav;
        argc--; argv++;
        
    } else if (t == sym___arrobe__aiff)     {
        fileExtension = sym___point__aiff;
        argc--; argv++;
    
    } else if (t == sym___arrobe__aif)      {
        fileExtension = sym___point__aif;
        argc--; argv++;
        
    } else { break; }
    //
    }
    
    if (!err) { err = (error__options (owner, s, argc, argv) != 0); }
    if (!err) { err = (!argc || !IS_SYMBOL (argv)); }
    
    if (!err) {
    //
    fileName = GET_SYMBOL (argv); 
    
    argc--; argv++;
    
    args->ap_fileName       = fileName;
    args->ap_fileExtension  = fileExtension;
    args->ap_onset          = onset;
    args->ap_numberOfFrames = numberOfFrames;
    args->ap_needToResize   = needToResize;
    
    *ac = argc;
    *av = argv;
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error soundfile_readFileHeaderCanonicalWAVE (int f, t_headerhelper *t, t_audioproperties *args)
{
    t_error err = PD_ERROR;

    if (t->h_bytesSet > SOUNDFILE_HEADER_WAVE) {
    //
    if (!strncmp (t->h_c + 12, "fmt ", 4)) {
    if (!strncmp (t->h_c + 36, "data", 4)) {
    //
    int swap = args->ap_needToSwap;
    
    int fmtSize          = (int)soundfile_swap4Pointer (t->h_c + 16, swap);
    int audioFormat      = (int)soundfile_swap2Pointer (t->h_c + 20, swap);
    int numberOfChannels = (int)soundfile_swap2Pointer (t->h_c + 22, swap);
    int sampleRate       = (int)soundfile_swap4Pointer (t->h_c + 24, swap);
    int bitsPerSample    = (int)soundfile_swap2Pointer (t->h_c + 34, swap);
    int dataSize         = (int)soundfile_swap4Pointer (t->h_c + 40, swap);
    
    PD_ASSERT (fmtSize == 16);
    PD_ASSERT (audioFormat == WAVE_FORMAT_PCM || audioFormat == WAVE_FORMAT_FLOAT);
    PD_UNUSED (fmtSize);
    
    if (audioFormat == WAVE_FORMAT_PCM || audioFormat == WAVE_FORMAT_FLOAT) {
    //
    if (bitsPerSample == 16 || bitsPerSample == 24 || bitsPerSample == 32) {
    //
    args->ap_sampleRate        = sampleRate;
    args->ap_headerSize        = SOUNDFILE_HEADER_WAVE;
    args->ap_bytesPerSample    = bitsPerSample / 8;
    args->ap_numberOfChannels  = numberOfChannels;
    args->ap_dataSizeInBytes   = dataSize;
    
    err = PD_ERROR_NONE;
    //
    }
    //
    }
    //
    }
    }
    //
    }
    
    return err;
}

static t_error soundfile_readFileHeaderCanonicalAIFF (int f, t_headerhelper *t, t_audioproperties *args)
{
    t_error err = PD_ERROR;
    
    if (t->h_bytesSet > SOUNDFILE_HEADER_AIFF) {
    //
    if (!strncmp (t->h_c + 12, "COMM", 4)) {
    if (!strncmp (t->h_c + 38, "SSND", 4)) {
    //
    int swap = args->ap_needToSwap;
    
    int numberOfChannels = (int)soundfile_swap2Pointer (t->h_c + 20, swap);
    int bitsPerSample    = (int)soundfile_swap2Pointer (t->h_c + 26, swap);
    int chunkSize        = (int)soundfile_swap4Pointer (t->h_c + 42, swap);
    int offset           = (int)soundfile_swap4Pointer (t->h_c + 46, swap);
    int blockAlign       = (int)soundfile_swap4Pointer (t->h_c + 50, swap);
    
    double sampleRate    = soundfile_getAiff80BitFloat (t->h_c + 28);
    
    PD_ASSERT (offset == 0);        /* Not tested nor implemented for now. */
    PD_ASSERT (blockAlign == 0);    /* Ditto. */
    PD_UNUSED (offset);
    PD_UNUSED (blockAlign);
    
    if (bitsPerSample == 16 || bitsPerSample == 24 || bitsPerSample == 32) {
    //
    args->ap_sampleRate        = sampleRate;
    args->ap_headerSize        = SOUNDFILE_HEADER_AIFF;
    args->ap_bytesPerSample    = bitsPerSample / 8;
    args->ap_numberOfChannels  = numberOfChannels;
    args->ap_dataSizeInBytes   = chunkSize - 8;
    
    err = PD_ERROR_NONE;
    //
    }
    //
    }
    }
    //
    }
    
    return err;
}

static t_error soundfile_readFileHeaderCanonicalNEXT (int f, t_headerhelper *t, t_audioproperties *args)
{
    t_error err = PD_ERROR;
    
    if (t->h_bytesSet > SOUNDFILE_HEADER_NEXT) {
    //
    int swap = args->ap_needToSwap;
    
    int dataLocation     = (int)soundfile_swap4Pointer (t->h_c + 4,  swap);
    int dataSize         = (int)soundfile_swap4Pointer (t->h_c + 8,  swap);
    int audioFormat      = (int)soundfile_swap4Pointer (t->h_c + 12, swap);
    int sampleRate       = (int)soundfile_swap4Pointer (t->h_c + 16, swap);
    int numberOfChannels = (int)soundfile_swap4Pointer (t->h_c + 20, swap);
    int bytesPerSample   = 0;
    
    if (audioFormat == NS_FORMAT_LINEAR_16)      { bytesPerSample = 2; }
    else if (audioFormat == NS_FORMAT_LINEAR_24) { bytesPerSample = 3; }
    else if (audioFormat == NS_FORMAT_FLOAT)     { bytesPerSample = 4; }
            
    if (bytesPerSample) {
    //
    args->ap_sampleRate        = sampleRate;
    args->ap_headerSize        = dataLocation;
    args->ap_bytesPerSample    = bytesPerSample;
    args->ap_numberOfChannels  = numberOfChannels;
    args->ap_dataSizeInBytes   = dataSize;
    
    err = PD_ERROR_NONE;
    //
    }
    //
    }
    
    return err;
}

static t_error soundfile_readFileHeaderFormat (int f, t_audioproperties *args)
{
    t_error err = PD_ERROR;
    
    t_headerhelper t; SOUNDFILE_HELPER_INIT (&t); 
    
    soundfile_helperRead (f, &t, 0);
    
    if (t.h_bytesSet >= 4) {
    //
    int format = SOUNDFILE_UNDEFINED;
    
    if (!strncmp (t.h_c,         ".snd", 4)) { format = SOUNDFILE_NEXT; args->ap_isBigEndian = 1; }
    else if (!strncmp (t.h_c,    "dns.", 4)) { format = SOUNDFILE_NEXT; args->ap_isBigEndian = 0; }
    else if (t.h_bytesSet >= 12) {
    //
    if (!strncmp (t.h_c,         "RIFF", 4)) {
        if (!strncmp (t.h_c + 8, "WAVE", 4)) { format = SOUNDFILE_WAVE; args->ap_isBigEndian = 0; }
    } else if (!strncmp (t.h_c,  "FORM", 4)) {
        if (!strncmp (t.h_c + 8, "AIFF", 4)) { format = SOUNDFILE_AIFF; args->ap_isBigEndian = 1; }
    }
    //
    }
    
    if (format != SOUNDFILE_UNDEFINED) {
    
        args->ap_fileType   = format;
        args->ap_needToSwap = (args->ap_isBigEndian != soundfile_systemIsBigEndian());
        
        if (format == SOUNDFILE_WAVE) {
            if ((err = soundfile_readFileHeaderCanonicalWAVE (f, &t, args))) {
                err = subchunk_readFileHeaderWAVE (f, &t, args);
            }
        }
        
        if (format == SOUNDFILE_AIFF) {
            if ((err = soundfile_readFileHeaderCanonicalAIFF (f, &t, args))) {
                err = subchunk_readFileHeaderAIFF (f, &t, args);
            }
        }
            
        if (format == SOUNDFILE_NEXT) { err = soundfile_readFileHeaderCanonicalNEXT (f, &t, args); }
    }
    //
    }

    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int soundfile_readFileHeaderProceed (int f, t_audioproperties *args)
{
    t_error err = soundfile_readFileHeaderFormat (f, args);
    
    if (!err) {
    //
    PD_ASSERT (args->ap_sampleRate          != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_fileType            != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_headerSize          != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_numberOfChannels    != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_bytesPerSample      != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_isBigEndian         != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_dataSizeInBytes     != SOUNDFILE_UNDEFINED);
        
    err |= (args->ap_dataSizeInBytes < 0);
    err |= (args->ap_dataSizeInBytes + args->ap_headerSize > lseek (f, 0, SEEK_END));
    //
    }
    
    if (!err) {
    //
    int m = args->ap_numberOfChannels * args->ap_bytesPerSample * args->ap_onset;
    off_t n = args->ap_headerSize + m;

    err = ((lseek (f, n, SEEK_SET)) != n);
        
    if (!err) {
        args->ap_dataSizeInBytes = PD_MAX (0, args->ap_dataSizeInBytes - m);
    }
    //
    }
    
    if (!err) { return f; } else { close (f); return -1; }
}

/* Caller is responsible to close the file. */

int soundfile_readFileHeader (t_glist *glist, t_audioproperties *args)
{
    t_fileproperties p; fileproperties_init (&p);
    
    PD_ASSERT (args->ap_fileName);
    PD_ASSERT (args->ap_fileExtension);
    
    int f = glist_fileOpen (glist,
                symbol_getName (args->ap_fileName),
                symbol_getName (args->ap_fileExtension),
                &p);
    
    if (f >= 0) { return soundfile_readFileHeaderProceed (f, args); }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error soundfile_writeFileParse (t_glist *glist,
    t_symbol *s,
    int *ac,
    t_atom **av,
    t_audioproperties *args,
    t_object *owner)
{
    t_error err = PD_ERROR_NONE;
    
    int argc                = *ac;
    t_atom *argv            = *av;
    t_symbol *fileName      = &s_;
    t_symbol *fileExtension = &s_;
    t_float sampleRate      = glist ? canvas_getSampleRate (glist) : audio_getSampleRate();
    int fileType            = SOUNDFILE_UNDEFINED;
    int bytesPerSample      = 2;
    int isBigEndian         = 0;
    int needToSwap          = 0;
    int onset               = 0;
    int numberOfFrames      = SOUNDFILE_UNKNOWN;
    int needToNormalize     = 0;
    
    int endianness = 1;     /* Default is big-endian (used only by NeXT/Sun soundfile format). */
        
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (t == sym___arrobe__nextstep) { t = sym___arrobe__next; }
    
    if (argc > 1 && t == sym___arrobe__skip) {
        onset = (int)atom_getFloat (argv + 1);
        onset = PD_MAX (0, onset);
        argc -= 2; argv += 2;
        
    } else if (argc > 1 && t == sym___arrobe__frames) {
        numberOfFrames = (int)atom_getFloat (argv + 1);
        numberOfFrames = PD_MAX (0, numberOfFrames);
        argc -= 2; argv += 2;
        
    } else if (argc > 1 && t == sym___arrobe__bytes) {
        bytesPerSample = (int)atom_getFloat (argv + 1);
        bytesPerSample = PD_CLAMP (bytesPerSample, 2, 4);
        argc -= 2; argv += 2;
        
    } else if (argc > 1 && t == sym___arrobe__samplerate) {
        sampleRate = atom_getFloat (argv + 1);
        sampleRate = (t_sample)PD_MAX (1.0, sampleRate);
        argc -= 2; argv += 2;
        
    } else if (t == sym___arrobe__normalize) {
        needToNormalize = 1;
        argc--; argv++;
    
    } else if (t == sym___arrobe__next)   {
        fileType = SOUNDFILE_NEXT; fileExtension = sym___point__snd;
        argc--; argv++;
        
    } else if (t == sym___arrobe__wave)   {
        fileType = SOUNDFILE_WAVE; fileExtension = sym___point__wav;
        argc--; argv++;
        
    } else if (t == sym___arrobe__aiff)   {
        fileType = SOUNDFILE_AIFF; fileExtension = sym___point__aiff;
        argc--; argv++;
    
    } else if (t == sym___arrobe__aif)    {
        fileType = SOUNDFILE_AIFF; fileExtension = sym___point__aif;
        argc--; argv++;
        
    } else if (t == sym___arrobe__big)    {
        endianness = 1;
        argc--; argv++;
        
    } else if (t == sym___arrobe__little) {
        endianness = 0;
        argc--; argv++;
        
    } else { break; }
    //
    }
    
    if (!err) { err = (error__options (owner, s, argc, argv) != 0); }
    if (!err) { err = (!argc || !IS_SYMBOL (argv)); }
    
    if (!err) {
    //
    fileName = GET_SYMBOL (argv); 
    
    argc--; argv++;
    
    if (fileType == SOUNDFILE_UNDEFINED) {

        if (string_endWith (symbol_getName (fileName), ".wav"))         { fileType = SOUNDFILE_WAVE; }
        else if (string_endWith (symbol_getName (fileName), ".WAV"))    { fileType = SOUNDFILE_WAVE; }
        else if (string_endWith (symbol_getName (fileName), ".aif"))    { fileType = SOUNDFILE_AIFF; }
        else if (string_endWith (symbol_getName (fileName), ".AIF"))    { fileType = SOUNDFILE_AIFF; }
        else if (string_endWith (symbol_getName (fileName), ".aiff"))   { fileType = SOUNDFILE_AIFF; }
        else if (string_endWith (symbol_getName (fileName), ".AIFF"))   { fileType = SOUNDFILE_AIFF; }
        else if (string_endWith (symbol_getName (fileName), ".snd"))    { fileType = SOUNDFILE_NEXT; }
        else if (string_endWith (symbol_getName (fileName), ".SND"))    { fileType = SOUNDFILE_NEXT; }
        else if (string_endWith (symbol_getName (fileName), ".au"))     { fileType = SOUNDFILE_NEXT; }
        else if (string_endWith (symbol_getName (fileName), ".AU"))     { fileType = SOUNDFILE_NEXT; }
        else {
            fileType = SOUNDFILE_WAVE; fileExtension = sym___point__wav;
        }
    }

    if (bytesPerSample == 4 && fileType == SOUNDFILE_AIFF) { PD_BUG; return PD_ERROR; }
    if (fileType == SOUNDFILE_WAVE)      { isBigEndian = 0; }
    else if (fileType == SOUNDFILE_AIFF) { isBigEndian = 1; }
    else {
        isBigEndian = endianness;
    }
    
    needToSwap = (isBigEndian != soundfile_systemIsBigEndian());
    
    args->ap_fileName           = fileName;
    args->ap_fileExtension      = fileExtension;
    args->ap_sampleRate         = sampleRate;
    args->ap_fileType           = fileType;
    args->ap_bytesPerSample     = bytesPerSample;
    args->ap_isBigEndian        = isBigEndian;
    args->ap_needToSwap         = needToSwap;
    args->ap_onset              = onset;
    args->ap_numberOfFrames     = numberOfFrames;
    args->ap_needToNormalize    = needToNormalize;
    
    *ac = argc;
    *av = argv;
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error soundfile_writeFileHeaderWAVE (t_headerhelper *t, t_audioproperties *args)
{
    int numberOfFrames = (args->ap_numberOfFrames == SOUNDFILE_UNKNOWN) ? 0 : args->ap_numberOfFrames;
    int dataSize       = args->ap_numberOfChannels * args->ap_bytesPerSample * numberOfFrames;
    int byteRate       = args->ap_numberOfChannels * args->ap_bytesPerSample * args->ap_sampleRate;
    int blockAlign     = args->ap_numberOfChannels * args->ap_bytesPerSample;
    int audioFormat    = WAVE_FORMAT_PCM;
    
    int swap = args->ap_needToSwap;
    
    t_wave h;
    
    if (args->ap_bytesPerSample == 4) { audioFormat = WAVE_FORMAT_FLOAT; }
    
    h.w_chunkSize        = soundfile_swap4Integer ((uint32_t)(SOUNDFILE_HEADER_WAVE - 8 + dataSize), swap);
    h.w_fmtChunkSize     = soundfile_swap4Integer ((uint32_t)(16), swap);
    h.w_audioFormat      = soundfile_swap2Integer ((uint16_t)audioFormat, swap);
    h.w_numberOfChannels = soundfile_swap2Integer ((uint16_t)args->ap_numberOfChannels, swap);
    h.w_samplesPerSecond = soundfile_swap4Integer ((uint32_t)args->ap_sampleRate, swap);
    h.w_bytesPerSecond   = soundfile_swap4Integer ((uint32_t)byteRate, swap);
    h.w_blockAlign       = soundfile_swap2Integer ((uint16_t)blockAlign, swap);
    h.w_bitsPerSample    = soundfile_swap2Integer ((uint16_t)(8 * args->ap_bytesPerSample), swap);
    h.w_dataChunkSize    = soundfile_swap4Integer ((uint32_t)dataSize, swap);
    
    memcpy (t->h_c + 0,  "RIFF", 4);
    memcpy (t->h_c + 4,  &h.w_chunkSize, 4);
    memcpy (t->h_c + 8,  "WAVE", 4);
    memcpy (t->h_c + 12, "fmt ", 4);
    memcpy (t->h_c + 16, &h.w_fmtChunkSize, 4);
    memcpy (t->h_c + 20, &h.w_audioFormat, 2);
    memcpy (t->h_c + 22, &h.w_numberOfChannels, 2);
    memcpy (t->h_c + 24, &h.w_samplesPerSecond, 4);
    memcpy (t->h_c + 28, &h.w_bytesPerSecond, 4);
    memcpy (t->h_c + 32, &h.w_blockAlign, 2);
    memcpy (t->h_c + 34, &h.w_bitsPerSample, 2);
    memcpy (t->h_c + 36, "data", 4);
    memcpy (t->h_c + 40, &h.w_dataChunkSize, 4);

    t->h_bytesSet = SOUNDFILE_HEADER_WAVE;
    
    args->ap_headerSize      = SOUNDFILE_HEADER_WAVE;
    args->ap_dataSizeInBytes = dataSize;

    return PD_ERROR_NONE;
}

static t_error soundfile_writeFileHeaderAIFF (t_headerhelper *t, t_audioproperties *args)
{
    int numberOfFrames = (args->ap_numberOfFrames == SOUNDFILE_UNKNOWN) ? 0 : args->ap_numberOfFrames;
    int dataSize       = args->ap_numberOfChannels * args->ap_bytesPerSample * numberOfFrames;
    
    int swap = args->ap_needToSwap;
    
    t_aiff h;
    
    h.a_chunkSize           = soundfile_swap4Integer ((uint32_t)(SOUNDFILE_HEADER_AIFF - 8 + dataSize), swap);
    h.a_commChunkSize       = soundfile_swap4Integer ((uint32_t)(18), swap);
    h.a_numberOfChannels    = soundfile_swap2Integer ((uint16_t)args->ap_numberOfChannels, swap);
    h.a_numberOfFrames      = soundfile_swap4Integer ((uint32_t)args->ap_numberOfFrames, swap);
    h.a_bitsPerSample       = soundfile_swap2Integer ((uint16_t)(8 * args->ap_bytesPerSample), swap);
    h.a_dataChunkSize       = soundfile_swap4Integer ((uint32_t)(dataSize + 8), swap);
    h.a_dataOffset          = (uint32_t)(0);
    h.a_dataBlock           = (uint32_t)(0);
    
    memcpy (t->h_c + 0,  "FORM", 4);
    memcpy (t->h_c + 4,  &h.a_chunkSize, 4);
    memcpy (t->h_c + 8,  "AIFF", 4);
    memcpy (t->h_c + 12, "COMM", 4);
    memcpy (t->h_c + 16,  &h.a_commChunkSize, 4);
    memcpy (t->h_c + 20,  &h.a_numberOfChannels, 2);
    memcpy (t->h_c + 22,  &h.a_numberOfFrames, 4);
    memcpy (t->h_c + 26,  &h.a_bitsPerSample, 2);
    soundfile_setAiff80BitFloat (args->ap_sampleRate, t->h_c + 28);
    memcpy (t->h_c + 38, "SSND", 4);
    memcpy (t->h_c + 42,  &h.a_dataChunkSize, 4);
    memcpy (t->h_c + 46,  &h.a_dataOffset, 4);
    memcpy (t->h_c + 50,  &h.a_dataBlock, 4);
    
    t->h_bytesSet = SOUNDFILE_HEADER_AIFF;

    args->ap_headerSize      = SOUNDFILE_HEADER_AIFF;
    args->ap_dataSizeInBytes = dataSize;
    
    return PD_ERROR_NONE;
}

static t_error soundfile_writeFileHeaderNEXT (t_headerhelper *t, t_audioproperties *args)
{
    int numberOfFrames = (args->ap_numberOfFrames == SOUNDFILE_UNKNOWN) ? 0 : args->ap_numberOfFrames;
    int dataSize       = args->ap_numberOfChannels * args->ap_bytesPerSample * numberOfFrames;
    int audioFormat    = NS_FORMAT_LINEAR_16;
    
    int swap = args->ap_needToSwap;
    
    t_nextstep h;

    if (args->ap_bytesPerSample == 3) { audioFormat = NS_FORMAT_LINEAR_24; }
    if (args->ap_bytesPerSample == 4) { audioFormat = NS_FORMAT_FLOAT; }
    
    h.ns_dataLocation = soundfile_swap4Integer ((uint32_t)SOUNDFILE_HEADER_NEXT, swap);
    h.ns_dataSize     = soundfile_swap4Integer ((uint32_t)dataSize, swap);
    h.ns_dataFormat   = soundfile_swap4Integer ((uint32_t)audioFormat, swap);
    h.ns_samplingRate = soundfile_swap4Integer ((uint32_t)args->ap_sampleRate, swap);
    h.ns_channelCount = soundfile_swap4Integer ((uint32_t)args->ap_numberOfChannels, swap);
    
    if (args->ap_isBigEndian) { 
        memcpy (t->h_c + 0, ".snd", 4);
    } else {
        memcpy (t->h_c + 0, "dns.", 4);
    }
    
    memcpy (t->h_c + 4,  &h.ns_dataLocation, 4);
    memcpy (t->h_c + 8,  &h.ns_dataSize, 4);
    memcpy (t->h_c + 12, &h.ns_dataFormat, 4);
    memcpy (t->h_c + 16, &h.ns_samplingRate, 4);
    memcpy (t->h_c + 20, &h.ns_channelCount, 4);
    memset (t->h_c + 24, 0, 4);
    
    t->h_bytesSet = SOUNDFILE_HEADER_NEXT;
    
    args->ap_headerSize      = SOUNDFILE_HEADER_NEXT;
    args->ap_dataSizeInBytes = dataSize;

    return PD_ERROR_NONE;
}

/* Caller is responsible to close the file. */

int soundfile_writeFileHeader (t_glist *glist, t_audioproperties *args, t_object *owner)
{
    t_error err = PD_ERROR_NONE;
    char name[PD_STRING] = { 0 };
    int f = -1;
    
    t_headerhelper t; SOUNDFILE_HELPER_INIT (&t);
    
    PD_ASSERT (args->ap_fileName != NULL);
    PD_ASSERT (args->ap_fileExtension != NULL);
    PD_ASSERT (args->ap_numberOfChannels != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_sampleRate != SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_headerSize == SOUNDFILE_UNDEFINED);
    PD_ASSERT (args->ap_dataSizeInBytes == SOUNDFILE_UNDEFINED);

    err = string_copy (name, PD_STRING, symbol_getName (args->ap_fileName));
    err |= string_add (name, PD_STRING, symbol_getName (args->ap_fileExtension));
    
    if (!err) {
        if (args->ap_fileType == SOUNDFILE_WAVE)      { err = soundfile_writeFileHeaderWAVE (&t, args); }
        else if (args->ap_fileType == SOUNDFILE_AIFF) { err = soundfile_writeFileHeaderAIFF (&t, args); }
        else if (args->ap_fileType == SOUNDFILE_NEXT) { err = soundfile_writeFileHeaderNEXT (&t, args); }
        else {
            err = PD_ERROR;
        }
    }
    
    if (!err) {
    //
    char filepath[PD_STRING] = { 0 };
    const char *directory = environment_getDirectoryAsString (glist_getEnvironment (glist));
    
    err = path_withDirectoryAndName (filepath, PD_STRING, directory, name);
    
    if (!err) {
    //
    if (!path_isFileExistAsRegularFile (filepath)) {
    //
    f = file_openWrite (filepath);
    
    if (f >= 0) { 
        if (write (f, t.h_c, t.h_bytesSet) < t.h_bytesSet) { close (f); f = -1; PD_BUG; }
    }
    //
    } else { error_alreadyExists (owner, symbol_addSuffix (args->ap_fileName, args->ap_fileExtension)); }
    //
    }
    //
    }
    
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error soundfile_writeFileCloseWAVE (int f, t_audioproperties *args)
{
    t_error err  = PD_ERROR;
    int dataSize = args->ap_numberOfFrames * args->ap_bytesPerSample * args->ap_numberOfChannels;
    int swap     = args->ap_needToSwap;
    uint32_t t;
    
    args->ap_dataSizeInBytes = dataSize;
    
    if (lseek (f, 4, SEEK_SET) == 4)  { 
        t = soundfile_swap4Integer ((uint32_t)(SOUNDFILE_HEADER_WAVE - 8 + dataSize), swap);
        if (write (f, (char *)(&t), 4) == 4) {
            if (lseek (f, 40, SEEK_SET) == 40) {
                t = soundfile_swap4Integer ((uint32_t)dataSize, swap);
                if (write (f, (char *)(&t), 4) == 4) { err = PD_ERROR_NONE; }
            }
        }
    }
    
    return err;
}

static t_error soundfile_writeFileCloseAIFF (int f, t_audioproperties *args)
{
    t_error err  = PD_ERROR;
    int dataSize = args->ap_numberOfFrames * args->ap_bytesPerSample * args->ap_numberOfChannels;
    int frames   = args->ap_numberOfFrames;
    int swap     = args->ap_needToSwap;
    uint32_t t;
    
    args->ap_dataSizeInBytes = dataSize;
    
    if (lseek (f, 4, SEEK_SET) == 4)  { 
        t = soundfile_swap4Integer ((uint32_t)(SOUNDFILE_HEADER_AIFF - 8 + dataSize), swap);
        if (write (f, (char *)(&t), 4) == 4) {
            if (lseek (f, 22, SEEK_SET) == 22) {
                t = soundfile_swap4Integer ((uint32_t)frames, swap);
                if (write (f, (char *)(&t), 4) == 4) {
                    if (lseek (f, 42, SEEK_SET) == 42) {
                        t = soundfile_swap4Integer ((uint32_t)(dataSize + 8), swap);
                        if (write (f, (char *)(&t), 4) == 4) { err = PD_ERROR_NONE; }
                    }
                }
            }
        }
    }
    
    return err;
}

static t_error soundfile_writeFileCloseNEXT (int f, t_audioproperties *args)
{
    t_error err  = PD_ERROR;
    int dataSize = args->ap_numberOfFrames * args->ap_bytesPerSample * args->ap_numberOfChannels;
    int swap     = args->ap_needToSwap;
    uint32_t t;
    
    args->ap_dataSizeInBytes = dataSize;
    
    if (lseek (f, 8, SEEK_SET) == 8)  { 
        t = soundfile_swap4Integer ((uint32_t)dataSize, swap);
        if (write (f, (char *)(&t), 4) == 4) { err = PD_ERROR_NONE; }
    }
    
    return err;
}

/* Report the proper data size if necessary. */

t_error soundfile_writeFileClose (int f, int items, t_audioproperties *args)
{
    t_error err = PD_ERROR_NONE;
    
    if ((args->ap_numberOfFrames == SOUNDFILE_UNKNOWN) || (items != args->ap_numberOfFrames)) {
    //
    args->ap_numberOfFrames = items;
    
    if (args->ap_fileType == SOUNDFILE_WAVE)      { err = soundfile_writeFileCloseWAVE (f, args); }
    else if (args->ap_fileType == SOUNDFILE_AIFF) { err = soundfile_writeFileCloseAIFF (f, args); }
    else if (args->ap_fileType == SOUNDFILE_NEXT) { err = soundfile_writeFileCloseNEXT (f, args); }
    else {
        err = PD_ERROR;
    }
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
