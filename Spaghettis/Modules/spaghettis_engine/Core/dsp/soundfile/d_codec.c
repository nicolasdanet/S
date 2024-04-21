
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

/* Note that 2-3 bytes per sample is assumed LPCM. */
/* Likewise, 4 bytes per sample is assumed 32-bit IEEE float. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define SOUNDFILE_SCALE     (1.0 / (1024.0 * 1024.0 * 1024.0 * 2.0))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int soundfile_encodeLinear16Value (float f, float k)
{
    int v = (int)(32768.0 + (f * k));
    v -= 32768;
    return PD_CLAMP (v, -32767, 32767);
}

static inline void soundfile_encodeLinear16BigEndian (float f, float k, unsigned char *p)
{
    int v = soundfile_encodeLinear16Value (f, k);
    
    p[0] = 0xff & (v >> 8);
    p[1] = 0xff & (v);
}

static inline void soundfile_encodeLinear16LittleEndian (float f, float k, unsigned char *p)
{
    int v = soundfile_encodeLinear16Value (f, k);
    
    p[0] = 0xff & (v);
    p[1] = 0xff & (v >> 8);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline int soundfile_encodeLinear24Value (float f, float k)
{
    int v = (int)(8388608.0 + (f * k));
    v -= 8388608;
    return PD_CLAMP (v, -8388607, 8388607);
}

static inline void soundfile_encodeLinear24BigEndian (float f, float k, unsigned char *p)
{
    int v = soundfile_encodeLinear24Value (f, k);
    
    p[0] = 0xff & (v >> 16);
    p[1] = 0xff & (v >> 8);
    p[2] = 0xff & (v);
}

static inline void soundfile_encodeLinear24LittleEndian (float f, float k, unsigned char *p)
{
    int v = soundfile_encodeLinear24Value (f, k);
    
    p[0] = 0xff & (v);
    p[1] = 0xff & (v >> 8);
    p[2] = 0xff & (v >> 16);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline void soundfile_encodeFloat32BigEndian (float f, float k, unsigned char *p)
{
    t_pun32 z; pun32_setFloat (&z, f * k);
    
    uint32_t i = pun32_getInteger (&z);
    
    p[0] = 0xff & (i >> 24);
    p[1] = 0xff & (i >> 16);
    p[2] = 0xff & (i >> 8);
    p[3] = 0xff & (i);
}

static inline void soundfile_encodeFloat32LittleEndian (float f, float k, unsigned char *p)
{
    t_pun32 z; pun32_setFloat (&z, f * k);
    
    uint32_t i = pun32_getInteger (&z);
    
    p[0] = 0xff & (i);
    p[1] = 0xff & (i >> 8);
    p[2] = 0xff & (i >> 16);
    p[3] = 0xff & (i >> 24);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void soundfile_encode32Linear16 (int numberOfChannels,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    float *s = NULL;
    
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    
    float k = (float)(normalFactor * 32768.0);
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            soundfile_encodeLinear16BigEndian (*s, k, p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            soundfile_encodeLinear16LittleEndian (*s, k, p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_encode32Linear24 (int numberOfChannels,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    float *s = NULL;
    
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    
    float k = (float)(normalFactor * 8388608.0);
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            soundfile_encodeLinear24BigEndian (*s, k, p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            soundfile_encodeLinear24LittleEndian (*s, k, p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_encode32Float (int numberOfChannels,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    float *s = NULL;
    
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            soundfile_encodeFloat32BigEndian (*s, normalFactor, p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            soundfile_encodeFloat32LittleEndian (*s, normalFactor, p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

void soundfile_encode32 (int numberOfChannels,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    if (bytesPerSample == 2) {
    
        soundfile_encode32Linear16 (numberOfChannels,
            v, 
            t, 
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian,
            normalFactor);
    
    } else if (bytesPerSample == 3) {

        soundfile_encode32Linear24 (numberOfChannels,
            v, 
            t, 
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian,
            normalFactor);
            
    } else if (bytesPerSample == 4) {

        soundfile_encode32Float (numberOfChannels,
            v, 
            t, 
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian,
            normalFactor);
            
    } else {
    
        PD_BUG;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void soundfile_encode64Linear16 (int numberOfChannels,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    
    float k = (float)(normalFactor * 32768.0);
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            double f = w_getFloat (v[i] + offset + j);
            soundfile_encodeLinear16BigEndian (f, k, p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            double f = w_getFloat (v[i] + offset + j);
            soundfile_encodeLinear16LittleEndian (f, k, p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_encode64Linear24 (int numberOfChannels,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    
    float k = (float)(normalFactor * 8388608.0);
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            double f = w_getFloat (v[i] + offset + j);
            soundfile_encodeLinear24BigEndian (f, k, p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            double f = w_getFloat (v[i] + offset + j);
            soundfile_encodeLinear24LittleEndian (f, k, p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_encode64Float (int numberOfChannels,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            double f = w_getFloat (v[i] + offset + j);
            soundfile_encodeFloat32BigEndian (f, normalFactor, p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < numberOfChannels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            double f = w_getFloat (v[i] + offset + j);
            soundfile_encodeFloat32LittleEndian (f, normalFactor, p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

void soundfile_encode64 (int numberOfChannels,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    float normalFactor)
{
    if (bytesPerSample == 2) {
    
        soundfile_encode64Linear16 (numberOfChannels,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian,
            normalFactor);
    
    } else if (bytesPerSample == 3) {

        soundfile_encode64Linear24 (numberOfChannels,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian,
            normalFactor);
        
    } else if (bytesPerSample == 4) {

        soundfile_encode64Float (numberOfChannels,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian,
            normalFactor);
        
    } else {
    
        PD_BUG;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Left operand of the shift operator is promote to the int type (assumed 32-bit). */

