
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_REGISTER_H_
#define M_REGISTER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A register for all objects in order to grab them quickly and safely. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _registerelement {
    t_id                re_id;
    t_id                re_owner;
    t_object            *re_object;
    } t_registerelement;

typedef struct _register {
    size_t              r_allocated;
    size_t              r_size;
    t_rand48            r_state;
    t_registerelement   *r_raw;
    } t_register;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_register     *register_new           (void);
PD_LOCAL t_object       *register_getObject     (t_register *x, t_id u);
PD_LOCAL t_glist        *register_getOwner      (t_register *x, t_id u);

PD_LOCAL void           register_free           (t_register *x);
PD_LOCAL void           register_add            (t_register *x, t_id u, t_object *o, t_glist *owner);
PD_LOCAL t_error        register_remove         (t_register *x, t_id u);
PD_LOCAL void           register_rename         (t_register *x, t_id u, t_id newUnique);
PD_LOCAL int            register_contains       (t_register *x, t_id u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_REGISTER_H_
