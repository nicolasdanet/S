
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_CLOSURES_H_
#define D_CLOSURES_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _closure {
    struct _closure     *s_next;
    t_id                s_id;
    t_id                s_src;
    int                 s_type;
    } t_closure;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct _space {
    t_closure           s_closure;                  /* Must be the first. */
    t_float             s_float0;
    t_float             s_float1;
    t_float             s_float2;
    t_float             s_float3;
    t_float             s_float4;
    t_float             s_float5;
    t_float             s_float6;
    t_float             s_float7;
    int                 s_int0;
    int                 s_int1;
    int                 s_int2;
    int                 s_int3;
    int                 s_int4;
    int                 s_int5;
    int                 s_int6;
    int                 s_int7;
    void                *s_pointer0;
    // void             *s_pointer1;
    // void             *s_pointer2;
    // void             *s_pointer3;
    // void             *s_pointer4;
    // void             *s_pointer5;
    // void             *s_pointer6;
    // void             *s_pointer7;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _sfvectors {
    t_closure           s_closure;                  /* Must be the first. */
    int                 s_size;
    t_sample            *s_v[SOUNDFILE_CHANNELS];
    } t_sfvectors;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _FFTState {
    t_closure           s_closure;                  /* Must be the first. */
    int                 s_size;
    double              *s_cache;
    } t_FFTState;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _garbage {
    t_closure           s_closure;                  /* Must be the first. */
    void                *s_ptr;
    } t_garbage;


// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _blockclosure {
    t_closure           s_closure;                  /* Must be the first. */
    int                 s_reblocked;                /* True if reblocking is required. */
    int                 s_contextLength;            /* Size of the DSP chain for all the context. */
    int                 s_phase;                    /* Index for supermultiple block size. */
    int                 s_period;                   /* Supermultiple factor. */
    int                 s_count;                    /* Counter for submultiple block size. */
    int                 s_frequency;                /* Submultiple factor. */
    } t_blockclosure;

typedef struct _vinletclosure {
    t_closure           s_closure;                  /* Must be the first. */
    int                 s_zeroed;
    int                 s_hopSize;                  /* Size of the hop if overlapped. */
    int                 s_inSize;
    int                 s_bufferSize;               /* Handle vector size conversion in a buffer. */
    t_sample            *s_in;
    t_sample            *s_buffer;
    t_sample            *s_bufferEnd;
    t_sample            *s_bufferWrite;
    t_sample            *s_bufferRead;
    } t_vinletclosure;

typedef struct _voutletclosure {
    t_closure           s_closure;                  /* Must be the first. */
    int                 s_hopSize;                  /* Size of the hop if overlapped. */
    int                 s_outSize;
    int                 s_bufferSize;               /* Handle vector size conversion in a buffer. */
    t_sample            *s_out;
    t_sample            *s_buffer;
    t_sample            *s_bufferEnd;
    t_sample            *s_bufferWrite;
    t_sample            *s_bufferRead;
    } t_voutletclosure;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void fft_stateRelease      (t_FFTState *x);
PD_LOCAL void fft_stateInitialize   (t_FFTState *x, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_object           *garbage_fetch          (t_object *o);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_space            *space_new              (t_object *owner);
PD_LOCAL t_sfvectors        *sfvectors_new          (t_object *owner);
PD_LOCAL t_FFTState         *fftstate_new           (t_object *owner, int n);

PD_LOCAL t_blockclosure     *block_newClosure       (t_object *owner);
PD_LOCAL t_vinletclosure    *vinlet_newClosure      (t_object *owner);
PD_LOCAL t_voutletclosure   *voutlet_newClosure     (t_object *owner);

PD_LOCAL void               garbage_newRaw          (void *m);      /* Without DSP chain it is freed. */
PD_LOCAL int                garbage_newObject       (t_object *o);  /* Without DSP chain zero is returned. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_CLOSURES_H_
