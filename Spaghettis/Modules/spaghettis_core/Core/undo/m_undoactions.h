
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_UNDOACTIONS_H_
#define M_UNDOACTIONS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undoaction   *undoseparator_new          (void);     /* Actions used for labelling only. */
PD_LOCAL t_undoaction   *undoadd_new                (void);
PD_LOCAL t_undoaction   *undoremove_new             (void);

PD_LOCAL t_undoaction   *undoconnect_new            (t_object *src, int m, t_object *dest, int n);
PD_LOCAL t_undoaction   *undodisconnect_new         (t_object *src, int m, t_object *dest, int n);
PD_LOCAL t_undoaction   *undomotion_new             (t_object *o, int deltaX, int deltaY);

PD_LOCAL t_undoaction   *undocreate_new             (t_object *o, t_undosnippet *snippet);
PD_LOCAL t_undoaction   *undodelete_new             (t_object *o, t_undosnippet *snippet);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_undoaction   *undocut_new                (void);
PD_LOCAL t_undoaction   *undopaste_new              (void);
PD_LOCAL t_undoaction   *undoduplicate_new          (void);
PD_LOCAL t_undoaction   *undosnap_new               (void);
PD_LOCAL t_undoaction   *undoencapsulate_new        (void);
PD_LOCAL t_undoaction   *undodeencapsulate_new      (void);

// PD_LOCAL t_undoaction   *undoresize_new          (t_object *o, int m, int n);

PD_LOCAL t_undoaction   *undofront_new              (t_object *o, t_undosnippet *snippet);
PD_LOCAL t_undoaction   *undoback_new               (t_object *o, t_undosnippet *snippet);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_UNDOACTIONS_H_
