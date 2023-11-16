
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

t_inlet    *vinlet_getInlet            (t_pd *x);
t_outlet   *voutlet_getOutlet          (t_pd *x);

int        vinlet_isSignal             (t_vinlet *x);
int        voutlet_isSignal            (t_voutlet *x);

int        vinlet_getIndex             (t_vinlet *x);
int        voutlet_getIndex            (t_voutlet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       comment_makeObject          (t_glist *glist, t_symbol *s, int argc, t_atom *argv);
void       message_makeObject          (t_glist *glist, t_symbol *s, int argc, t_atom *argv);
void       gatom_makeObject            (t_glist *glist, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_garray   *garray_fetch               (t_symbol *s);

int        garray_getSize              (t_garray *x);
void       garray_getData              (t_garray *x, int *size, t_word **w);
void       garray_setFloatAtIndex      (t_garray *x, int i, t_float f);
t_float    garray_getFloatAtIndex      (t_garray *x, int i);
void       garray_setFloatFromIndex    (t_garray *x, int i, t_float f);
t_float    garray_getAmplitude         (t_garray *x);
void       garray_setAsUsedInDsp       (t_garray *x, int usedInDsp);
int        garray_isUsedInDsp          (t_garray *x);
void       garray_resize               (t_garray *x, t_float f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_OBJECTS_H_
