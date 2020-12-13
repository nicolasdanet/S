
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_array.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void *arrayrange_new (t_class *c, int argc, t_atom *argv, int makeOnsetInlet, int makeSizeInlet)
{
    t_arrayrange *x = (t_arrayrange *)pd_new (c);

    x->ar_error = arrayclient_init (&x->ar_arrayclient, &argc, &argv);
    x->ar_onset = 0;
    x->ar_size  = -1;
    
    if (!x->ar_error) {
    //
    if (makeOnsetInlet) { inlet_newFloat (cast_object (x), &x->ar_onset); }
    if (makeSizeInlet)  { inlet_newFloat (cast_object (x), &x->ar_size);  }
        
    if (argc && IS_FLOAT (argv)) { x->ar_onset = GET_FLOAT (argv); argc--; argv++; }
    if (argc && IS_FLOAT (argv)) { x->ar_size  = GET_FLOAT (argv); argc--; argv++; }

    if (argc) { warning_unusedArguments (class_getName (c), argc, argv); }
        
    inlet_newSymbol (cast_object (x), ARRAYCLIENT_ADDRESS_NAME (&x->ar_arrayclient));
    //
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void arrayrange_setOnset (t_arrayrange *x, t_float f)
{
    x->ar_onset = f;
}

PD_LOCAL void arrayrange_setSize (t_arrayrange *x, t_float f)
{
    x->ar_size = f;
}

PD_LOCAL t_float arrayrange_getOnset (t_arrayrange *x)
{
    return x->ar_onset;
}

PD_LOCAL t_float arrayrange_getSize (t_arrayrange *x)
{
    return x->ar_size;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_garray *arrayrange_getRange (t_arrayrange *x, int *i, int *n)
{
    t_garray *a = arrayclient_fetchGraphicArray ((t_arrayclient *)x);
    
    *i = 0;
    *n = 0;
    
    if (a) {
    //
    int size = garray_getSize (a);
    int count, first = PD_CLAMP (x->ar_onset, 0, size);
    
    if (x->ar_size < 0) { count = size - first; }
    else {
        count = x->ar_size; if (first + count > size) { count = size - first; }
    }
    
    *i = first;
    *n = count;
    //
    }
    
    return a;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error arrayrange_quantile (t_arrayrange *x, t_float f, t_float *v)
{
    int start, n; t_garray *a = arrayrange_getRange (x, &start, &n);

    *v = 0.0;
    
    if (a) {
    //
    int i = 0; double sum = 0.0;
    
    for (i = 0; i < n; i++) {
        t_float t = garray_getFloatAtIndex (a, start + i);
        sum += (t > 0.0) ? t : 0.0; 
    }
    
    sum *= f;
    
    for (i = 0; i < n - 1; i++) {
        t_float t = garray_getFloatAtIndex (a, start + i);
        sum -= (t > 0.0) ? t : 0.0;
        if (sum < 0) {
            break; 
        }
    }
    
    *v = i;
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *arrayrange_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_arrayrange *x = (t_arrayrange *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, arrayclient_getName (&x->ar_arrayclient));
    buffer_appendFloat (b,  arrayrange_getOnset (x));
    buffer_appendFloat (b,  arrayrange_getSize (x));
    
    return b;
    //
    }
    
    return NULL;
}

PD_LOCAL void arrayrange_restore (t_arrayrange *x, t_symbol *s, int argc, t_atom *argv)
{
    t_arrayrange *old = (t_arrayrange *)instance_pendingFetch (cast_object (x));

    t_symbol *t   = atom_getSymbolAtIndex (0, argc, argv);
    t_float onset = old ? old->ar_onset : atom_getFloatAtIndex (1, argc, argv);
    t_float size  = old ? old->ar_size  : atom_getFloatAtIndex (2, argc, argv);
    
    arrayrange_setOnset (x, onset);
    arrayrange_setSize (x,  size);
    
    if (old) { arrayclient_restore (&x->ar_arrayclient, &old->ar_arrayclient); }
    else {
        arrayclient_setName (&x->ar_arrayclient, t);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
