
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int          metadata_inputCv[DEVICES_MAXIMUM_CHANNELS];                 /* Static. */
static int          metadata_inputMinimum[DEVICES_MAXIMUM_CHANNELS];            /* Static. */
static int          metadata_inputMaximum[DEVICES_MAXIMUM_CHANNELS];            /* Static. */
static t_symbol     *metadata_inputName[DEVICES_MAXIMUM_CHANNELS];              /* Static. */

static int          metadata_outputCv[DEVICES_MAXIMUM_CHANNELS];                /* Static. */
static int          metadata_outputMinimum[DEVICES_MAXIMUM_CHANNELS];           /* Static. */
static int          metadata_outputMaximum[DEVICES_MAXIMUM_CHANNELS];           /* Static. */
static t_symbol     *metadata_outputName[DEVICES_MAXIMUM_CHANNELS];             /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void metadata_reset (void)
{
    int i;
    
    for (i = 0; i < DEVICES_MAXIMUM_CHANNELS; i++) {
        
        metadata_inputCv[i]         = 0;
        metadata_inputMinimum[i]    = 0;
        metadata_inputMaximum[i]    = 0;
        metadata_inputName[i]       = NULL;

        metadata_outputCv[i]        = 0;
        metadata_outputMinimum[i]   = 0;
        metadata_outputMaximum[i]   = 0;
        metadata_outputName[i]      = NULL;
    }
}

PD_LOCAL void metadata_report (t_error err)
{
    #if PD_LINUX
    
    int i;
    
    void (*f)(const char *fmt, ...) = err ? post_error : post;

    for (i = 0; i < DEVICES_MAXIMUM_CHANNELS; i++) {
        
        if (metadata_inputCv[i]) {
            (f) (PD_TRANSLATE ("dsp: input %d / cv %d"), i + 1, metadata_inputCv[i]);                   // --
        }
        if (metadata_inputMinimum[i] != metadata_inputMaximum[i]) {
            int t0 = metadata_inputMinimum[i];
            int t1 = metadata_inputMaximum[i];
            (f) (PD_TRANSLATE ("dsp: input %d / minimum %d"), i + 1, PD_MIN (t0, t1));                  // --
            (f) (PD_TRANSLATE ("dsp: input %d / maximum %d"), i + 1, PD_MAX (t0, t1));                  // --
        }
        if (metadata_inputName[i]) {
            (f) (PD_TRANSLATE ("dsp: input %d / name %s"), i + 1, metadata_inputName[i]->s_name);       // --
        }
    }
    
    for (i = 0; i < DEVICES_MAXIMUM_CHANNELS; i++) {
        
        if (metadata_outputCv[i]) {
            (f) (PD_TRANSLATE ("dsp: output %d / cv %d"), i + 1, metadata_outputCv[i]);                 // --
        }
        if (metadata_outputMinimum[i] != metadata_outputMaximum[i]) {
            int t0 = metadata_outputMinimum[i];
            int t1 = metadata_outputMaximum[i];
            (f) (PD_TRANSLATE ("dsp: output %d / minimum %d"), i + 1, PD_MIN (t0, t1));                 // --
            (f) (PD_TRANSLATE ("dsp: output %d / maximum %d"), i + 1, PD_MAX (t0, t1));                 // --
        }
        if (metadata_outputName[i]) {
            (f) (PD_TRANSLATE ("dsp: output %d / name %s"), i + 1, metadata_outputName[i]->s_name);     // --
        }
    }
    
    #endif // PD_LINUX
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void metadata_setCV (int isOutput, int n, int v)
{
    if (isOutput) { metadata_outputCv[n] = (v != 0); } else { metadata_inputCv[n] = (v != 0); }
}

static void metadata_setMinimum (int isOutput, int n, int v)
{
    if (isOutput) { metadata_outputMinimum[n] = v; } else { metadata_inputMinimum[n] = v; }
}

static void metadata_setMaximum (int isOutput, int n, int v)
{
    if (isOutput) { metadata_outputMaximum[n] = v; } else { metadata_inputMaximum[n] = v; }
}

static void metadata_setName (int isOutput, int n, t_symbol *s)
{
    if (isOutput) { metadata_outputName[n] = s; } else { metadata_inputName[n] = s; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if PD_LINUX

PD_LOCAL int metadata_getCV (int isOutput, int n)
{
    PD_ASSERT (n >= 0 && n < DEVICES_MAXIMUM_CHANNELS);
    
    return isOutput ? metadata_outputCv[n] : metadata_inputCv[n];
}

PD_LOCAL int metadata_getMinimum (int isOutput, int n)
{
    PD_ASSERT (n >= 0 && n < DEVICES_MAXIMUM_CHANNELS);
    
    return isOutput ? metadata_outputMinimum[n] : metadata_inputMinimum[n];
}

PD_LOCAL int metadata_getMaximum (int isOutput, int n)
{
    PD_ASSERT (n >= 0 && n < DEVICES_MAXIMUM_CHANNELS);
    
    return isOutput ? metadata_outputMaximum[n] : metadata_inputMaximum[n];
}

PD_LOCAL t_symbol *metadata_getName (int isOutput, int n)
{
    PD_ASSERT (n >= 0 && n < DEVICES_MAXIMUM_CHANNELS);
    
    return isOutput ? metadata_outputName[n] : metadata_inputName[n];
}

#endif // PD_LINUX

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void metadata_setParsed (int argc, t_atom *argv)
{
    t_symbol *s = atom_getSymbolAtIndex (0, argc, argv);
    
    int input   = (s == sym_input);
    int output  = (s == sym_output);
    int reset   = (s == sym_reset);
    
    if ((argc > 3) && (input || output)) {
    //
    int n = atom_getFloat (argv + 1) - 1;
    
    if (n >= 0 && n < DEVICES_MAXIMUM_CHANNELS) {
    //
    t_symbol *k = atom_getSymbol (argv + 2);
    int v       = (int)atom_getFloat (argv + 3);

    if (k == sym_cv)      { metadata_setCV (output, n, v); }
    if (k == sym_minimum) { metadata_setMinimum (output, n, v); }
    if (k == sym_maximum) { metadata_setMaximum (output, n, v); }
    if (k == sym_name)    { metadata_setName (output, n, symbol_withAtoms (argc - 3, argv + 3)); }
    //
    }
    //
    } else if (reset) { metadata_reset(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
