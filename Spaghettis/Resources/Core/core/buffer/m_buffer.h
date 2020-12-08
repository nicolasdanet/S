
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_BUFFER_H_
#define M_BUFFER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       atom_copyAtom                       (t_atom *src, t_atom *dest);
PD_LOCAL void       atom_copyAtoms                      (t_atom *src, int m, t_atom *dest, int n);
PD_LOCAL int        atom_copyAtomsExpanded              (t_atom *src, int m, t_atom *dest, int n, t_glist *g);

PD_LOCAL t_atomtype atom_getType                        (t_atom *a);

PD_LOCAL int        atom_typesAreEquals                 (t_atom *a, t_atom *b);
PD_LOCAL int        atom_areEquals                      (t_atom *a, t_atom *b);
PD_LOCAL t_error    atom_withStringUnzeroed             (t_atom *a, const char *s, int size);
PD_LOCAL t_error    atom_toString                       (t_atom *a, char *dest, int size);

PD_LOCAL void       atom_shuffle                        (int argc, t_atom *argv);
PD_LOCAL void       atom_sort                           (int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_object   *buffer_getObjectAt                 (t_buffer *x, int n);           /* Not checked. */
PD_LOCAL t_buffer   *buffer_getBufferAt                 (t_buffer *x, int n);           /* Not checked. */
PD_LOCAL t_clock    *buffer_getClockAt                  (t_buffer *x, int n);           /* Not checked. */

PD_LOCAL char       *buffer_toString                    (t_buffer *x);      /* Caller acquires ownership. */

PD_LOCAL void       buffer_toStringUnzeroed             (t_buffer *x, char **s, int *size);
PD_LOCAL void       buffer_withStringUnzeroed           (t_buffer *x, const char *s, int size);

PD_LOCAL void       buffer_reparseIfContainsWhitespace  (t_buffer *x);

PD_LOCAL int        buffer_messagesIsLastProperlyEnded  (t_buffer *x);
PD_LOCAL int        buffer_messagesGetNumberOf          (t_buffer *x);
PD_LOCAL t_error    buffer_messagesGetAt                (t_buffer *x, int n, int *start, int *end);
PD_LOCAL t_error    buffer_messagesGetAtWithTypeOfEnd   (t_buffer *x,
                                                            int n,
                                                            int *start,
                                                            int *end,
                                                            t_atomtype *t);

PD_LOCAL void       buffer_slotsClear                   (t_buffer *x);
PD_LOCAL int        buffer_slotsSize                    (t_buffer *x);
PD_LOCAL int        buffer_slotsIsEmpty                 (t_buffer *x);
PD_LOCAL void       buffer_slotsAppend                  (t_buffer *x, t_buffer *b);
PD_LOCAL t_error    buffer_slotsGet                     (t_buffer *x, int n, t_buffer *b);
PD_LOCAL t_error    buffer_slotsSet                     (t_buffer *x, int n, t_buffer *b);

PD_LOCAL void       buffer_serialize                    (t_buffer *x, t_buffer *y);
PD_LOCAL void       buffer_deserialize                  (t_buffer *x, int argc, t_atom *argv);

PD_LOCAL t_error    buffer_fileRead                     (t_buffer *x, t_symbol *name, t_glist *glist);
PD_LOCAL t_error    buffer_fileWrite                    (t_buffer *x, t_symbol *name, t_symbol *directory);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_BUFFER_H_
