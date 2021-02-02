
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_text.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_error textclient_init (t_textclient *x, int *ac, t_atom **av)
{
    int argc = *ac;
    t_atom *argv = *av;
    
    x->tc_name = NULL;
    
    if (argc) {
    //
    /* Dollar expansion is zero in abstraction opened as patch. */
    
    if (IS_FLOAT (argv) && (GET_FLOAT (argv) == 0.0)) { x->tc_name = &s_; argc--; argv++; }
    else {
        if (!IS_SYMBOL (argv)) { return PD_ERROR; }
        else {
            x->tc_name = GET_SYMBOL (argv); argc--; argv++;
        }
    }
    //
    }

    *ac = argc;
    *av = argv;
    
    return PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *textclient_getName (t_textclient *x)
{
    return x->tc_name;
}

PD_LOCAL void textclient_setName (t_textclient *x, t_symbol *s)
{
    x->tc_name = s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *textclient_fetchBuffer (t_textclient *x)
{
    if (x->tc_name) {
    //
    t_textbuffer *y = (t_textbuffer *)symbol_getThingByClass (x->tc_name, textdefine_class);

    if (y) { return textbuffer_getBuffer (y); } else { error_canNotFind (sym_text, x->tc_name); }
    //
    }
    
    return NULL;
}

PD_LOCAL t_glist *textclient_fetchOwner (t_textclient *x)
{
    if (x->tc_name) {
    //
    t_textbuffer *y = (t_textbuffer *)symbol_getThingByClass (x->tc_name, textdefine_class);

    if (y) { return textbuffer_getOwner (y); } else { error_canNotFind (sym_text, x->tc_name); }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void textclient_update (t_textclient *x)
{
    if (x->tc_name) {
    //
    t_textbuffer *y = (t_textbuffer *)symbol_getThingByClass (x->tc_name, textdefine_class);
    
    if (!y) { error_canNotFind (sym_text, x->tc_name); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
