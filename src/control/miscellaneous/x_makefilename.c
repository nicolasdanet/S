
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *makefilename_class;                 /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _makefilename {
    t_object    x_obj;                              /* Must be the first. */
    t_atomtype  x_typeRequired;
    int         x_isIntegerCastRequired;
    t_symbol    *x_format;
    t_outlet    *x_outlet;
    } t_makefilename;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that for now complex and multiple substitutions are rejected. */

static t_error makefilename_scanFormat (t_makefilename *x)
{
    t_error err = PD_ERROR;
    
    if (x->x_format) {
    //
    int k = 0; const char *s = NULL;
    
    x->x_typeRequired = A_NULL; err = PD_ERROR_NONE;
    
    for (s = x->x_format->s_name; *s; s++) {
    
        if (string_containsOccurrenceAtStart (s, "%") && k == 0) {
            if (x->x_typeRequired == A_NULL) { k = 1; }
            else {
                err = PD_ERROR; break;
            }
            
        } else if (k) {
            if (string_containsOccurrenceAtStart (s, "s")) {
                k = 0; x->x_isIntegerCastRequired = 0; x->x_typeRequired = A_SYMBOL;

            } else if (string_containsOccurrenceAtStart (s, "fgGeE")) {
                k = 0; x->x_isIntegerCastRequired = 0; x->x_typeRequired = A_FLOAT;
                
            } else if (string_containsOccurrenceAtStart (s, "xXdiouc")) {
                k = 0; x->x_isIntegerCastRequired = 1; x->x_typeRequired = A_FLOAT;
            
            } else if (string_containsOccurrenceAtStart (s, "%")) {
                k = 0;
                
            } else {
                err = PD_ERROR; break;
            }
        }
    }
    //
    }
    
    if (err) { x->x_typeRequired = A_NULL; }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void makefilename_float (t_makefilename *x, t_float f)
{
    t_error err = PD_ERROR;
  
    char name[PD_STRING] = { 0 };
    
    if (x->x_typeRequired == A_NULL)  { err = string_copy (name, PD_STRING, x->x_format->s_name); }
    
    if (x->x_typeRequired == A_FLOAT) {
        if (x->x_isIntegerCastRequired) { 
            err = string_sprintf (name, PD_STRING, x->x_format->s_name, (int)f); 
        } else {
            err = string_sprintf (name, PD_STRING, x->x_format->s_name, f);
        }
    } 
    
    if (!err) { outlet_symbol (x->x_outlet, gensym (name)); }
    else {
        error_invalid (sym_makefilename, sym_substitution);
    }
}

static void makefilename_symbol (t_makefilename *x, t_symbol *s)
{
    t_error err = PD_ERROR;
    
    char name[PD_STRING] = { 0 };
    
    if (x->x_typeRequired == A_NULL)   { err = string_copy (name, PD_STRING, x->x_format->s_name); }
    if (x->x_typeRequired == A_SYMBOL) {
        err = string_sprintf (name, PD_STRING, x->x_format->s_name, s->s_name);
    }
        
    if (!err) { outlet_symbol (x->x_outlet, gensym (name)); }
    else {
        error_invalid (sym_makefilename, sym_substitution);
    }
}

static void makefilename_list (t_makefilename *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc && IS_FLOAT (argv) && (argc == 1 || x->x_typeRequired == A_FLOAT)) {
        makefilename_float (x, GET_FLOAT (argv));
    } else {
        makefilename_symbol (x, symbol_withAtoms (argc, argv));
    }
}

static void makefilename_anything (t_makefilename *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)makefilename_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void makefilename_set (t_makefilename *x, t_symbol *dummy, int argc, t_atom *argv)
{
    x->x_format = symbol_withAtoms (argc, argv);
    
    if (makefilename_scanFormat (x)) { error_invalid (sym_makefilename, sym_format); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *makefilename_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_makefilename *x = (t_makefilename *)z;
    t_buffer *b = buffer_new();
    
    if (x->x_typeRequired != A_NULL) {
    //
    buffer_appendSymbol (b, sym_set);
    buffer_appendSymbol (b, x->x_format);
    buffer_appendComma (b);
    //
    }
    
    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

static void makefilename_restore (t_makefilename *x)
{
    t_makefilename *old = (t_makefilename *)instance_pendingFetch (cast_object (x));

    if (old) {
    //
    x->x_typeRequired          = old->x_typeRequired;
    x->x_isIntegerCastRequired = old->x_isIntegerCastRequired;
    x->x_format                = old->x_format;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *makefilename_new (t_symbol *dummy, int argc, t_atom *argv)
{
    t_makefilename *x = (t_makefilename *)pd_new (makefilename_class);
    
    t_symbol *t = symbol_withAtoms (argc, argv);
    
    if (t == &s_) { t = sym___percent__s; }
        
    x->x_typeRequired           = A_NULL;
    x->x_isIntegerCastRequired  = 0;
    x->x_format                 = t;
    x->x_outlet                 = outlet_newSymbol (cast_object (x));
    
    if (makefilename_scanFormat (x)) { error_invalid (sym_makefilename, sym_format); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void makefilename_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_makefilename,
            (t_newmethod)makefilename_new,
            NULL,
            sizeof (t_makefilename),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addFloat (c, (t_method)makefilename_float);
    class_addSymbol (c, (t_method)makefilename_symbol);
    class_addList (c, (t_method)makefilename_list);
    class_addAnything (c, (t_method)makefilename_anything);
    
    class_addMethod (c, (t_method)makefilename_set,     sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)makefilename_restore, sym__restore,   A_NULL);
    
    class_setDataFunction (c, makefilename_functionData);
    class_requirePending (c);

    makefilename_class = c;
}

PD_LOCAL void makefilename_destroy (void)
{
    class_free (makefilename_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
