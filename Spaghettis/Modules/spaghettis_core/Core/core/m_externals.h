
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

/* Handy to track unused functions. */

#ifndef PD_LOCAL
#define PD_LOCAL
#endif

#ifndef PD_FORCE
#define PD_FORCE        /* Avoid warnings for GUI only functions. */
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Functions exported (with wrappers) for plugins. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol   *gensym                     (const char *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pd   *pd_new                         (t_class *c);

PD_LOCAL void   pd_free                         (t_pd *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void   pd_bind                         (t_pd *x, t_symbol *s);
PD_LOCAL void   pd_unbind                       (t_pd *x, t_symbol *s);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void   pd_bang                         (t_pd *x);
PD_LOCAL void   pd_float                        (t_pd *x, t_float f);
PD_LOCAL void   pd_symbol                       (t_pd *x, t_symbol *s);
PD_LOCAL void   pd_list                         (t_pd *x, int argc, t_atom *argv);
PD_LOCAL void   pd_message                      (t_pd *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_pd   *symbol_getThing                (t_symbol *s);
PD_LOCAL t_pd   *symbol_getThingByClass         (t_symbol *s, t_class *c);

PD_LOCAL const char *symbol_getName             (t_symbol *s);

PD_LOCAL int    symbol_hasThing                 (t_symbol *s);
PD_LOCAL int    symbol_hasThingQuiet            (t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_class    *class_new                  (t_symbol *name,
                                                    t_newmethod newMethod,
                                                    t_method freeMethod,
                                                    size_t size,
                                                    int flags,
                                                    t_atomtype type1, ...);

PD_LOCAL void   class_addCreator                (t_newmethod newMethod, t_symbol *s, t_atomtype type1, ...);

PD_LOCAL void   class_addMethod                 (t_class *c, t_method fn, t_symbol *s, t_atomtype type1, ...);
PD_LOCAL void   class_free                      (t_class *c);

PD_LOCAL void   class_addBang                   (t_class *c, t_method fn);
PD_LOCAL void   class_addFloat                  (t_class *c, t_method fn);
PD_LOCAL void   class_addSymbol                 (t_class *c, t_method fn);
PD_LOCAL void   class_addList                   (t_class *c, t_method fn);
PD_LOCAL void   class_addAnything               (t_class *c, t_method fn);

PD_LOCAL void   class_setHelpName               (t_class *c, t_symbol *s);
PD_LOCAL void   class_setHelpDirectory          (t_class *c, t_symbol *s);

PD_LOCAL void   class_setDataFunction           (t_class *c, t_datafn f);
PD_LOCAL void   class_setDismissFunction        (t_class *c, t_dismissfn f);

PD_LOCAL void   class_requirePending            (t_class *c);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    object_dspNeedInitializer       (t_object *x);

PD_LOCAL void   object_getSignalValues          (t_object *x, t_buffer *b);
PD_LOCAL void   object_copySignalValues         (t_object *x, t_object *old);

PD_LOCAL void   object_fetchAndCopySignalValuesIfRequired   (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_inlet    *inlet_newFloat             (t_object *owner, t_float *fp);
PD_LOCAL t_inlet    *inlet_newSymbol            (t_object *owner, t_symbol **sp);
PD_LOCAL t_inlet    *inlet_newSignal            (t_object *owner);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_outlet   *outlet_newBang             (t_object *owner);
PD_LOCAL t_outlet   *outlet_newFloat            (t_object *owner);
PD_LOCAL t_outlet   *outlet_newSymbol           (t_object *owner);
PD_LOCAL t_outlet   *outlet_newList             (t_object *owner);
PD_LOCAL t_outlet   *outlet_newAnything         (t_object *owner);
PD_LOCAL t_outlet   *outlet_newMixed            (t_object *owner);
PD_LOCAL t_outlet   *outlet_newSignal           (t_object *owner);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   outlet_bang                     (t_outlet *x);
PD_LOCAL void   outlet_float                    (t_outlet *x, t_float f);
PD_LOCAL void   outlet_symbol                   (t_outlet *x, t_symbol *s);
PD_LOCAL void   outlet_list                     (t_outlet *x, int argc, t_atom *argv);
PD_LOCAL void   outlet_anything                 (t_outlet *x, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       atom_setSymbol              (t_atom *a, t_symbol *s);
PD_LOCAL void       atom_setFloat               (t_atom *a, t_float f);

PD_LOCAL t_float    atom_getFloat               (t_atom *a);
PD_LOCAL t_float    atom_getFloatAtIndex        (int n, int argc, t_atom *argv);

PD_LOCAL t_symbol   *atom_getSymbol             (t_atom *a);
PD_LOCAL t_symbol   *atom_getSymbolAtIndex      (int n, int argc, t_atom *argv);

PD_LOCAL t_symbol   *atom_getSymbolOrDollarSymbol           (t_atom *a);
PD_LOCAL t_symbol   *atom_getSymbolOrDollarSymbolAtIndex    (int n, int argc, t_atom *argv);

PD_LOCAL char       *atom_atomsToString         (int argc, t_atom *argv);   /* Caller acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer   *buffer_new                     (void);
PD_LOCAL t_buffer   *buffer_newCopy                 (t_buffer *toCopy);
PD_LOCAL t_atom     *buffer_getAtoms                (t_buffer *x);
PD_LOCAL t_atom     *buffer_getAtomAtIndex          (t_buffer *x, int n);
PD_LOCAL t_atom     *buffer_getAtomAtIndexChecked   (t_buffer *x, int n);

PD_LOCAL int        buffer_getSize              (t_buffer *x);
PD_LOCAL void       buffer_free                 (t_buffer *x);
PD_LOCAL void       buffer_clear                (t_buffer *x);
PD_LOCAL void       buffer_shuffle              (t_buffer *x);

PD_LOCAL void       buffer_reserve              (t_buffer *x, int n);
PD_LOCAL void       buffer_resize               (t_buffer *x, int n);
PD_LOCAL t_error    buffer_extend               (t_buffer *x, int start, int end, int n);
PD_LOCAL t_error    buffer_replace              (t_buffer *x, int start, int end, int argc, t_atom *argv);
PD_LOCAL t_error    buffer_getAtIndex           (t_buffer *x, int n, t_atom *a);
PD_LOCAL t_error    buffer_setAtIndex           (t_buffer *x, int n, t_atom *a);
PD_LOCAL t_error    buffer_insertAtIndex        (t_buffer *x, int n, t_atom *a);
PD_LOCAL t_error    buffer_pop                  (t_buffer *x, t_atom *a);

//PD_LOCAL t_error    buffer_setFloatAtIndex    (t_buffer *x, int n, t_float f);
PD_LOCAL t_error    buffer_setSymbolAtIndex     (t_buffer *x, int n, t_symbol *s);

PD_LOCAL t_float    buffer_getFloatAtIndex      (t_buffer *x, int n);
PD_LOCAL t_symbol   *buffer_getSymbolAtIndex    (t_buffer *x, int n);

PD_LOCAL void       buffer_prepend              (t_buffer *x, int argc, t_atom *argv);
PD_LOCAL void       buffer_append               (t_buffer *x, int argc, t_atom *argv);
PD_LOCAL void       buffer_appendAtom           (t_buffer *x, t_atom *a);
PD_LOCAL void       buffer_appendBuffer         (t_buffer *x, t_buffer *y);
PD_LOCAL void       buffer_appendFloat          (t_buffer *x, t_float f);
PD_LOCAL void       buffer_appendSymbol         (t_buffer *x, t_symbol *s);
PD_LOCAL void       buffer_appendDollar         (t_buffer *x, int n);
PD_LOCAL void       buffer_appendDollarSymbol   (t_buffer *x, t_symbol *s);
PD_LOCAL void       buffer_appendSemicolon      (t_buffer *x);
PD_LOCAL void       buffer_appendComma          (t_buffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that unset and delay a clock MUST be done only in one (and always the same) thread. */
/* It can be inside the DSP perform. */
/* The new and free functions must be called NON-CONCURRENTLY in the main thread. */

PD_LOCAL t_clock    *clock_new                  (void *owner, t_method fn);

PD_LOCAL void       clock_free                  (t_clock *x);
PD_LOCAL void       clock_unset                 (t_clock *x);                   /* Usable in DSP. */
PD_LOCAL void       clock_delay                 (t_clock *x, double delay);     /* Usable in DSP. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* First parameter not used for now (but could in the future to reveal the caller). */

PD_LOCAL void   post                            (t_object *x, const char *fmt, ...);
PD_LOCAL void   post_system                     (t_object *x, const char *fmt, ...);
PD_LOCAL void   post_warning                    (t_object *x, const char *fmt, ...);
PD_LOCAL void   post_error                      (t_object *x, const char *fmt, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_glist    *instance_contextGetCurrent (void);
PD_LOCAL t_chain    *instance_chainGetTemporary (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   chain_append                    (t_chain *x, t_perform f, int n, ...);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_initializer  *initializer_new        (t_initializerfn fn, void *lhs, void *rhs);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif  // M_EXTERNALS_H_
