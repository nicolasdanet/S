
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_SOUNDFILE_H_
#define D_SOUNDFILE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* The WAVE header. */ 
/* All WAVE files are little-endian. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://soundfile.sapp.org/doc/WaveFormat/ > */
/* < http://tiny.systems/software/soundProgrammer/WavFormatDocs.pdf > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _wave {
    char            w_fileID[4];
    uint32_t        w_chunkSize;
    char            w_waveID[4];
    char            w_fmtID[4];
    uint32_t        w_fmtChunkSize;
    uint16_t        w_audioFormat;
    uint16_t        w_numberOfChannels;
    uint32_t        w_samplesPerSecond;
    uint32_t        w_bytesPerSecond;
    uint16_t        w_blockAlign;
    uint16_t        w_bitsPerSample;
    char            w_dataChunkID[4];
    uint32_t        w_dataChunkSize;
    } t_wave;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The AIFF header. */
/* All AIFF files are big-endian. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://muratnkonar.com/aiff/index.html > */
/* < http://paulbourke.net/dataformats/audio/ > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _aiff {
    char            a_fileID[4];
    uint32_t        a_chunkSize;
    char            a_aiffID[4];
    char            a_commID[4];
    uint32_t        a_commChunkSize;
    uint16_t        a_numberOfChannels;
    uint32_t        a_numberOfFrames;
    uint16_t        a_bitsPerSample;
    unsigned char   a_sampleRate[10];
    char            a_dataChunkID[4];
    uint32_t        a_dataChunkSize;
    uint32_t        a_dataOffset;
    uint32_t        a_dataBlock;
    } t_aiff;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The NeXTStep sound header. */ 
/* Can be big-endian or little-endian. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://soundfile.sapp.org/doc/NextFormat/ > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _nextstep {
    char            ns_magic[4];
    uint32_t        ns_dataLocation;
    uint32_t        ns_dataSize;
    uint32_t        ns_dataFormat;
    uint32_t        ns_samplingRate;
    uint32_t        ns_channelCount;
    char            ns_info[4];
    } t_nextstep;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define WAVE_FORMAT_PCM             1
#define WAVE_FORMAT_FLOAT           3
//#define WAVE_FORMAT_ALAW          6
//#define WAVE_FORMAT_MULAW         7

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define NS_FORMAT_LINEAR_16         3
#define NS_FORMAT_LINEAR_24         4
#define NS_FORMAT_FLOAT             6

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SOUNDFILE_UNKNOWN           PD_INT_MAX
#define SOUNDFILE_UNDEFINED         -1

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SOUNDFILE_WAVE              0
#define SOUNDFILE_AIFF              1
#define SOUNDFILE_NEXT              2

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int soundfile_systemIsBigEndian (void)
{
    #if PD_LITTLE_ENDIAN
    
        return 0;
        
    #else
    
        return 1;
        
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline uint32_t soundfile_swap4Integer (uint32_t n, int needToSwap)
{
    if (!needToSwap) { return n; }
    else {
        return (((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24));
    }
}

static inline uint16_t soundfile_swap2Integer (uint16_t n, int needToSwap)
{
    if (!needToSwap) { return n; }
    else {
        return (((n & 0xff) << 8) | ((n & 0xff00) >> 8));
    }
}

static inline uint32_t soundfile_swap4Pointer (char *c, int needToSwap)
{
    uint32_t n; memcpy ((char *)&n, c, 4); return soundfile_swap4Integer (n, needToSwap);
}

static inline uint16_t soundfile_swap2Pointer (char *c, int needToSwap)
{
    uint16_t n; memcpy ((char *)&n, c, 2); return soundfile_swap2Integer (n, needToSwap);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define UnsignedToFloat(u) (((double)((long)(u - 2147483647L - 1))) + 2147483648.0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void soundfile_setAiff80BitFloat (double sampleRate, char *bytes)
{
    int e;
    unsigned int m = (unsigned int)ldexp (frexp (sampleRate, &e), 32);
    
    bytes[0] = (e + 16382) >> 8;
    bytes[1] = (e + 16382);
    bytes[2] = m >> 24;
    bytes[3] = m >> 16;
    bytes[4] = m >> 8;
    bytes[5] = m;
    bytes[6] = 0;
    bytes[7] = 0;
    bytes[8] = 0;
    bytes[9] = 0;
}

/* < http://www.onicos.com/staff/iz/formats/ieee.c > */

