
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Write to buffer. */

static t_int *vinlet_performPrologue (t_int *w)
{
    t_vinletclosure *c = (t_vinletclosure *)(w[1]);
    
    t_sample *out      = c->s_bufferWrite;
    t_sample *in       = c->s_in;
    int n              = c->s_inSize;
    
    if (out == c->s_bufferEnd) {
        t_sample *f1 = c->s_buffer;
        t_sample *f2 = c->s_buffer + c->s_hopSize;
        int shift    = c->s_bufferSize - c->s_hopSize;
        out -= c->s_hopSize;
        if (shift) {
            // PD_LOG ("SHIFT");
            // PD_LOG_NUMBER (c->s_hopSize);
            // PD_LOG ("/");
            // PD_LOG_NUMBER (shift);
        }
        while (shift--) { *f1++ = *f2++; }
    }
    
    // PD_LOG ("P");
    // PD_LOG_NUMBER (out - c->s_buffer);
    // PD_LOG ("/");
    // PD_LOG_NUMBER (n);
    
    while (n--) { *out++ = *in++; }
    
    c->s_bufferWrite = out;
    
    return (w + 2);
}

/* Read from buffer. */

static t_int *vinlet_perform (t_int *w)
{
    t_vinletclosure *c = (t_vinletclosure *)(w[1]);
    PD_RESTRICTED out  = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    t_sample *in = c->s_bufferRead;
    
    int z = c->s_zeroed;
    
    // PD_LOG ("R");
    // PD_LOG_NUMBER (in - c->s_buffer);
    // PD_LOG ("/");
    // PD_LOG_NUMBER (n);
    
    while (n--) { if (z) { *in = 0.0; } *out = *in; out++; in++; }
    
    if (in == c->s_bufferEnd) { in = c->s_buffer; }
    
    c->s_bufferRead = in;
    
    return (w + 4);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vinlet_initialize (void *lhs, void *rhs)
{
    t_vinlet *x   = (t_vinlet *)lhs;
    t_vinlet *old = (t_vinlet *)rhs;
    
    if (x->vi_bufferSize == old->vi_bufferSize) {
    //
    memcpy (x->vi_buffer, old->vi_buffer, x->vi_bufferSize * sizeof (t_sample));    /* No aliasing. */
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void vinlet_dspPrologue (t_vinlet *x, t_signal **signals, t_blockproperties *p)
{
    PD_ASSERT (x->vi_closure == NULL);
    
    if (vinlet_isSignal (x)) {
    //
    if (!p->bp_reblocked) {     /* Vector sizes are equal thus no buffering is required. */
    
        PD_ASSERT (signals); x->vi_directSignal = signals[inlet_getIndexAsSignal (x->vi_inlet)];

    } else {                    /* Buffering required. */
    //
    t_signal *s = NULL;
    int parentVectorSize = 1;
    int vectorSize = 1;

    resample_set (&x->vi_resample, p->bp_downsample, p->bp_upsample);
    
    x->vi_directSignal = NULL;
    
    t_vinletclosure *c = x->vi_closure = vinlet_newClosure (cast_object (x));
    
    if (signals) {
        s = signals[inlet_getIndexAsSignal (x->vi_inlet)];
        parentVectorSize = s->s_vectorSize;
        vectorSize = parentVectorSize * p->bp_upsample / p->bp_downsample;
    }

    int bufferSize = PD_MAX (p->bp_blockSize, vectorSize);
    
    if (bufferSize != x->vi_bufferSize) {
        garbage_newRaw ((void *)x->vi_buffer);
        x->vi_bufferSize = bufferSize;
        x->vi_buffer     = (t_sample *)PD_MEMORY_GET (x->vi_bufferSize * sizeof (t_sample));
    }
    
    c->s_buffer     = x->vi_buffer;
    c->s_bufferSize = x->vi_bufferSize;
    c->s_bufferEnd  = x->vi_buffer + x->vi_bufferSize;
    
    if (!signals) { c->s_zeroed = 1; }
    else {
    //
    int phase = p->bp_period - 1;
    
    c->s_hopSize     = p->bp_period * vectorSize;
    c->s_bufferWrite = phase ? c->s_bufferEnd - (c->s_hopSize - (phase * vectorSize)) : c->s_bufferEnd;
    
    // PD_LOG ("INLET SIZE");
    // PD_LOG_NUMBER (c->s_bufferSize);
    // PD_LOG ("INLET WRITE");
    // PD_LOG_NUMBER (c->s_bufferWrite - c->s_buffer);
    // PD_LOG ("INLET HOP");
    // PD_LOG_NUMBER (c->s_hopSize);
    
    PD_ASSERT (c->s_hopSize <= c->s_bufferSize);
    
    c->s_inSize = vectorSize;
    c->s_in     = s->s_vector;
        
    if (resample_isRequired (&x->vi_resample)) {
        c->s_in = resample_getBufferInlet (&x->vi_resample, s->s_vector, parentVectorSize, vectorSize);
    }

    dsp_add1 (vinlet_performPrologue, c);       /* Must be after resampling above. */
    //
    }
    //
    }
    //
    }
}

void vinlet_dsp (t_vinlet *x, t_signal **sp)
{
    if (vinlet_isSignal (x)) {
    //
    t_signal *out = sp[0];

    if (x->vi_directSignal) { signal_borrow (out, x->vi_directSignal); }    /* By-pass the inlet. */
    else {
    //
    t_vinletclosure *c = x->vi_closure;
    
    /* No phase required. */ 
    /* Submultiple read is always completed at each tick. */
    
    c->s_bufferRead = x->vi_buffer;
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_vinlet *old = (t_vinlet *)garbage_fetch (cast_object (x));
    
    if (old) { initializer_new (vinlet_initialize, x, old); }
    //
    }
    
    dsp_add3 (vinlet_perform, c, out->s_vector, out->s_vectorSize);
    //
    }
    //
    }
    
    x->vi_closure = NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
