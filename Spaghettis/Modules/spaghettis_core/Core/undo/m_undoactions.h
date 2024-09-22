
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_UNDOACTIONS_H_
#define M_UNDOACTIONS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction    *undoseparator_new          (void);     /* Actions used for labelling only. */
t_undoaction    *undoadd_new                (void);
t_undoaction    *undoremove_new             (void);

t_undoaction    *undoconnect_new            (t_object *src, int m, t_object *dest, int n);
t_undoaction    *undodisconnect_new         (t_object *src, int m, t_object *dest, int n);
t_undoaction    *undomotion_new             (t_object *o, int oldX, int oldY, int newX, int newY);

t_undoaction    *undocreate_new             (t_object *o, t_undosnippet *snippet);
t_undoaction    *undodelete_new             (t_object *o, t_undosnippet *snippet);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_undoaction    *undocut_new                (void);
t_undoaction    *undopaste_new              (void);
t_undoaction    *undoduplicate_new          (void);
t_undoaction    *undosnap_new               (void);
t_undoaction    *undoencapsulate_new        (void);
t_undoaction    *undodeencapsulate_new      (void);

t_undoaction    *undoresize_new             (t_object *o, int oldWidth, int oldHeight, int width, int height);
t_undoaction    *undoorientation_new        (t_object *o, int isVertical);

t_undoaction    *undofront_new              (t_object *o, t_undosnippet *snippet);
t_undoaction    *undoback_new               (t_object *o, t_undosnippet *snippet);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_UNDOACTIONS_H_
