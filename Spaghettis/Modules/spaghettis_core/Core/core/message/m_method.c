
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void (*t_gimme)         (t_pd *, t_symbol *, int, t_atom *);
typedef t_pd *(*t_newgimme)     (t_symbol *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void (*t_method10)      (t_int);
typedef void (*t_method11)      (t_int, t_float);
typedef void (*t_method12)      (t_int, t_float, t_float);
typedef void (*t_method20)      (t_int, t_int);
typedef void (*t_method21)      (t_int, t_int, t_float);
typedef void (*t_method30)      (t_int, t_int, t_int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef t_pd *(*t_newmethod00)  (void);
typedef t_pd *(*t_newmethod01)  (t_float);
typedef t_pd *(*t_newmethod02)  (t_float, t_float);
typedef t_pd *(*t_newmethod10)  (t_int);
typedef t_pd *(*t_newmethod11)  (t_int, t_float);
typedef t_pd *(*t_newmethod20)  (t_int, t_int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define METHOD_COMBINE(m, n)    (((m) << 2) | (n))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void method_execute (t_pd *x, t_method f, int m, t_int *ai, int n, t_float *af)
{
    if (instance_isMakerObject (x)) {

        t_pd *o = NULL;

        switch (METHOD_COMBINE (m, n)) {
        //
        case METHOD_COMBINE (0, 0)  : o = (*(t_newmethod00)f)();                break;
        case METHOD_COMBINE (0, 1)  : o = (*(t_newmethod01)f) (af[0]);          break;
        case METHOD_COMBINE (0, 2)  : o = (*(t_newmethod02)f) (af[0], af[1]);   break;
        case METHOD_COMBINE (1, 0)  : o = (*(t_newmethod10)f) (ai[0]);          break;
        case METHOD_COMBINE (1, 1)  : o = (*(t_newmethod11)f) (ai[0], af[0]);   break;
        case METHOD_COMBINE (2, 0)  : o = (*(t_newmethod20)f) (ai[0], ai[1]);   break;
        default : PD_BUG;
        //
        }

        instance_objectSetNewest (o);

    } else {

        /* In that case adress of the object is passed at first. */

        switch (METHOD_COMBINE (m, n)) {
        //
        case METHOD_COMBINE (1, 0)  : (*(t_method10)f) (ai[0]);                 break;
        case METHOD_COMBINE (1, 1)  : (*(t_method11)f) (ai[0], af[0]);          break;
        case METHOD_COMBINE (1, 2)  : (*(t_method12)f) (ai[0], af[0], af[1]);   break;
        case METHOD_COMBINE (2, 0)  : (*(t_method20)f) (ai[0], ai[1]);          break;
        case METHOD_COMBINE (2, 1)  : (*(t_method21)f) (ai[0], ai[1], af[0]);   break;
        case METHOD_COMBINE (3, 0)  : (*(t_method30)f) (ai[0], ai[1], ai[2]);   break;
        default : PD_BUG;
        //
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that A_FLOAT arguments are always passed at last. */
/* A zero number can be converted to an empty symbol (invalid substitutions in abstractions). */

static t_error method_entryTyped (t_entry *e, t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    t_atomtype t;
    t_atomtype *p = e->me_arguments;
    t_method f = e->me_method;
    t_int   ai[PD_ARGUMENTS + 1] = { 0 };
    t_float af[PD_ARGUMENTS] = { 0 };
    t_int   *ip = ai;
    t_float *fp = af;
    int m = 0;
    int n = 0;

    if (!instance_isMakerObject (x)) { *ip = (t_int)x; ip++; m++; }
    
    if (argc > PD_ARGUMENTS) {
    //
    warning_unusedArguments (pd_asObjectIfBox (x), s, argc - PD_ARGUMENTS, argv + PD_ARGUMENTS);
        
    argc = PD_ARGUMENTS;
    //
    }

    while ((t = *p++)) {
    //
    switch (t) {
    //
    case A_FLOAT     :  if (!argc) { return PD_ERROR; }         /* Falls through. */
    case A_DEFFLOAT  :  if (!argc) { *fp = 0.0; }
                        else {
                            if (IS_FLOAT (argv)) { *fp = GET_FLOAT (argv); }
                            else {
                                return PD_ERROR;
                            }
                            argc--; argv++;
                        }
                        n++; fp++; break;

    case A_SYMBOL    :  if (!argc) { return PD_ERROR;  }        /* Falls through. */
    case A_DEFSYMBOL :  if (!argc) { *ip = (t_int)&s_; }
                        else {
                            if (IS_SYMBOL (argv)) { *ip = (t_int)GET_SYMBOL (argv); }
                            else if (IS_FLOAT (argv) && GET_FLOAT (argv) == 0.0) { *ip = (t_int)&s_; }
                            else {
                                return PD_ERROR;
                            }
                            argc--; argv++;
                        }
                        m++; ip++; break;

    default          :  return PD_ERROR;
    //
    }
    //
    }

    method_execute (x, f, m, ai, n, af);

    if (instance_isMakerObject (x)) {
    if (argc) {
        warning_unusedArguments (NULL, s, argc, argv);
    }
    }

    return PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The A_GIMME signature should always be prefered now. */

static t_error method_entry (t_entry *e, t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    t_atomtype *p = e->me_arguments;

    if (*p != A_GIMME) { return method_entryTyped (e, x, s, argc, argv); }
    else {
        if (!instance_isMakerObject (x)) { (*((t_gimme)e->me_method)) (x, s, argc, argv); }
        else {
            t_pd *t = ((*((t_newgimme)e->me_method)) (s, argc, argv));

            instance_objectSetNewest (t);
        }
    }

    return PD_ERROR_NONE;
}

static t_error method_slot (t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;

    t_class *c = pd_class (x);

    if (s == &s_float) {
        if (argc && IS_FLOAT (argv)) { (*(class_getFloatMethod (c))) (x, GET_FLOAT (argv)); }
        else {
            if (!argc) { (*(class_getFloatMethod (c))) (x, 0.0); }
            else {
                err = PD_ERROR;
            }
        }

    } else if (s == &s_bang)   {
        (*(class_getBangMethod (c))) (x);

    } else if (s == &s_list)   {
        (*(class_getListMethod (c))) (x, s, argc, argv);

    } else if (s == &s_symbol) {
        if (argc && IS_SYMBOL (argv)) { (*(class_getSymbolMethod (c))) (x, GET_SYMBOL (argv)); }
        else {
            int k = (!argc) || (argc && IS_FLOAT (argv) && GET_FLOAT (argv) == 0.0);
            if (k) { (*(class_getSymbolMethod (c))) (x, &s_); }
            else {
                err = PD_ERROR;
            }
        }
    }

    if (!err && instance_isMakerObject (x)) {
    //
    if (argc > 0 && s == &s_bang)       { warning_unusedArguments (NULL, s, argc, argv); }
    else if (argc > 1 && s != &s_list)  { warning_unusedArguments (NULL, s, argc - 1, argv + 1); }
    //
    }

    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void pd_message (t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;

    if (s == &s_bang || s == &s_float || s == &s_symbol || s == &s_list) {
        err = method_slot (x, s, argc, argv);
        if (!err) {
            return;
        }
    }

    if (!err) {

        t_class *c = pd_class (x);
        t_entry *m = NULL;
        int i;

        for (i = c->c_methodsSize, m = c->c_methods; i--; m++) {
            if (m->me_name == s) {
                err = method_entry (m, x, s, argc, argv);
                if (!err) { return; }
                else {
                    break;
                }
            }
        }

        if (!err) {
            (*(class_getAnythingMethod (c))) (x, s, argc, argv);
            return;
        }
    }

    error_invalidArguments (pd_asObjectIfBox (x), s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void pd_bang (t_pd *x)
{
    (*(class_getBangMethod (pd_class (x)))) (x);
}

PD_LOCAL void pd_float (t_pd *x, t_float f)
{
    (*(class_getFloatMethod (pd_class (x)))) (x, f);
}

PD_LOCAL void pd_symbol (t_pd *x, t_symbol *s)
{
    (*(class_getSymbolMethod (pd_class (x)))) (x, s);
}

PD_LOCAL void pd_list (t_pd *x, int argc, t_atom *argv)
{
    (*(class_getListMethod (pd_class (x)))) (x, &s_list, argc, argc ? argv : NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
