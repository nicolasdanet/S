
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_BUFFER_H_
#define M_BUFFER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef int (*t_cmpfn)(const void*, const void*);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       atom_copyAtom                       (t_atom *src, t_atom *dest);
void       atom_copyAtoms                      (t_atom *src, int m, t_atom *dest, int n);
int        atom_copyAtomsExpanded              (t_atom *src, int m, t_atom *dest, int n, t_glist *g);

t_atomtype atom_getType                        (t_atom *a);

int        atom_typesAreEquals                 (t_atom *a, t_atom *b);
int        atom_areEquals                      (t_atom *a, t_atom *b);
t_error    atom_withStringUnzeroed             (t_atom *a, const char *s, int size);
t_error    atom_toString                       (t_atom *a, char *dest, int size);

void       atom_qsort                          (int argc, t_atom *argv, t_cmpfn f);

void       atom_shuffle                        (int argc, t_atom *argv);
void       atom_sort                           (int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       buffer_qsort                        (t_buffer *x, t_cmpfn f);
void       buffer_appendClock                  (t_buffer *x, t_clock *c);

void       *buffer_getVoidAt                   (t_buffer *x, int n);
t_object   *buffer_getObjectAt                 (t_buffer *x, int n);           /* Not checked. */
t_buffer   *buffer_getBufferAt                 (t_buffer *x, int n);           /* Not checked. */
t_clock    *buffer_getClockAt                  (t_buffer *x, int n);           /* Not checked. */

char       *buffer_toString                    (t_buffer *x);      /* Caller acquires ownership. */

void       buffer_toStringUnzeroed             (t_buffer *x, char **s, int *size);
void       buffer_withStringUnzeroed           (t_buffer *x, const char *s, int size);

void       buffer_reparseIfContainsWhitespace  (t_buffer *x);

int        buffer_messagesIsLastProperlyEnded  (t_buffer *x);
int        buffer_messagesGetNumberOf          (t_buffer *x);
t_error    buffer_messagesGetAt                (t_buffer *x, int n, int *start, int *end);
t_error    buffer_messagesGetAtWithTypeOfEnd   (t_buffer *x,
                                                            int n,
                                                            int *start,
                                                            int *end,
                                                            t_atomtype *t);

void       buffer_serialize                    (t_buffer *x, t_buffer *y);
void       buffer_deserialize                  (t_buffer *x, int argc, t_atom *argv);

t_error    buffer_fileRead                     (t_buffer *x,
                                                            t_symbol *name,
                                                            t_glist  *glist,
                                                            t_object *owner);
                                                            
t_error    buffer_fileWrite                    (t_buffer *x, t_symbol *name, t_symbol *directory);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_BUFFER_H_
