
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef W_OUTPUTS_H_
#define W_OUTPUTS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void outputs_quit                          (void);
PD_LOCAL void outputs_clearConsole                  (void);
PD_LOCAL void outputs_reportDsp                     (int);
PD_LOCAL void outputs_patchOpened                   (t_symbol *, t_symbol *);
PD_LOCAL void outputs_reportAvailableAudioDevices   (t_deviceslist *);
PD_LOCAL void outputs_reportCurrentAudioDevices     (t_deviceslist *);
PD_LOCAL void outputs_reportAvailableMidiDevices    (t_deviceslist *);
PD_LOCAL void outputs_reportCurrentMidiDevices      (t_deviceslist *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void outputs_objectAdded                   (t_object *x, t_glist *owner);
PD_LOCAL void outputs_objectChanged                 (t_object *x, t_glist *owner);
PD_LOCAL void outputs_objectRemoved                 (t_object *x, t_glist *owner);
PD_LOCAL void outputs_objectRenamed                 (t_object *x, t_glist *owner, t_id t);

PD_LOCAL void outputs_patchDirty                    (t_glist *g, int isDirty);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_OUTPUTS_H_
