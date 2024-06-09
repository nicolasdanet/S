
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_EXTERNALS_H_
#define M_EXTERNALS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Functions exported (with wrappers) for plugins. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol    *gensym                     (const char *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd    *pd_new                         (t_class *c);

void    pd_free                         (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_object    *pd_asObjectIfBox           (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void    pd_bind                         (t_pd *x, t_symbol *s);
void    pd_unbind                       (t_pd *x, t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void    pd_bang                         (t_pd *x);
void    pd_float                        (t_pd *x, t_float f);
void    pd_symbol                       (t_pd *x, t_symbol *s);
void    pd_list                         (t_pd *x, int argc, t_atom *argv);
void    pd_message                      (t_pd *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_pd    *symbol_getThing                (t_symbol *s);
t_pd    *symbol_getThingByClass         (t_symbol *s, t_class *c);

const char *symbol_getName              (t_symbol *s);

int     symbol_hasThing                 (t_symbol *s, t_object *o);
int     symbol_hasThingQuiet            (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *class_new                      (t_symbol *name,
                                            t_newmethod newMethod,
                                            t_method freeMethod,
                                            size_t size,
                                            int flags,
                                            t_atomtype type1, ...);

void    class_addCreator                (t_newmethod newMethod, t_symbol *s, t_atomtype type1, ...);

void    class_addMethod                 (t_class *c, t_method fn, t_symbol *s, t_atomtype type1, ...);
void    class_free                      (t_class *c);

void    class_addBang                   (t_class *c, t_method fn);
void    class_addFloat                  (t_class *c, t_method fn);
void    class_addSymbol                 (t_class *c, t_method fn);
void    class_addList                   (t_class *c, t_method fn);
void    class_addAnything               (t_class *c, t_method fn);

void    class_setHelpName               (t_class *c, t_symbol *s);
void    class_setHelpDirectory          (t_class *c, t_symbol *s);

void    class_setDataFunction           (t_class *c, t_datafn f);
void    class_setDismissFunction        (t_class *c, t_dismissfn f);

void    class_requirePending            (t_class *c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int     object_dspNeedInitializer       (t_object *x);

void    object_getSignalValues          (t_object *x, t_buffer *b);
void    object_copySignalValues         (t_object *x, t_object *old);

void    object_fetchAndCopySignalValuesIfRequired   (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_inlet     *inlet_newFloat             (t_object *owner, t_float *fp);
t_inlet     *inlet_newSymbol            (t_object *owner, t_symbol **sp);
t_inlet     *inlet_newSignal            (t_object *owner);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_outlet    *outlet_newBang             (t_object *owner);
t_outlet    *outlet_newFloat            (t_object *owner);
t_outlet    *outlet_newSymbol           (t_object *owner);
t_outlet    *outlet_newList             (t_object *owner);
t_outlet    *outlet_newAnything         (t_object *owner);
t_outlet    *outlet_newMixed            (t_object *owner);
t_outlet    *outlet_newSignal           (t_object *owner);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void    outlet_bang                     (t_outlet *x);
void    outlet_float                    (t_outlet *x, t_float f);
void    outlet_symbol                   (t_outlet *x, t_symbol *s);
void    outlet_list                     (t_outlet *x, int argc, t_atom *argv);
void    outlet_anything                 (t_outlet *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        atom_setSymbol              (t_atom *a, t_symbol *s);
void        atom_setFloat               (t_atom *a, t_float f);

t_float     atom_getFloat               (t_atom *a);
t_float     atom_getFloatAtIndex        (int n, int argc, t_atom *argv);

t_symbol    *atom_getSymbol             (t_atom *a);
t_symbol    *atom_getSymbolAtIndex      (int n, int argc, t_atom *argv);

t_symbol    *atom_getSymbolOrDollarSymbol           (t_atom *a);
t_symbol    *atom_getSymbolOrDollarSymbolAtIndex    (int n, int argc, t_atom *argv);

char        *atom_atomsToString         (int argc, t_atom *argv);           /* Caller acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer    *buffer_new                     (void);
t_buffer    *buffer_newCopy                 (t_buffer *toCopy);
t_atom      *buffer_getAtoms                (t_buffer *x);
t_atom      *buffer_getAtomAtIndex          (t_buffer *x, int n);
t_atom      *buffer_getAtomAtIndexChecked   (t_buffer *x, int n);

int         buffer_getSize                  (t_buffer *x);
void        buffer_free                     (t_buffer *x);
void        buffer_clear                    (t_buffer *x);
void        buffer_shuffle                  (t_buffer *x);

void        buffer_reserve                  (t_buffer *x, int n);
void        buffer_resize                   (t_buffer *x, int n);
t_error     buffer_extend                   (t_buffer *x, int start, int end, int n);
t_error     buffer_replace                  (t_buffer *x, int start, int end, int argc, t_atom *argv);
t_error     buffer_getAtIndex               (t_buffer *x, int n, t_atom *a);
t_error     buffer_setAtIndex               (t_buffer *x, int n, t_atom *a);
t_error     buffer_insertAtIndex            (t_buffer *x, int n, t_atom *a);
t_error     buffer_removeAtIndex            (t_buffer *x, int n);
t_error     buffer_pop                      (t_buffer *x, t_atom *a);

//t_error   buffer_setFloatAtIndex          (t_buffer *x, int n, t_float f);
t_error     buffer_setSymbolAtIndex         (t_buffer *x, int n, t_symbol *s);

t_float     buffer_getFloatAtIndex          (t_buffer *x, int n);
t_symbol    *buffer_getSymbolAtIndex        (t_buffer *x, int n);

void        buffer_prepend                  (t_buffer *x, int argc, t_atom *argv);
void        buffer_append                   (t_buffer *x, int argc, t_atom *argv);
void        buffer_appendAtom               (t_buffer *x, t_atom *a);
void        buffer_appendBuffer             (t_buffer *x, t_buffer *y);
void        buffer_appendFloat              (t_buffer *x, t_float f);
void        buffer_appendSymbol             (t_buffer *x, t_symbol *s);
void        buffer_appendDollar             (t_buffer *x, int n);
void        buffer_appendDollarSymbol       (t_buffer *x, t_symbol *s);
void        buffer_appendSemicolon          (t_buffer *x);
void        buffer_appendComma              (t_buffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that unset and delay a clock MUST be done only in one (and always the same) thread. */
/* It can be inside the DSP perform. */
/* The associated task is executed in the main thread. */
/* The new and free functions must be called NON-CONCURRENTLY in the main thread. */

t_clock     *clock_newSafe                  (void *owner, t_method fn);

/* If the clock is used only in main thread. */

t_clock     *clock_newSingle                (void *owner, t_method fn);

void        clock_free                      (t_clock *x);
void        clock_unset                     (t_clock *x);                   /* Usable in DSP. */
void        clock_delay                     (t_clock *x, double delay);     /* Usable in DSP. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* First parameter not used for now (but could in the future to reveal the caller). */

void    post                                (t_object *x, const char *fmt, ...);
void    post_system                         (t_object *x, const char *fmt, ...);
void    post_warning                        (t_object *x, const char *fmt, ...);
void    post_error                          (t_object *x, const char *fmt, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_glist *instance_contextGetCurrent (void);
t_chain *instance_chainGetTemporary (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void    chain_append                        (t_chain *x, t_perform f, int n, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_initializer   *initializer_new            (t_initializerfn fn, void *lhs, void *rhs);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif  // M_EXTERNALS_H_
