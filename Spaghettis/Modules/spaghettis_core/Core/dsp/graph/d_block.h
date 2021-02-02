
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_BLOCK_H_
#define D_BLOCK_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define BLOCK_MAXIMUM   65536

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _blockproperties {
    int             bp_blockSize;               /* Blocksize (power of two). */
    int             bp_overlap;                 /* Number of overlap (power of two). */
    int             bp_downsample;              /* Downsampling factor. */
    int             bp_upsample;                /* Upsampling factor. */
    int             bp_reblocked;               /* True if reblocking is required. */
    t_float         bp_sampleRate;              /* Sample rate of the context. */
    int             bp_period;                  /* Supermultiple factor. */
    int             bp_frequency;               /* Submultiple factor. */
    } t_blockproperties;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that at most one value between downsample or upsample can be set. */
/* It is the resampling factor (e.g. 1, 2, 4 respectively below or above). */
/* Whereas the other is set to 1. */
/* It is the same with the period and the frequency. */
/* It can be a supermultiple OR a submultiple. */

typedef struct _block {
    t_object        bk_obj;                     /* Must be the first. */
    int             bk_blockSize;               /* Blocksize (power of two). */
    int             bk_overlap;                 /* Number of overlap (power of two). */
    int             bk_downsample;              /* Downsampling factor (power of two). */
    int             bk_upsample;                /* Upsampling factor (power of two). */
    } t_block;

/* See also t_blockclosure structure. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_int      *block_performPrologue      (t_int *w);
PD_LOCAL t_int      *block_performEpilogue      (t_int *w);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_float    block_getResamplingRatio    (t_block *x);
PD_LOCAL int        block_getBlockSize          (t_block *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_BLOCK_H_
