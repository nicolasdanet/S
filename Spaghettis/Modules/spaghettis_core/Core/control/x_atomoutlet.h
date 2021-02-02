
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef X_ATOMOUTLET_H_
#define X_ATOMOUTLET_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _atomoutlet {
    t_atom      ao_atom;
    t_inlet     *ao_inlet;
    t_outlet    *ao_outlet;
    } t_atomoutlet;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    ATOMOUTLET_NONE     = 0,
    ATOMOUTLET_INLET    = 1,
    ATOMOUTLET_OUTLET   = 2,
    ATOMOUTLET_BOTH     = 3
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol   *atomoutlet_parseAbbreviated    (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_atom     *atomoutlet_getAtom             (t_atomoutlet *x);
PD_LOCAL t_inlet    *atomoutlet_getInlet            (t_atomoutlet *x);
PD_LOCAL t_outlet   *atomoutlet_getOutlet           (t_atomoutlet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       atomoutlet_copyAtom             (t_atomoutlet *x, t_atom *a);
PD_LOCAL t_error    atomoutlet_setAtom              (t_atomoutlet *x, t_atom *a);
PD_LOCAL int        atomoutlet_isEqualToAtom        (t_atomoutlet *x, t_atom *a);
PD_LOCAL t_error    atomoutlet_broadcastIfTypeMatch (t_atomoutlet *x, t_atom *a);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       atomoutlet_makeFloat            (t_atomoutlet *x, t_object *o, int flags, t_float f);
PD_LOCAL void       atomoutlet_makeSymbol           (t_atomoutlet *x, t_object *o, int flags, t_symbol *s);
PD_LOCAL void       atomoutlet_make                 (t_atomoutlet *x, t_object *o, int flags, t_atom *a);

PD_LOCAL void       atomoutlet_makeSymbolParsed     (t_atomoutlet *x, t_object *o, int flags, t_atom *a);
PD_LOCAL void       atomoutlet_makeParsed           (t_atomoutlet *x, t_object *o, int flags, t_atom *a);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // X_ATOMOUTLET_H_
