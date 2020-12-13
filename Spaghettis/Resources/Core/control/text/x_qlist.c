
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

/* Note that for now that object is reset with encapsulation. */

// -- TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void qlist_dismiss (t_qlist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *qlist_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void qlist_proceedWait (t_qlist *x, int doNotSend, int isAutomatic, int start, int end)
{
    t_buffer *b = textbuffer_getBuffer (&x->ql_textbuffer);
    int i = start + 1;
        
    while (i < buffer_getSize (b) && IS_FLOAT (buffer_getAtomAtIndex (b, i))) { i++; }
    
    x->ql_waitCount = i - start;
    
    if (isAutomatic) { clock_delay (x->ql_clock, buffer_getFloatAtIndex (b, start)); }
    else {
        outlet_list (x->ql_outletLeft, x->ql_waitCount, buffer_getAtomAtIndex (b, start));
    }
}

static int qlist_proceedNext (t_qlist *x,
    int doNotSend,
    int isAutomatic,
    int start,
    int end,
    t_atomtype type)
{
    t_buffer *b = textbuffer_getBuffer (&x->ql_textbuffer);
    
    int count = end - start;
    
    if (count && !x->ql_target) {
        t_atom *a = buffer_getAtomAtIndex (b, start);
        t_atom e; atom_copyAtomsExpanded (a, 1, &e, 1, x->ql_owner);
        
        if (!IS_SYMBOL (&e)) { return 0; }
        else {
            t_symbol *t = GET_SYMBOL (&e);
            if (symbol_hasThingQuiet (t)) { x->ql_target = symbol_getThing (t); }
            else {
                return 0;
            }
        }
        start++;
        count--;
    }
    
    if (x->ql_target) {
    //
    x->ql_flagRewound = 0;
    
    if (!doNotSend) {
        if (!count) { pd_message (x->ql_target, &s_list, 0, NULL); }
        else {
            t_atom *first = buffer_getAtomAtIndex (b, start);
            t_atom *e = NULL;
            
            PD_ATOMS_ALLOCA (e, count);
            
            atom_copyAtomsExpanded (first, count, e, count, x->ql_owner);
            
            if (IS_FLOAT (e)) { pd_message (x->ql_target, &s_list, count, e); }
            else if (IS_SYMBOL (e)) {
                pd_message (x->ql_target, GET_SYMBOL (e), count - 1, e + 1);
            }
            
            PD_ATOMS_FREEA (e, count);
        }
    }
    
    if (type != A_COMMA)   { x->ql_target = NULL; }
    if (x->ql_flagRewound) { return 1; }
    //
    }

    return 0;
}

