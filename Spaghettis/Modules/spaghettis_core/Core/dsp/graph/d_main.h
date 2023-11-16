
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_MAIN_H_
#define D_MAIN_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/*

    Ugen's routines build a temporary graph from the DSP objects.
    It is sorted next to obtain a linear list of operations to perform.
    Memory for signals is allocated according to the interconnections.
    Once that's been done, the graph is deleted (while the signals remain).
    
    Prologue and epilogue functions manage nested graphs relations.
    With resampling and reblocking it could require additional buffers.

    The "block~" object maintains the synchronisation with the parent's DSP process.
    It does NOT do any computation in its own right.
    It triggers associated ugens at a supermultiple or submultiple of the upstream.
    
    The overall order of scheduling is:

        - inlets prologue
        - block prologue
        - the ugens in the graph, including inlets and outlets
        - block epilogue
        - outlets epilogue

    The related functions called are:
 
        - vinlet_performPrologue
        - block_performPrologue
        - vinlet_perform
        - voutlet_perform
        - block_performEpilogue
        - voutlet_performEpilogue
 
    Note that jumps can occurs to by-pass or redo a sequence.
 
*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   dsp_setState    (int n);
int    dsp_getState    (void);

void   dsp_update      (void);
int    dsp_suspend     (void);
void   dsp_resume      (int oldState);

void   dsp_close       (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_MAIN_H_
