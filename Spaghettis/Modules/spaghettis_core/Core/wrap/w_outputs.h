
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void outputs_objectAdded                   (t_object *x, t_glist *owner);
PD_LOCAL void outputs_objectAttributes              (t_object *x, t_glist *owner, const Tags& t = Tags());
PD_LOCAL void outputs_objectParameters              (t_object *x, t_glist *owner, const Tags& t = Tags());

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void outputs_objectRemoved                 (t_object *x, t_glist *owner);
PD_LOCAL void outputs_objectRenamed                 (t_object *x, t_glist *owner, t_id t);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void outputs_patchDirty                    (t_glist *g, int isDirty);
PD_LOCAL void outputs_patchOrder                    (t_glist *g, std::vector<UniqueId>&&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void outputs_lineAdded     (t_id u, t_object *src, int m, t_object *dest, int n, t_glist *g);
PD_LOCAL void outputs_lineChanged   (t_id u, t_object *src, int m, t_object *dest, int n, t_glist *g);
PD_LOCAL void outputs_lineRemoved   (t_id u, t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_OUTPUTS_H_
