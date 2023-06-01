
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef G_OBJECTS_H_
#define G_OBJECTS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_inlet    *vinlet_getInlet            (t_pd *x);
PD_LOCAL t_outlet   *voutlet_getOutlet          (t_pd *x);

PD_LOCAL int        vinlet_isSignal             (t_vinlet *x);
PD_LOCAL int        voutlet_isSignal            (t_voutlet *x);

PD_LOCAL int        vinlet_getIndex             (t_vinlet *x);
PD_LOCAL int        voutlet_getIndex            (t_voutlet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       comment_makeObject          (t_glist *glist, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       message_makeObject          (t_glist *glist, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       gatom_makeObject            (t_glist *glist, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_garray   *garray_fetch               (t_symbol *s);

PD_LOCAL int        garray_getSize              (t_garray *x);
PD_LOCAL void       garray_getData              (t_garray *x, int *size, t_word **w);
PD_LOCAL void       garray_setFloatAtIndex      (t_garray *x, int i, t_float f);
PD_LOCAL t_float    garray_getFloatAtIndex      (t_garray *x, int i);
PD_LOCAL void       garray_setFloatFromIndex    (t_garray *x, int i, t_float f);
PD_LOCAL t_float    garray_getAmplitude         (t_garray *x);
PD_LOCAL void       garray_setAsUsedInDsp       (t_garray *x, int usedInDsp);
PD_LOCAL int        garray_isUsedInDsp          (t_garray *x);
PD_LOCAL void       garray_resize               (t_garray *x, t_float f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_OBJECTS_H_
