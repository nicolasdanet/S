
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

t_error    unique_objectSelect         (t_id u);
t_error    unique_objectDeselect       (t_id u);
t_error    unique_objectSnap           (t_id u);
t_error    unique_objectMoveBack       (t_id u);
t_error    unique_objectMoveFront      (t_id u);
t_error    unique_objectRemove         (t_id u);
t_error    unique_objectPosition       (t_id u, int x, int y);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error    unique_objectDisplace       (t_id u, int deltaX, int deltaY);
t_error    unique_objectMoveAtFirst    (t_id u);
t_error    unique_objectMoveAtLast     (t_id u);
t_error    unique_objectMoveAt         (t_id u, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    unique_objectBang           (t_id u);
t_error    unique_objectFloat          (t_id u, t_float);
t_error    unique_objectMessage        (t_id u, t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

t_error    unique_objectParameter      (t_id u, const core::Group&);
t_error    unique_objectHelp           (t_id u);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error    unique_objectGetIndexOf     (t_id u, int *n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    unique_objectLineConnect    (t_id u, int m, t_id v, int n);
t_error    unique_objectLineDisconnect (t_id u, int m, t_id v, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

t_error    unique_patchCreateObject    (t_id u, Point::Real, const juce::String&);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    unique_patchClose           (t_id u);
t_error    unique_patchSave            (t_id u);

t_error    unique_patchSetEditView     (t_id u, t_rectangle *r);
t_error    unique_patchSetRunView      (t_id u, t_rectangle *r);

t_error    unique_patchUndo            (t_id u);
t_error    unique_patchRedo            (t_id u);

t_error    unique_patchCut             (t_id u);
t_error    unique_patchCopy            (t_id u);
t_error    unique_patchPaste           (t_id u, t_point *m);
t_error    unique_patchDuplicate       (t_id u);
t_error    unique_patchRemove          (t_id u);

t_error    unique_patchEncapsulate     (t_id u);
t_error    unique_patchDeencapsulate   (t_id u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_UNIQUE_H_
