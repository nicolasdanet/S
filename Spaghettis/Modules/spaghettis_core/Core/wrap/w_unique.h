
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef W_UNIQUE_H_
#define W_UNIQUE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error    unique_objectSelect         (t_id u);
PD_FORCE t_error    unique_objectDeselect       (t_id u);
PD_FORCE t_error    unique_objectSnap           (t_id u);
PD_FORCE t_error    unique_objectMoveBack       (t_id u);
PD_FORCE t_error    unique_objectMoveFront      (t_id u);
PD_FORCE t_error    unique_objectRemove         (t_id u);
PD_FORCE t_error    unique_objectPosition       (t_id u, int x, int y);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_FORCE t_error    unique_objectDisplace       (t_id u, int deltaX, int deltaY);
PD_FORCE t_error    unique_objectMoveAtFirst    (t_id u);
PD_FORCE t_error    unique_objectMoveAtLast     (t_id u);
PD_FORCE t_error    unique_objectMoveAt         (t_id u, int n);
// PD_FORCE t_error    unique_objectMessage     (t_id u, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_FORCE t_error    unique_objectParameter      (t_id u, const core::Group&);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_FORCE t_error    unique_objectGetIndexOf     (t_id u, int *n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error    unique_objectLineConnect    (t_id u, int m, t_id v, int n);
PD_FORCE t_error    unique_objectLineDisconnect (t_id u, int m, t_id v, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_FORCE t_error    unique_patchCreateObject    (t_id u, juce::Point<int>, const juce::String&);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE t_error    unique_patchClose           (t_id u);
PD_FORCE t_error    unique_patchSave            (t_id u);

PD_FORCE t_error    unique_patchSetEditView     (t_id u, t_rectangle *r);
PD_FORCE t_error    unique_patchSetRunView      (t_id u, t_rectangle *r);

PD_FORCE t_error    unique_patchUndo            (t_id u);
PD_FORCE t_error    unique_patchRedo            (t_id u);

PD_FORCE t_error    unique_patchCut             (t_id u);
PD_FORCE t_error    unique_patchCopy            (t_id u);
PD_FORCE t_error    unique_patchPaste           (t_id u, t_point *m);
PD_FORCE t_error    unique_patchDuplicate       (t_id u);
PD_FORCE t_error    unique_patchRemove          (t_id u);

PD_FORCE t_error    unique_patchEncapsulate     (t_id u);
PD_FORCE t_error    unique_patchDeencapsulate   (t_id u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_UNIQUE_H_
