
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef X_LIST_H_
#define X_LIST_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _listinletelement {
    t_atom              le_atom;
    } t_listinletelement;

typedef struct _listinlet {
    t_pd                li_pd;                  /* MUST be the first. */
    int                 li_size;
    t_listinletelement  *li_vector;
    } t_listinlet;

typedef struct _listinlethelper {
    t_object            lh_obj;                 /* Must be the first. */
    t_listinlet         lh_listinlet;
    } t_listinlethelper;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Trick to avoid duplication of code. */

typedef struct _listappend {
    t_listinlethelper   x_h;                    /* Must be the first. */
    t_outlet            *x_outlet;
    t_outlet            *x_outletRight;
    } t_listappend;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef t_listappend t_listchange;
typedef t_listappend t_listequal;
typedef t_listappend t_listprepend;
typedef t_listappend t_liststore;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   listinlet_init                  (t_listinlet *x);
PD_LOCAL void   listinlet_free                  (t_listinlet *x);
PD_LOCAL void   listinlet_copyAtomsUnchecked    (t_listinlet *x, t_atom *a);

PD_LOCAL t_atom     *listinlet_getAtomAtIndex   (t_listinlet *x, int i);

PD_LOCAL void   listinlet_listClear             (t_listinlet *x);
PD_LOCAL void   listinlet_listSet               (t_listinlet *x, int argc, t_atom *argv);
PD_LOCAL void   listinlet_listAppend            (t_listinlet *x, int argc, t_atom *argv);
PD_LOCAL void   listinlet_listPrepend           (t_listinlet *x, int argc, t_atom *argv);
PD_LOCAL void   listinlet_listSetAt             (t_listinlet *x, int i, int argc, t_atom *argv);
PD_LOCAL void   listinlet_listInsert            (t_listinlet *x, int i, int argc, t_atom *argv);
PD_LOCAL void   listinlet_listRemove            (t_listinlet *x, int i, int n);
PD_LOCAL void   listinlet_listReplace           (t_listinlet *x, int i, int n, int argc, t_atom *argv);
PD_LOCAL int    listinlet_listIsEqualTo         (t_listinlet *x, int argc, t_atom *argv);
PD_LOCAL void   listinlet_listGet               (t_listinlet *x, t_buffer *b);
PD_LOCAL void   listinlet_listSetByCopy         (t_listinlet *x, t_listinlet *toCopy);
PD_LOCAL int    listinlet_listSize              (t_listinlet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   *listappend_new                 (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listchange_new                 (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listequal_new                  (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listfromsymbol_new             (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listgroup_new                  (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listiterate_new                (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listlength_new                 (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listprepend_new                (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listreverse_new                (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listrotate_new                 (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listscramble_new               (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listsort_new                   (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listsplit_new                  (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *liststore_new                  (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *liststream_new                 (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listsum_new                    (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listtosymbol_new               (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   *listtrim_new                   (t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer   *listhelper_functionData    (t_object *z, int flags);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void   listhelper_restore              (t_listinlethelper *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   listappend_list                 (t_listappend *x, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void   listappend_anything             (t_listappend *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // X_LIST_H_
