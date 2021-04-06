
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_DSP_H_
#define D_DSP_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SOUNDFILE_CHANNELS  64      /* Maximum number of channels handled for sound files. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Order of inclusion matters. */

#include "dsp/d_macros.h"
#include "dsp/d_cos.h"
#include "dsp/graph/d_main.h"
#include "dsp/graph/d_dspthread.h"
#include "dsp/graph/d_chain.h"
#include "dsp/graph/d_closures.h"
#include "dsp/graph/d_signal.h"
#include "dsp/graph/d_ugen.h"
#include "dsp/graph/d_canvas.h"
#include "dsp/graph/d_resample.h"
#include "dsp/graph/d_block.h"
#include "dsp/graph/d_functions.h"
#include "dsp/graph/d_vinlet.h"
#include "dsp/graph/d_voutlet.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_DSP_H_
