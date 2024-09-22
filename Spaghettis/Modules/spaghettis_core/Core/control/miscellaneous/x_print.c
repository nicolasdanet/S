
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *print_class;    /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _print {
    t_object    x_obj;          /* Must be the first. */
    t_symbol    *x_name;
    } t_print;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void print_bang (t_print *x)
{
    post (cast_object (x), "%s: bang", symbol_getName (x->x_name));
}

static void print_float (t_print *x, t_float f)
{
    post (cast_object (x), "%s: %.9g", symbol_getName (x->x_name), f);
}

static void print_symbol (t_print *x, t_symbol *s)
{
    post (cast_object (x), "%s: %s", symbol_getName (x->x_name), symbol_getName (s));
}

static void print_list (t_print *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc < PD_STRING) {
    //
    char *t = atom_atomsToString (argc, argv);
    
    post (cast_object (x), "%s: [ %s ]", symbol_getName (x->x_name), t);
    
    PD_MEMORY_FREE (t);
    //
    } else { warning_tooManyCharacters (cast_object (x), sym_print); }
}

static void print_anything (t_print *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc < PD_STRING) {
    //
    char *t = atom_atomsToString (argc, argv);
    
    post (cast_object (x), "%s: %s [ %s ]", symbol_getName (x->x_name), symbol_getName (s), t);
    
    PD_MEMORY_FREE (t);
    //
    } else { warning_tooManyCharacters (cast_object (x), sym_print); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *print_new (t_symbol *s, int argc, t_atom *argv)
{
    t_print *x = (t_print *)pd_new (print_class);
    
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (t == sym___dash__none) { x->x_name = &s_; }
    else {
        if (argc) { x->x_name = symbol_withAtoms (argc, argv); } 
        else {
            x->x_name = sym_print;
        }
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void print_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_print,
            (t_newmethod)print_new,
            NULL,
            sizeof (t_print),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)print_bang);
    class_addFloat (c, (t_method)print_float);
    class_addSymbol (c, (t_method)print_symbol);
    class_addList (c, (t_method)print_list);
    class_addAnything (c, (t_method)print_anything);
    
    print_class = c;
}

void print_destroy (void)
{
    class_free (print_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

