
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void legacy_convert (t_buffer *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error buffer_fromFile (t_buffer *, const char *, const char *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_symbol *eval_bufferGetObject (t_atom *v, int argc, t_atom *argv)
{   
    if (IS_DOLLARSYMBOL (v))   {
        return dollar_expandSymbolWithArguments (GET_SYMBOL (v), NULL, argc, argv);
    } else if (IS_DOLLAR  (v)) {
        t_symbol *s = atom_getSymbolAtIndex (GET_DOLLAR (v) - 1, argc, argv); 
        return (s == &s_ ? NULL : s);
    }

    return atom_getSymbol (v);
}

static int eval_bufferGetMessage (t_atom *v, t_pd *object, t_pd **next, t_atom *m, int argc, t_atom *argv)
{
    t_symbol *s = NULL;
    int end = 0;
    
    switch (atom_getType (v)) {
    //
    case A_SEMICOLON    :   if (instance_isMakerObject (object)) { SET_SYMBOL (m, sym___semicolon__); }
                            else { 
                                *next = NULL; end = 1; 
                            }
                            break;
    case A_COMMA        :   if (instance_isMakerObject (object)) { SET_SYMBOL (m, sym___comma__); }
                            else { 
                                end = 1; 
                            }
                            break;
    case A_FLOAT        :   *m = *v; break;
    case A_SYMBOL       :   *m = *v; break;
    case A_DOLLAR       :   dollar_expandWithArguments (v, m, NULL, argc, argv); break;
    case A_DOLLARSYMBOL :   s = dollar_expandSymbolWithArguments (GET_SYMBOL (v), NULL, argc, argv);
                            if (s) { SET_SYMBOL (m, s); }
                            else {
                                SET_SYMBOL (m, GET_SYMBOL (v));
                            }
                            break;
    default             :   end = 1; PD_BUG; 
    //
    }
    
    return end;
}

void eval_bufferProceed (int size, t_atom *v, t_pd *object, int argc, t_atom *argv)
{
    t_atom *message = NULL;
    t_atom *m = NULL;
    t_pd *next = NULL;
    int args = 0;
    int n = size;
    
    instance_setBoundA (NULL);
    
    PD_ATOMS_ALLOCA (message, size);
    
    while (1) {
    //
    while (!object) {
        
        t_symbol *s = NULL;
        
        while (n && IS_SEMICOLON_OR_COMMA (v)) { n--; v++; }
        
        if (n) { s = eval_bufferGetObject (v, argc, argv); }
        else {
            break;
        }
        
        if (s == NULL || !(object = symbol_getThing (s))) {
            if (!s) { error_invalid (NULL, &s_, sym_expansion); }
            else {
                symbol_hasThing (s, NULL);
            }
            do { n--; v++; } while (n && !IS_SEMICOLON (v));
            
        } else {
            n--; v++; break;
        }
    }
    
    PD_ASSERT ((object != NULL) || (n == 0));
    
    m    = message;
    args = 0;
    next = object;
        
    while (1) {
        if (!n || eval_bufferGetMessage (v, object, &next, m, argc, argv)) { break; }
        else {
            m++; args++; n--; v++;
        }
    }
    
    if (args) {
        if (IS_SYMBOL (message))     { pd_message (object, GET_SYMBOL (message), args - 1, message + 1); }
        else if (IS_FLOAT (message)) {
            if (args == 1) { pd_float (object, GET_FLOAT (message)); }
            else {
                pd_list (object, args, message);
            }
        } else {
            PD_BUG;
        }
    }
    
    if (!n) { break; }
    
    object = next;
    n--;
    v++;
    //
    }
    
    PD_ATOMS_FREEA (message, size);
}

void eval_buffer (t_buffer *x, t_pd *object, int argc, t_atom *argv)
{
    eval_bufferProceed (buffer_getSize (x), buffer_getAtoms (x), object, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void eval_fileByBuffer (t_symbol *name, t_symbol *directory, t_buffer *b)
{
    int state = dsp_suspend();
    
    instance_environmentSetFile (name, directory);
    
    eval_buffer (b, NULL, 0, NULL);
    
    instance_environmentResetFile();
    
    dsp_resume (state);
}

void eval_file (t_symbol *name, t_symbol *directory)
{
    t_buffer *t = buffer_new();
    
    t_error err = buffer_fromFile (t, symbol_getName (name), symbol_getName (directory));
    
    if (err) { error_failsToRead (NULL, name); }
    else {
        legacy_convert (t); eval_fileByBuffer (name, directory, t);
    }
    
    buffer_free (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