static void qlist_proceed (t_qlist *x, int doNotSend, int isAutomatic)
{
    t_buffer *b = textbuffer_getBuffer (&x->ql_textbuffer);
    t_error err = PD_ERROR_NONE;
    
    PD_ASSERT (!x->ql_flagReentrant);
        
    x->ql_target = NULL;
    x->ql_flagReentrant = 1;
    
    while (!err) {
    //
    int i = x->ql_indexOfMessage;
    t_atomtype type;
    int start, end, size;
    
    err = buffer_messagesGetAtWithTypeOfEnd (b, i, &start, &end, &type);
    
    PD_ASSERT (x->ql_waitCount <= end - start);
    
    if (!err) {
        
        start += x->ql_waitCount;
        size = end - start;
        
        if (size && !x->ql_target && IS_FLOAT (buffer_getAtomAtIndex (b, start))) {
            qlist_proceedWait (x, doNotSend, isAutomatic, start, end);
            break;
            
        } else {
            x->ql_waitCount = 0;
            x->ql_indexOfMessage += 1;
            
            if (qlist_proceedNext (x, doNotSend, isAutomatic, start, end, type)) {
                break;
            }
        }
    }
    //
    }

    x->ql_flagReentrant = 0;
    
    if (err) { x->ql_indexOfMessage = PD_INT_MAX; outlet_bang (x->ql_outletRight); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void qlist_task (t_qlist *x)
{
    qlist_proceed (x, 0, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void qlist_bang (t_qlist *x)
{
    qlist_rewind (x);

    if (!x->ql_flagReentrant) { qlist_proceed (x, 0, 1); }
    else {
        clock_delay (x->ql_clock, 0.0);
    }
}

static void qlist_next (t_qlist *x, t_float f)
{
    if (!x->ql_flagReentrant) { qlist_proceed (x, (f != 0.0), 0); }
    else {
        error_unexpected (sym_qlist, sym_next);
    }
}

PD_LOCAL void qlist_rewind (t_qlist *x)
{
    if (x->ql_clock) { clock_unset (x->ql_clock); }
    
    x->ql_indexOfMessage = 0;
    x->ql_waitCount      = 0;
    x->ql_flagRewound    = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void qlist_clear (t_qlist *x)
{
    qlist_rewind (x);
    buffer_clear (textbuffer_getBuffer (&x->ql_textbuffer));
}

PD_LOCAL void qlist_set (t_qlist *x, t_symbol *s, int argc, t_atom *argv)
{
    qlist_clear (x);
    buffer_deserialize (textbuffer_getBuffer (&x->ql_textbuffer), argc, argv);
}

PD_LOCAL void qlist_add (t_qlist *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_append (textbuffer_getBuffer (&x->ql_textbuffer), argc, argv);
    buffer_appendSemicolon (textbuffer_getBuffer (&x->ql_textbuffer));
}

PD_LOCAL void qlist_append (t_qlist *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_append (textbuffer_getBuffer (&x->ql_textbuffer), argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void qlist_read (t_qlist *x, t_symbol *s)
{
    if (s != &s_) {
    //
    qlist_clear (x);
    textbuffer_read (&x->ql_textbuffer, s);
    //
    }
}

PD_LOCAL void qlist_write (t_qlist *x, t_symbol *s)
{
    textbuffer_write (&x->ql_textbuffer, s);
}

/* Note that float arguments are always passed at last. */

static void qlist_unit (t_qlist *x, t_symbol *unitName, t_float f)
{
    t_error err = clock_setUnitParsed (x->ql_clock, f, (unitName == &s_ ? sym_millisecond : unitName));
    
    if (err) { error_invalid (sym_qlist, sym_unit); }
    else {
        x->ql_unitName  = unitName;
        x->ql_unitValue = f;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *qlist_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_qlist *x  = (t_qlist *)z;
    t_buffer *b = buffer_new();
    
    if (x->ql_unitName) {
        buffer_appendSymbol (b, sym_unit);
        buffer_appendFloat (b,  x->ql_unitValue);
        buffer_appendSymbol (b, x->ql_unitName);
        buffer_appendComma (b);
    }
    
    buffer_appendSymbol (b, sym_set);
    buffer_serialize (b, textbuffer_getBuffer (&x->ql_textbuffer));
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

static void qlist_functionDismiss (t_object *z)
{
    qlist_dismiss ((t_qlist *)z);
}

PD_LOCAL void qlist_restore (t_qlist *x, t_symbol *s, int argc, t_atom *argv)
{
    t_qlist *old = (t_qlist *)instance_pendingFetch (cast_object (x));
    
    if (old) {
    //
    t_buffer *b = buffer_new();
    buffer_serialize (b, textbuffer_getBuffer (&old->ql_textbuffer));
    qlist_set (x, NULL, buffer_getSize (b), buffer_getAtoms (b));
    buffer_free (b);
    
    if (old->ql_unitName) { qlist_unit (x, old->ql_unitName, old->ql_unitValue); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *qlist_new (t_symbol *s, int argc, t_atom *argv)
{
    t_qlist *x = (t_qlist *)pd_new (qlist_class);
    
    textbuffer_init (&x->ql_textbuffer);
    
    x->ql_indexOfMessage = 0;
    x->ql_waitCount      = 0;
    x->ql_owner          = instance_contextGetCurrent();
    x->ql_outletLeft     = outlet_newList (cast_object (x));
    x->ql_outletRight    = outlet_newBang (cast_object (x));
    x->ql_clock          = clock_new ((void *)x, (t_method)qlist_task);

    if (argc && !IS_FLOAT (argv)) { qlist_read (x, symbol_withAtoms (argc, argv)); }
    
    static int once = 0; if (!once) { warning_deprecatedObject (sym_qlist); once = 1; }
    
    return x;
}

static void qlist_dismiss (t_qlist *x)
{
    clock_unset (x->ql_clock);
}

static void qlist_free (t_qlist *x)
{
    clock_free (x->ql_clock); textbuffer_free (&x->ql_textbuffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void qlist_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_qlist,
            (t_newmethod)qlist_new,
            (t_method)qlist_free,
            sizeof (t_qlist),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)qlist_bang);
    
    class_addMethod (c, (t_method)qlist_rewind,         sym_rewind,     A_NULL);
    class_addMethod (c, (t_method)qlist_clear,          sym_clear,      A_NULL);
    class_addMethod (c, (t_method)qlist_set,            sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)qlist_add,            sym_add,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)qlist_append,         sym_append,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)qlist_next,           sym_next,       A_DEFFLOAT, A_NULL); 
    class_addMethod (c, (t_method)qlist_read,           sym_read,       A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)qlist_write,          sym_write,      A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)qlist_unit,           sym_unit,       A_FLOAT, A_DEFSYMBOL, A_NULL); 
    class_addMethod (c, (t_method)qlist_unit,           sym_tempo,      A_FLOAT, A_DEFSYMBOL, A_NULL);
    class_addMethod (c, (t_method)qlist_restore,        sym__restore,   A_NULL);

    class_setDataFunction (c, qlist_functionData);
    class_setDismissFunction (c, qlist_functionDismiss);
    class_requirePending (c);
    
    qlist_class = c;
}

PD_LOCAL void qlist_destroy (void)
{
    class_free (qlist_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
