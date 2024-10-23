
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef W_INPUTS_H_
#define W_INPUTS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_dspSwitch                   (void);
void inputs_searchPathsRescan           (int logged);
void inputs_searchPathsSet              (const juce::StringArray&);
void inputs_snapSet                     (bool);
void inputs_snapSetGrid                 (int);
void inputs_devicesRescan               (int logged);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_patchUndo                   (core::UniqueId);
void inputs_patchRedo                   (core::UniqueId);
void inputs_patchCut                    (core::UniqueId);
void inputs_patchCopy                   (core::UniqueId);
void inputs_patchPaste                  (core::UniqueId, Point::Real pt);
void inputs_patchDuplicate              (core::UniqueId);
void inputs_patchRemove                 (core::UniqueId);
void inputs_patchEncapsulate            (core::UniqueId);
void inputs_patchDeencapsulate          (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_patchNew                    (const juce::File&);
void inputs_patchOpen                   (const juce::File&);
void inputs_patchClose                  (core::UniqueId);
void inputs_patchSave                   (core::UniqueId);
void inputs_patchSetDirty               (core::UniqueId);
void inputs_patchSetEditView            (core::UniqueId, juce::Rectangle<int>);
void inputs_patchSetEditViewProperties  (core::UniqueId, core::Point::Real, int, bool, int);
void inputs_patchCreateObject           (core::UniqueId, Point::Real pt, juce::String);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_objectSelect                (core::UniqueId);
void inputs_objectDeselect              (core::UniqueId);
void inputs_objectMoveBack              (core::UniqueId);
void inputs_objectMoveFront             (core::UniqueId);
void inputs_objectSnap                  (core::UniqueId);
void inputs_objectPosition              (core::UniqueId, Point::Real pt);
void inputs_objectSendBang              (core::UniqueId);
void inputs_objectSendFloat             (core::UniqueId, double);
void inputs_objectOpenHelp              (core::UniqueId);
void inputs_objectSetParameters         (core::UniqueId, const data::Group&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_lineConnect                 (core::UniqueId u, int m, core::UniqueId v, int n);
void inputs_lineDisconnect              (core::UniqueId u, int m, core::UniqueId v, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_INPUTS_H_
