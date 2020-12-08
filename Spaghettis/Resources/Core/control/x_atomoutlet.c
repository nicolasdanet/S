
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_atomoutlet.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_symbol *atomoutlet_parseAbbreviated (t_symbol *s)
{
    t_symbol *t = s;
    
    if (t == sym_b)      { t = &s_bang;     }
    else if (t == sym_l) { t = &s_list;     }
    else if (t == sym_s) { t = &s_symbol;   }
    else if (t == sym_a) { t = &s_anything; }
    else if (t == sym_f) { t = &s_float;    }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_atom *atomoutlet_getAtom (t_atomoutlet *x)
{
    return &x->ao_atom;
}

PD_LOCAL t_inlet *atomoutlet_getInlet (t_atomoutlet *x)
{
    return x->ao_inlet;
}

PD_LOCAL t_outlet *atomoutlet_getOutlet (t_atomoutlet *x)
{
    return x->ao_outlet;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void atomoutlet_copyAtom (t_atomoutlet *x, t_atom *a)
{
    *a = *atomoutlet_getAtom (x);
}

PD_LOCAL t_error atomoutlet_setAtom (t_atomoutlet *x, t_atom *a)
{
    if (!atom_typesAreEquals (&x->ao_atom, a)) { return PD_ERROR; }
    else {
    //
    switch (atom_getType (&x->ao_atom)) {
        case A_FLOAT    : SET_FLOAT (&x->ao_atom,  GET_FLOAT (a));  break;
        case A_SYMBOL   : SET_SYMBOL (&x->ao_atom, GET_SYMBOL (a)); break;
        default         : PD_BUG; return PD_ERROR;
    }
    //
    }
    
    return PD_ERROR_NONE;
}

PD_LOCAL int atomoutlet_isEqualToAtom (t_atomoutlet *x, t_atom *a)
{
    if (atom_typesAreEquals (&x->ao_atom, a)) {
    //
    if (IS_FLOAT (&x->ao_atom) && (GET_FLOAT (&x->ao_atom) == GET_FLOAT (a)))    { return 1; }
    if (IS_SYMBOL (&x->ao_atom) && (GET_SYMBOL (&x->ao_atom) == GET_SYMBOL (a))) { return 1; }
    //
    }
    
    return 0;
}

PD_LOCAL t_error atomoutlet_broadcastIfTypeMatch (t_atomoutlet *x, t_atom *a)
{
    if (!atom_typesAreEquals (&x->ao_atom, a)) { return PD_ERROR; }
    else {
    //
    switch (atom_getType (&x->ao_atom)) {
        case A_FLOAT    : outlet_float (x->ao_outlet, GET_FLOAT (a));   break;
        case A_SYMBOL   : outlet_symbol (x->ao_outlet, GET_SYMBOL (a)); break;
        default         : PD_BUG; return PD_ERROR;
    }
    //
    }
    
    return PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void atomoutlet_init (t_atomoutlet *x)
{
    SET_FLOAT (&x->ao_atom, 0.0);
    x->ao_inlet  = NULL;
    x->ao_outlet = NULL;
}

PD_LOCAL void atomoutlet_makeFloat (t_atomoutlet *x, t_object *owner, int flags, t_float f)
{
    atomoutlet_init (x);
    SET_FLOAT (&x->ao_atom, f);
    if (flags & ATOMOUTLET_OUTLET) { x->ao_outlet = outlet_newMixed (owner); }
    if (flags & ATOMOUTLET_INLET)  { x->ao_inlet  = inlet_newFloat (owner, ADDRESS_FLOAT (&x->ao_atom)); }
}

PD_LOCAL void atomoutlet_makeSymbol (t_atomoutlet *x, t_object *owner, int flags, t_symbol *s)
{
    atomoutlet_init (x);
    SET_SYMBOL (&x->ao_atom, s);
    if (flags & ATOMOUTLET_OUTLET) { x->ao_outlet = outlet_newMixed (owner); }
    if (flags & ATOMOUTLET_INLET)  { x->ao_inlet  = inlet_newSymbol (owner, ADDRESS_SYMBOL (&x->ao_atom)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void atomoutlet_make (t_atomoutlet *x, t_object *owner, int flags, t_atom *a)
{
    if (IS_SYMBOL (a)) {
        atomoutlet_makeSymbol (x, owner, flags, GET_SYMBOL (a));
    } else {
        atomoutlet_makeFloat (x, owner, flags, atom_getFloat (a));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void atomoutlet_makeSymbolParsed (t_atomoutlet *x, t_object *owner, int flags, t_atom *a)
{
    t_symbol *t = atomoutlet_parseAbbreviated (atom_getSymbol (a));
    
    if (t == &s_) { t = &s_float; }
    
    atomoutlet_makeSymbol (x, owner, flags, t);
}

PD_LOCAL void atomoutlet_makeParsed (t_atomoutlet *x, t_object *owner, int flags, t_atom *a)
{
    t_symbol *t = atomoutlet_parseAbbreviated (atom_getSymbol (a));
    
    if (t == &s_symbol)      { atomoutlet_makeSymbol (x, owner, flags, t);  }
    else if (t == &s_float)  { atomoutlet_makeFloat (x, owner, flags, 0.0); }
    else                     { atomoutlet_make (x, owner, flags, a);        }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
