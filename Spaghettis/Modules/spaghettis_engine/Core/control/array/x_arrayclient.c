
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_array.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error arrayclient_init (t_arrayclient *x, int *ac, t_atom **av)
{
    int argc = *ac; t_atom *argv = *av;
    
    x->ac_name = NULL;
    
    if (argc && !error__options (cast_object (x), class_getName (pd_class (x)), argc, argv)) {
    //
    /* Dollar expansion is zero in abstraction opened as patch. */
        
    if (IS_FLOAT (argv) && (GET_FLOAT (argv) == 0.0)) { x->ac_name = &s_; argc--; argv++; }
    else {
        if (!IS_SYMBOL (argv)) { return PD_ERROR; }
        else {
            x->ac_name = GET_SYMBOL (argv); argc--; argv++;
        }
    }
    //
    }
    
    *ac = argc;
    *av = argv;
    
    return PD_ERROR_NONE;
}

void arrayclient_free (t_arrayclient *x)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *arrayclient_getName (t_arrayclient *x)
{
    return (x->ac_name ? x->ac_name : &s_);
}

void arrayclient_setName (t_arrayclient *x, t_symbol *s)
{
    if (s && s != &s_) { x->ac_name = s; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void arrayclient_restore (t_arrayclient *x, t_arrayclient *old)
{
    arrayclient_setName (x, arrayclient_getName (old));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_garray *arrayclient_fetchGraphicArray (t_arrayclient *x)
{
    t_symbol *s = x->ac_name;
    
    if (s && s != &s_) { return garray_fetch (s); }
    else {
        error_canNotFind (cast_object (x), class_getName (pd_class (x)), (s && s != &s_) ? s : sym_garray);
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
