
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

static t_class *textfile_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void textfile_bang (t_qlist *x)
{
    t_buffer *b = textbuffer_getBuffer (&x->ql_textbuffer);
    
    int i = x->ql_indexOfMessage;
    int start, end;
    
    if (buffer_messagesGetAt (b, i, &start, &end) == PD_ERROR_NONE) {
    //
    int size = end - start;
    t_atom *first = buffer_getAtomAtIndex (b, start);
    t_atom *e = NULL;
    
    PD_ATOMS_ALLOCA (e, size);
        
    atom_copyAtomsExpanded (first, size, e, size, object_getOwner (cast_object (x)));
        
    if (size && IS_SYMBOL (e)) {
        outlet_anything (x->ql_outletLeft, GET_SYMBOL (e), size - 1, e + 1);
    } else {
        outlet_list (x->ql_outletLeft, size, size ? e : NULL);
    }
    
    PD_ATOMS_FREEA (e, size);
    
    x->ql_indexOfMessage += 1;
    //
    } else { x->ql_indexOfMessage = PD_INT_MAX; outlet_bang (x->ql_outletRight); }
}

static void textfile_rewind (t_qlist *x)
{
    x->ql_indexOfMessage = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *textfile_new (t_symbol *s, int argc, t_atom *argv)
{
    t_qlist *x = (t_qlist *)pd_new (textfile_class);
    
    textbuffer_init (&x->ql_textbuffer, object_getOwner (cast_object (x)));
    
    x->ql_indexOfMessage = 0;
    x->ql_outletLeft     = outlet_newMixed (cast_object (x));
    x->ql_outletRight    = outlet_newBang (cast_object (x));
    
    if (argc && !IS_FLOAT (argv)) { qlist_read (x, symbol_withAtoms (argc, argv)); }
    
    warning_deprecatedObject (cast_object (x), sym_textfile);

    return x;
}

static void textfile_free (t_qlist *x)
{
    textbuffer_free (&x->ql_textbuffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void textfile_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_textfile,
            (t_newmethod)textfile_new,
            (t_method)textfile_free,
            sizeof (t_qlist),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)textfile_bang);
        
    class_addMethod (c, (t_method)textfile_rewind,      sym_rewind,     A_NULL);
    
    class_addMethod (c, (t_method)qlist_clear,          sym_clear,      A_NULL);
    class_addMethod (c, (t_method)qlist_set,            sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)qlist_add,            sym_add,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)qlist_append,         sym_append,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)qlist_read,           sym_read,       A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)qlist_write,          sym_write,      A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)qlist_restore,        sym__restore,   A_NULL);

    class_setDataFunction (c, qlist_functionData);
    class_requirePending (c);
    
    textfile_class = c;
}

PD_LOCAL void textfile_destroy (void)
{
    class_free (textfile_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
