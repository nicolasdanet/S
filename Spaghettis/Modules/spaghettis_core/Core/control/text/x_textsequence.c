
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_text.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int atom_copyAtomsExpandedWithArguments (t_atom *, int, t_atom *, int, t_glist *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *textsequence_class;                 /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _textsequence {
    t_textclient        x_textclient;
    t_float             x_delay;
    int                 x_indexOfMessage;
    int                 x_waitNumberOfLeading;
    int                 x_waitCount;
    int                 x_isAutomatic;
    int                 x_isLooping;
    t_float             x_unitValue;
    t_symbol            *x_unitName;
    int                 x_argc;
    t_atom              *x_argv;
    t_symbol            *x_sendTo;
    t_symbol            *x_waitSymbol;
    t_outlet            *x_outletMain;
    t_outlet            *x_outletWait; 
    t_outlet            *x_outletEnd;
    t_clock             *x_clock;
    } t_textsequence;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textsequence_dismiss    (t_textsequence *);
static void textsequence_stop       (t_textsequence *);
static void textsequence_message    (t_textsequence *, t_float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int textsequence_proceedNeedToWait (t_textsequence *x, t_buffer *b, int start, int end) 
{
    if (end - start == 0)    { return 0; }
    else if (x->x_waitCount) { return 0; }
    else if (x->x_sendTo)    { return 0; }
    else {
        t_atom *first    = buffer_getAtomAtIndex (b, start);
        int waitOnNumber = IS_FLOAT (first)  && x->x_waitNumberOfLeading;
        int waitOnSymbol = IS_SYMBOL (first) && (GET_SYMBOL (first) == x->x_waitSymbol);
        
        return (waitOnNumber || waitOnSymbol);
    }
}

static int textsequence_proceedSetWaitRange (t_textsequence *x, t_buffer *b, int start, int end)
{
    if (IS_FLOAT (buffer_getAtomAtIndex (b, start))) {
        int n = start + x->x_waitNumberOfLeading;
        int i = start;
        n = (n < 0 ? PD_INT_MAX : n);
        n = PD_MIN (n, end);
        while (i < n && IS_FLOAT (buffer_getAtomAtIndex (b, i))) {
            i++; x->x_waitCount++;
        }
            
    } else {
        x->x_waitCount = end - start;
    }
    
    return (x->x_waitCount == (end - start));       /* Advance if nothing remains to output. */
}

static void textsequence_proceedOutWaitForSymbol (t_textsequence *x, t_buffer *b, int start, int end)
{
    if (x->x_isAutomatic && x->x_waitCount == 2 && IS_FLOAT (buffer_getAtomAtIndex (b, start + 1))) {
        x->x_delay = buffer_getFloatAtIndex (b, start + 1);
        
    } else {
        PD_ASSERT (x->x_outletWait);
        PD_ASSERT (x->x_waitCount);
        x->x_isAutomatic = 0;
        outlet_list (x->x_outletWait, x->x_waitCount - 1, buffer_getAtomAtIndex (b, start + 1));
    }
}

static void textsequence_proceedOutWaitForFloat (t_textsequence *x, t_buffer *b, int start, int end)
{
    if (x->x_isAutomatic && x->x_waitCount == 1) { 
        x->x_delay = buffer_getFloatAtIndex (b, start);
        
    } else {
        PD_ASSERT (x->x_outletWait);
        PD_ASSERT (x->x_waitCount);
        x->x_isAutomatic = 0;
        outlet_list (x->x_outletWait, x->x_waitCount, buffer_getAtomAtIndex (b, start));
    }
}

static void textsequence_proceedOutWait (t_textsequence *x, t_buffer *b, int start, int end)
{
    int k = IS_FLOAT (buffer_getAtomAtIndex (b, start));
    
    x->x_sendTo    = NULL;
    x->x_isLooping = 0;

    if (k) { textsequence_proceedOutWaitForFloat (x, b, start, end);  } 
    else   { textsequence_proceedOutWaitForSymbol (x, b, start, end); }
}

static void textsequence_proceedOutContentMain (t_textsequence *x, t_atom *t, int count, t_atomtype type)
{
    /* Ensure that comma separated messages are well prepended. */
    /* For instance "toto a b c, 1 2;" results in "toto a b c" and "toto 1 2". */
        
    if (x->x_sendTo) {
        if (count) { memmove (t + 1, t, count * sizeof (t_atom)); }
        SET_SYMBOL (t, x->x_sendTo);
        count++;
    }
        
    if (type != A_COMMA) { x->x_sendTo = NULL; }
    else {
        if (!x->x_sendTo && count && IS_SYMBOL (t)) { x->x_sendTo = GET_SYMBOL (t); }
    }
        
    outlet_list (x->x_outletMain, count, t);
}

static void textsequence_proceedOutContentGlobal (t_textsequence *x, t_atom *t, int count, t_atomtype type)
{
    int shitfRight = 0;
    t_symbol *symSend = x->x_sendTo;
    
    PD_ASSERT (count > 0);
    
    if (!symSend) { if (IS_SYMBOL (t)) { symSend = GET_SYMBOL (t); } else { PD_BUG; } shitfRight = 1; }
    
    if (symbol_hasThing (symSend, cast_object (x))) {
    //    
    int n = count - shitfRight;
    t_atom *v = t + shitfRight;
    
    if (n > 0 && IS_SYMBOL (v)) { pd_message (symbol_getThing (symSend), GET_SYMBOL (v), n - 1, v + 1); }
    else {
        pd_list (symbol_getThing (symSend), n, v);
    }
    //
    }
    
    x->x_sendTo = (type != A_COMMA ? NULL : symSend);
}

static void textsequence_proceedOutContent (t_textsequence *x, 
    t_buffer *b, 
    int start, 
    int end, 
    t_atomtype type,
    int argc,
    t_atom *argv)
{
    t_glist *view = textclient_fetchOwner (&x->x_textclient);
        
    int count = end - start - x->x_waitCount;
    t_atom *a = buffer_getAtomAtIndex (b, start + x->x_waitCount);
    int size  = count + 1;
    t_atom *t = NULL;
    
    PD_ATOMS_ALLOCA (t, size);      /* Extra size reserved for possible labelling (see above). */
    
    if (argc) { atom_copyAtomsExpandedWithArguments (a, count, t, count, view, argc, argv);  }
    else {
        atom_copyAtomsExpanded (a, count, t, count, view);
    }
    
    if (x->x_outletMain) { textsequence_proceedOutContentMain (x, t, count, type); }
    else if (count > 0)  { textsequence_proceedOutContentGlobal (x, t, count, type); }
    
    PD_ATOMS_FREEA (t, size);
}

static void textsequence_proceed (t_textsequence *x, int argc, t_atom *argv)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);
    int start, end;
    t_atomtype type;
        
    if (b && !buffer_messagesGetAtWithTypeOfEnd (b, x->x_indexOfMessage, &start, &end, &type)) {
        
        int advance, needToWait = textsequence_proceedNeedToWait (x, b, start, end);
        
        if (needToWait) {
            advance = textsequence_proceedSetWaitRange (x, b, start, end); 
            textsequence_proceedOutWait (x, b, start, end);
        } else {
            advance = 1;
            textsequence_proceedOutContent (x, b, start, end, type, argc, argv);
        }
        
        if (advance) { x->x_waitCount = 0; x->x_indexOfMessage += 1; }
        
    } else { 
    
        if (!b) { error_undefined (cast_object (x), sym_text__space__search, sym_text); }
        x->x_indexOfMessage = PD_INT_MAX;
        x->x_waitCount      = 0;
        x->x_isLooping      = 0;
        x->x_isAutomatic    = 0;
        x->x_sendTo         = NULL;
        outlet_bang (x->x_outletEnd);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textsequence_task (t_textsequence *x)
{
    x->x_sendTo = NULL;
    x->x_delay  = 0.0;
    
    while (x->x_isAutomatic) {
        x->x_isLooping = 1; while (x->x_isLooping) { textsequence_proceed (x, x->x_argc, x->x_argv); }
        if (x->x_delay > 0.0) { break; }
    }
    
    if (x->x_isAutomatic) { clock_delay (x->x_clock, x->x_delay); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void textsequence_bang (t_textsequence *x)
{
    x->x_isLooping = 1; while (x->x_isLooping) { textsequence_proceed (x, x->x_argc, x->x_argv); }
}

static void textsequence_list (t_textsequence *x, t_symbol *s, int argc, t_atom *argv)
{
    x->x_isLooping = 1;
    
    while (x->x_isLooping) {
    //
    if (argc) { textsequence_proceed (x, argc, argv); }
    else {
        textsequence_proceed (x, x->x_argc, x->x_argv);
    }
    //
    }
}

static void textsequence_stop (t_textsequence *x)
{
    x->x_isLooping = 0; if (x->x_isAutomatic) { clock_unset (x->x_clock); x->x_isAutomatic = 0; }
}

static void textsequence_step (t_textsequence *x)
{
    textsequence_stop (x); textsequence_proceed (x, x->x_argc, x->x_argv);
}

static void textsequence_automatic (t_textsequence *x)
{
    x->x_sendTo = NULL;
    
    if (x->x_isAutomatic) { clock_unset (x->x_clock); } else { x->x_isAutomatic = 1; }
    
    textsequence_task (x);
}

static void textsequence_rewind (t_textsequence *x)
{
    textsequence_message (x, 0.0);
}

static void textsequence_message (t_textsequence *x, t_float f)
{
    t_buffer *b = textclient_fetchBuffer (&x->x_textclient);

    if (b) {
    
        x->x_indexOfMessage = (int)PD_ABS (f);
        x->x_waitCount = 0;
        x->x_sendTo = NULL;

    } else { error_undefined (cast_object (x), sym_text__space__sequence, sym_text); }
}

static void textsequence_arguments (t_textsequence *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    size_t oldSize = x->x_argc * sizeof (t_atom);
    size_t newSize = argc * sizeof (t_atom);
    
    x->x_argc = argc;
    x->x_argv = (t_atom *)PD_MEMORY_RESIZE (x->x_argv, oldSize, newSize);
    
    for (i = 0; i < argc; i++) { x->x_argv[i] = argv[i]; }
}

/* Note that float arguments are always passed at last. */

static void textsequence_unit (t_textsequence *x, t_symbol *unitName, t_float f)
{
    t_error err = clock_setUnitParsed (x->x_clock, f, unitName);
    
    if (err) { error_invalid (cast_object (x), sym_text__space__sequence, sym_unit); }
    else {
        x->x_unitName  = unitName;
        x->x_unitValue = f;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *textsequence_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_textsequence *x = (t_textsequence *)z;
    
    t_buffer *b = buffer_new();
        
    if (x->x_unitName || x->x_argc) {
    //
    if (x->x_unitName) {
        buffer_appendSymbol (b, sym_unit);
        buffer_appendFloat (b,  x->x_unitValue);
        buffer_appendSymbol (b, x->x_unitName);
    }
    
    if (x->x_unitName && x->x_argc) { buffer_appendComma (b); }
    
    if (x->x_argc) {
        buffer_appendSymbol (b, sym_arguments);
        buffer_append (b, x->x_argc, x->x_argv);
    }
    
    buffer_appendComma (b);
    //
    }
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, textclient_getName (&x->x_textclient));
    
    return b;
    //
    }
    
    return NULL;
}

static void textsequence_functionDismiss (t_object *z)
{
    textsequence_dismiss ((t_textsequence *)z);
}

static void textsequence_restore (t_textsequence *x, t_symbol *s, int argc, t_atom *argv)
{
    t_textsequence *old = (t_textsequence *)instance_pendingFetch (cast_object (x));
    
    t_symbol *name = old ? textclient_getName (&old->x_textclient) : atom_getSymbolAtIndex (0, argc, argv);
    
    textclient_setName (&x->x_textclient, name);
    
    if (old) {
    //
    if (old->x_unitName) { textsequence_unit (x, old->x_unitName, old->x_unitValue); }
    if (old->x_argc)     { textsequence_arguments (x, NULL, old->x_argc, old->x_argv); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *textsequence_new (t_symbol *s, int argc, t_atom *argv)
{
    t_textsequence *x = (t_textsequence *)pd_new (textsequence_class);
        
    t_error err = textclient_init (&x->x_textclient, &argc, &argv);
    
    if (!err) {
    
        int hasWait = 0;
        int useGlobal = 0;
        
        while (argc > 0) {
        
            t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
        
            if (t == sym___arrobe__global) {
                useGlobal = 1; argc--; argv++; 
                
            } else if (argc >= 2 && t == sym___arrobe__wait) {
                if (!x->x_waitSymbol && !x->x_waitNumberOfLeading) {
                    if (IS_SYMBOL (argv + 1)) { x->x_waitSymbol = atom_getSymbol (argv + 1); }
                    else {
                        x->x_waitNumberOfLeading = PD_MAX (0, (int)atom_getFloat (argv + 1));
                    }
                    argc -= 2; argv += 2;
                }
                
            } else if (argc >= 3 && t == sym___arrobe__unit) {
                textsequence_unit (x, atom_getSymbol (argv + 2), atom_getFloat (argv + 1));
                argc -= 3; argv += 3;
                
            } else {
                break;
            }
        }
        
        error__options (cast_object (x), s, argc, argv);

        if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
        
        hasWait = (useGlobal || x->x_waitSymbol || x->x_waitNumberOfLeading);
        
        x->x_indexOfMessage = 0;
        
        x->x_argc   = 0;
        x->x_argv   = (t_atom *)PD_MEMORY_GET (0);
        x->x_clock  = clock_newSingle ((void *)x, (t_method)textsequence_task);
        
        if (useGlobal)  { x->x_waitNumberOfLeading = PD_INT_MAX; }
        if (!useGlobal) { x->x_outletMain = outlet_newList (cast_object (x)); }
        if (hasWait)    { x->x_outletWait = outlet_newList (cast_object (x)); }
        
        x->x_outletEnd = outlet_newBang (cast_object (x));
        
        inlet_newSymbol (cast_object (x), TEXTCLIENT_NAME (&x->x_textclient));
        
    } else {
    
        error_invalidArguments (cast_object (x), sym_text__space__search, argc, argv);
        
        pd_free (cast_pd (x)); x = NULL;
    }
    
    return x;
}

static void textsequence_dismiss (t_textsequence *x)
{
    clock_unset (x->x_clock);
}

static void textsequence_free (t_textsequence *x)
{
    if (x->x_argv)  { PD_MEMORY_FREE (x->x_argv); }
    if (x->x_clock) { clock_free (x->x_clock);    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void textsequence_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_text__space__sequence,
            (t_newmethod)textsequence_new,
            (t_method)textsequence_free,
            sizeof (t_textsequence),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)textsequence_bang);
    class_addList (c, (t_method)textsequence_list);
    
    class_addMethod (c, (t_method)textsequence_stop,        sym_stop,       A_NULL);
    class_addMethod (c, (t_method)textsequence_step,        sym_step,       A_NULL);
    class_addMethod (c, (t_method)textsequence_automatic,   sym_automatic,  A_NULL);
    class_addMethod (c, (t_method)textsequence_rewind,      sym_rewind,     A_NULL);
    class_addMethod (c, (t_method)textsequence_message,     sym_message,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)textsequence_message,     sym_line,       A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)textsequence_arguments,   sym_arguments,  A_GIMME, A_NULL);
    class_addMethod (c, (t_method)textsequence_unit,        sym_unit,       A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)textsequence_unit,        sym_tempo,      A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)textsequence_restore,     sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, textsequence_functionData);
    class_setDismissFunction (c, textsequence_functionDismiss);
    class_requirePending (c);
    
    class_setHelpName (c, sym_text);
    
    textsequence_class = c;
}

void textsequence_destroy (void)
{
    class_free (textsequence_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
