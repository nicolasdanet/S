
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_VINLET_H_
#define D_VINLET_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _vinlet {
    t_object        vi_obj;             /* Must be the first. */
    t_resample      vi_resample;        /* Extended buffer if resampling is required. */
    int             vi_dismissed;
    int             vi_bufferSize;
    t_sample        *vi_buffer;         /* Handle vector size conversion in a buffer. */
    t_vinletclosure *vi_closure;
    t_outlet        *vi_outlet;
    t_inlet         *vi_inlet;
    t_signal        *vi_directSignal;   /* Used to efficiently by-pass the inlet. */
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   vinlet_dsp          (t_vinlet *x, t_signal **sp);
PD_LOCAL void   vinlet_dspPrologue  (t_vinlet *x, t_signal **sp,  t_blockproperties *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_VINLET_H_
