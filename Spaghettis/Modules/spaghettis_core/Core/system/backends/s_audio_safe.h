
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_AUDIO_SAFE_H_
#define S_AUDIO_SAFE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void audio_safe (t_sample *sound, int size)
{
    t_sample *s = sound; int n = size;
    
    while (n--) {
    //
    t_sample f = *s;
    
    if (PD_FLOAT32_IS_INVALID_OR_ZERO (f)) { f = (t_sample)0.0; }

    *s++ = PD_CLAMP (f, (t_sample)-1.0, (t_sample)1.0);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_AUDIO_SAFE_H_
