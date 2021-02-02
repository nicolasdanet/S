
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_float         *cos_tilde_table;       /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class  *cos_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _cost_tilde {
    t_object    x_obj;                  /* Must be the first. */
    t_outlet    *x_outlet;
    } t_cos_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0

PD_LOCAL void cos_tilde_initialize (void)
{
    if (!cos_tilde_table) {
    //
    double phase = 0.0;
    double phaseIncrement = PD_TWO_PI / COSINE_TABLE_SIZE;
    int i;
    
    cos_tilde_table = (t_float *)PD_MEMORY_GET (sizeof (t_float) * (COSINE_TABLE_SIZE + 1));
    
    for (i = 0; i < COSINE_TABLE_SIZE + 1; i++) {
        cos_tilde_table[i] = (t_float)cos (phase);
        phase += phaseIncrement;
    }
    //
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://github.com/pure-data/pure-data/pull/106 > */

PD_LOCAL void cos_tilde_initialize (void)
{
    if (!cos_tilde_table) {
    //
    double phase = 0.0;
    double phaseIncrement = PD_TWO_PI / COSINE_TABLE_SIZE;
    int i;
        
    t_float *p = cos_tilde_table = (t_float *)PD_MEMORY_GET (sizeof (t_float) * (COSINE_TABLE_SIZE + 1));
    
    for (i = COSINE_TABLE_SIZE / 4; i--; p++, phase += phaseIncrement) { *p = (t_float)cos (phase); }
    
    *p++ = (t_float)0.0;
    
    for (i = COSINE_TABLE_SIZE / 4; i--; p++) { *p = -cos_tilde_table[i]; }
    for (i = COSINE_TABLE_SIZE / 2; i--; p++) { *p =  cos_tilde_table[i]; }     // --
    //
    }
}

PD_LOCAL void cos_tilde_release (void)
{
    if (cos_tilde_table) { PD_MEMORY_FREE (cos_tilde_table); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *cos_tilde_perform (t_int *w)
{
    PD_RESTRICTED in = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) {
    //
    *out++ = (t_sample)dsp_getCosineAtLUT (dsp_clipForHoeldrichOverflow (*in++) * COSINE_TABLE_SIZE);
    //
    }

    return (w + 4);
}

static void cos_tilde_dsp (t_cos_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add3 (cos_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *cos_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_cos_tilde *x = (t_cos_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *cos_tilde_new (void)
{
    t_cos_tilde *x = (t_cos_tilde *)pd_new (cos_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void cos_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_cos__tilde__,
            (t_newmethod)cos_tilde_new,
            NULL,
            sizeof (t_cos_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
            
    class_addDSP (c, (t_method)cos_tilde_dsp);
    
    class_setDataFunction (c, cos_tilde_functionData);
    
    cos_tilde_class = c;
}

PD_LOCAL void cos_tilde_destroy (void)
{
    class_free (cos_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