static inline float soundfile_decodeLinear16BigEndian (unsigned char *p)
{
    return (float)(SOUNDFILE_SCALE * ((p[0] << 24) | (p[1] << 16)));
}

static inline float soundfile_decodeLinear16LittleEndian (unsigned char *p)
{
    return (float)(SOUNDFILE_SCALE * ((p[1] << 24) | (p[0] << 16)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline float soundfile_decodeLinear24BigEndian (unsigned char *p)
{
    return (float)(SOUNDFILE_SCALE * ((p[0] << 24) | (p[1] << 16) | (p[2] << 8)));
}

static inline float soundfile_decodeLinear24LittleEndian (unsigned char *p)
{
    return (float)(SOUNDFILE_SCALE * ((p[2] << 24) | (p[1] << 16) | (p[0] << 8)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline float soundfile_decodeFloat32BigEndian (unsigned char *p)
{
    t_pun32 z;
    
    pun32_setInteger (&z, (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3]));
    
    return pun32_getFloat (&z);
}

static inline float soundfile_decodeFloat32LittleEndian (unsigned char *p)
{
    t_pun32 z;
    
    pun32_setInteger (&z, (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | (p[0]));
    
    return pun32_getFloat (&z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void soundfile_decode32Linear16 (int numberOfChannels,
    int n,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    float *s = NULL;
    
    int channels = PD_MIN (numberOfChannels, n);
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            *s = soundfile_decodeLinear16BigEndian (p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            *s = soundfile_decodeLinear16LittleEndian (p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_decode32Linear24 (int numberOfChannels,
    int n,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    float *s = NULL;
    
    int channels = PD_MIN (numberOfChannels, n);
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            *s = soundfile_decodeLinear24BigEndian (p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            *s = soundfile_decodeLinear24LittleEndian (p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_decode32Float (int numberOfChannels,
    int n,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    float *s = NULL;
    
    int channels = PD_MIN (numberOfChannels, n);
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            *s = soundfile_decodeFloat32BigEndian (p2);
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1, s = v[i] + offset; j < numberOfFrames; j++, p2 += bytesPerFrame, s++) {
            *s = soundfile_decodeFloat32LittleEndian (p2);
        }
        
        p1 += bytesPerSample;
        }
    }
}

void soundfile_decode32 (int numberOfChannels,
    float **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    int n)
{
    if (bytesPerSample == 2) {
        
        soundfile_decode32Linear16 (numberOfChannels,
            n,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian);
    
    } else if (bytesPerSample == 3) {
        
        soundfile_decode32Linear24 (numberOfChannels,
            n,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian);
            
    } else if (bytesPerSample == 4) {
        
        soundfile_decode32Float (numberOfChannels,
            n,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian);
            
    } else {
        
        PD_BUG;
    }
    
    /* Set to zero the supernumerary channels. */
    
    {
        int i, j;
        float *s = NULL;
        
        for (i = numberOfChannels; i < n; i++) {
            for (j = 0, s = v[i] + onset; j < numberOfFrames; j++, s++) {
                *s = 0.0; 
            }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void soundfile_decode64Linear16 (int numberOfChannels,
    int n,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    
    int channels = PD_MIN (numberOfChannels, n);
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            w_setFloat (v[i] + offset + j, soundfile_decodeLinear16BigEndian (p2));
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            w_setFloat (v[i] + offset + j, soundfile_decodeLinear16LittleEndian (p2));
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_decode64Linear24 (int numberOfChannels,
    int n,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    
    int channels = PD_MIN (numberOfChannels, n);
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            w_setFloat (v[i] + offset + j, soundfile_decodeLinear24BigEndian (p2));
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            w_setFloat (v[i] + offset + j, soundfile_decodeLinear24LittleEndian (p2));
        }
        
        p1 += bytesPerSample;
        }
    }
}

static void soundfile_decode64Float (int numberOfChannels,
    int n,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian)
{
    int i, j;
    unsigned char *p1 = t;
    unsigned char *p2 = NULL;
    
    int channels = PD_MIN (numberOfChannels, n);
    int bytesPerFrame = bytesPerSample * numberOfChannels;
    int offset = onset;
    
    if (isBigEndian) {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            w_setFloat (v[i] + offset + j, soundfile_decodeFloat32BigEndian (p2));
        }
        
        p1 += bytesPerSample;
        }
        
    } else {
        for (i = 0; i < channels; i++) {
        for (j = 0, p2 = p1; j < numberOfFrames; j++, p2 += bytesPerFrame) {
            w_setFloat (v[i] + offset + j, soundfile_decodeFloat32LittleEndian (p2));
        }
        
        p1 += bytesPerSample;
        }
    }
}

void soundfile_decode64 (int numberOfChannels,
    t_word **v,
    unsigned char *t,
    int numberOfFrames,
    int onset,
    int bytesPerSample,
    int isBigEndian,
    int n)
{
    if (bytesPerSample == 2) {
        
        soundfile_decode64Linear16 (numberOfChannels,
            n,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian);
    
    } else if (bytesPerSample == 3) {
        
        soundfile_decode64Linear24 (numberOfChannels,
            n,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian);
        
    } else if (bytesPerSample == 4) {
        
        soundfile_decode64Float (numberOfChannels,
            n,
            v,
            t,
            numberOfFrames,
            onset,
            bytesPerSample,
            isBigEndian);
        
    } else {
        
        PD_BUG;
    }
    
    /* Set to zero the supernumerary channels. */
    
    {
        int i, j;
    
        for (i = numberOfChannels; i < n; i++) {
            for (j = 0; j < numberOfFrames; j++) { w_setFloat (v[i] + onset + j, 0.0); }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