static inline double soundfile_getAiff80BitFloat (char *bytes)
{
    double f;
    int e;
    unsigned long hi, lo;
    
    e  = ((bytes[0] & 0x7F) << 8) | (bytes[1] & 0xFF);
    
    hi = ((unsigned long)(bytes[2] & 0xFF) << 24)
            | ((unsigned long)(bytes[3] & 0xFF) << 16)
            | ((unsigned long)(bytes[4] & 0xFF) << 8)
            | ((unsigned long)(bytes[5] & 0xFF));
    lo = ((unsigned long)(bytes[6] & 0xFF) << 24)
            | ((unsigned long)(bytes[7] & 0xFF) << 16)
            | ((unsigned long)(bytes[8] & 0xFF) << 8)
            | ((unsigned long)(bytes[9] & 0xFF));

    if (e == 0 && hi == 0 && lo == 0) { f = 0; }
    else {
        if (e == 0x7FFF) { f = HUGE_VAL; }      /* Infinity or NaN. */
        else {
            e -= 16383;
            e -= 31; f  = ldexp (UnsignedToFloat (hi), e);
            e -= 32; f += ldexp (UnsignedToFloat (lo), e);
        }
    }

    if (bytes[0] & 0x80) { return -f; }
    else {
        return f;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _audioproperties {
    t_symbol        *ap_fileName;
    t_symbol        *ap_fileExtension;
    t_float         ap_sampleRate;
    int             ap_fileType;
    int             ap_headerSize;
    int             ap_numberOfChannels;
    int             ap_bytesPerSample;
    int             ap_isBigEndian;
    int             ap_needToSwap;
    int             ap_dataSizeInBytes;
    int             ap_onset;
    int             ap_numberOfFrames;
    int             ap_needToNormalize;
    int             ap_needToResize;
    } t_audioproperties;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void soundfile_propertiesInit (t_audioproperties *args)
{
    args->ap_fileName               = &s_;
    args->ap_fileExtension          = &s_;
    args->ap_sampleRate             = SOUNDFILE_UNDEFINED;
    args->ap_fileType               = SOUNDFILE_UNDEFINED;
    args->ap_headerSize             = SOUNDFILE_UNDEFINED;
    args->ap_numberOfChannels       = SOUNDFILE_UNDEFINED;
    args->ap_bytesPerSample         = SOUNDFILE_UNDEFINED;
    args->ap_isBigEndian            = SOUNDFILE_UNDEFINED;
    args->ap_needToSwap             = SOUNDFILE_UNDEFINED;
    args->ap_dataSizeInBytes        = SOUNDFILE_UNDEFINED;
    args->ap_onset                  = SOUNDFILE_UNDEFINED;
    args->ap_numberOfFrames         = SOUNDFILE_UNKNOWN;
    args->ap_needToNormalize        = SOUNDFILE_UNDEFINED;
    args->ap_needToResize           = SOUNDFILE_UNDEFINED;
}

static inline void soundfile_propertiesCopy (t_audioproperties *args, t_audioproperties *from)
{
    args->ap_fileName               = from->ap_fileName;
    args->ap_fileExtension          = from->ap_fileExtension;
    args->ap_sampleRate             = from->ap_sampleRate;
    args->ap_fileType               = from->ap_fileType;
    args->ap_headerSize             = from->ap_headerSize;
    args->ap_numberOfChannels       = from->ap_numberOfChannels;
    args->ap_bytesPerSample         = from->ap_bytesPerSample;
    args->ap_isBigEndian            = from->ap_isBigEndian;
    args->ap_needToSwap             = from->ap_needToSwap;
    args->ap_dataSizeInBytes        = from->ap_dataSizeInBytes;
    args->ap_onset                  = from->ap_onset;
    args->ap_numberOfFrames         = from->ap_numberOfFrames;
    args->ap_needToNormalize        = from->ap_needToNormalize;
    args->ap_needToResize           = from->ap_needToResize;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SOUNDFILE_HELPER_SIZE       PD_STRING

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define SOUNDFILE_HELPER_INIT(x)    {                           \
                                        *((x)->h_c)      = 0;   \
                                        (x)->h_bytesSet  = 0;   \
                                        (x)->h_onset     = 0;   \
                                        *((x)->h_ID)     = 0;   \
                                        (x)->h_chunkSize = 0;   \
                                    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _headerhelper {
    char    h_c[SOUNDFILE_HELPER_SIZE];
    int     h_bytesSet;
    int     h_onset;
    char    h_ID[5];
    int     h_chunkSize;
    } t_headerhelper;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_error soundfile_helperRead (int f, t_headerhelper *t, off_t offset)
{
    t_error err = ((lseek (f, offset, SEEK_SET)) != offset);
    
    if (err) { t->h_bytesSet = 0; }
    else { 
        t->h_bytesSet = (int)read (f, t->h_c, (size_t)SOUNDFILE_HELPER_SIZE);
    }
    
    err = (t->h_bytesSet <= 0);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    soundfile_readFileParse     (t_glist *x,
                                                    t_symbol *s,
                                                    int *argc,
                                                    t_atom **argv,
                                                    t_audioproperties *p,
                                                    t_object *owner);
                                                    
PD_LOCAL int        soundfile_readFileHeader    (t_glist *x, t_audioproperties *args);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    soundfile_writeFileParse    (t_glist *x,
                                                    t_symbol *s,
                                                    int *argc,
                                                    t_atom **argv,
                                                    t_audioproperties *p,
                                                    t_object *owner);
                                                    
PD_LOCAL int        soundfile_writeFileHeader   (t_glist *x, t_audioproperties *args, t_object *owner);
PD_LOCAL t_error    soundfile_writeFileClose    (int f, int itemsWritten, t_audioproperties *args);
                                            
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       soundfile_encode32          (int numberOfChannels,
                                                    float **v,
                                                    unsigned char *t,
                                                    int numberOfFrames,
                                                    int onset,
                                                    int bytesPerSamples,
                                                    int isBigEndian,
                                                    float normalFactor);
                                            
PD_LOCAL void       soundfile_decode32          (int numberOfChannels,
                                                    float **v,
                                                    unsigned char *t,
                                                    int numberOfFrames,
                                                    int onset,
                                                    int bytesPerSamples,
                                                    int isBigEndian,
                                                    int n);                 /* Number of channels required. */

PD_LOCAL void       soundfile_encode64          (int numberOfChannels,
                                                    t_word **v,
                                                    unsigned char *t,
                                                    int numberOfFrames,
                                                    int onset,
                                                    int bytesPerSamples,
                                                    int isBigEndian,
                                                    float normalFactor);

PD_LOCAL void       soundfile_decode64          (int numberOfChannels,
                                                    t_word **v,
                                                    unsigned char *t,
                                                    int numberOfFrames,
                                                    int onset,
                                                    int bytesPerSamples,
                                                    int isBigEndian,
                                                    int n);                 /* Number of channels required. */


// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_SOUNDFILE_H_
