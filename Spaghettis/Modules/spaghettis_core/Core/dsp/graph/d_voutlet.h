
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_VOUTLET_H_
#define D_VOUTLET_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _voutlet {
    t_object            vo_obj;             /* Must be the first. */
    t_resample          vo_resample;        /* Extended buffer if resampling is required. */
    int                 vo_dismissed;
    int                 vo_bufferSize;
    t_sample            *vo_buffer;         /* Handle vector size conversion in a buffer. */
    t_voutletclosure    *vo_closure;
    t_outlet            *vo_outlet;
    t_signal            *vo_directSignal;   /* Used to efficiently by-pass the outlet. */
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   voutlet_dsp             (t_voutlet *x, t_signal **sp);
PD_LOCAL void   voutlet_dspPrologue     (t_voutlet *x, t_signal **sp, t_blockproperties *p);
PD_LOCAL void   voutlet_dspEpilogue     (t_voutlet *x, t_signal **sp, t_blockproperties *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_VOUTLET_H_
