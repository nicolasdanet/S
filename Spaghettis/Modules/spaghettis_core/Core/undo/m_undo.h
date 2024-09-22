
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_UNDO_H_
#define M_UNDO_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef enum {
    UNDO_SEPARATOR      = 0,
    UNDO_ADD            = 1,
    UNDO_REMOVE         = 2,
    UNDO_CUT,
    UNDO_PASTE,
    UNDO_DUPLICATE,
    UNDO_SNAP,
    UNDO_ENCAPSULATE,
    UNDO_DEENCAPSULATE,
    UNDO_CONNECT,
    UNDO_DISCONNECT,
    UNDO_MOTION,
    UNDO_RESIZE,
    UNDO_ORIENTATION,
    UNDO_CREATE,
    UNDO_DELETE,
    UNDO_FRONT,
    UNDO_BACK
    } t_undotype;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _undoaction {
    t_pd                ua_pd;                      /* Must be the first. */
    t_id                ua_id;                      /* Identifier of involved object. */
    t_undotype          ua_type;
    int                 ua_safe;                    /* Don't need to suspend DSP. */
    int                 ua_inlet;                   /* Inlet created or deleted. */
    int                 ua_inletIndex;              /* Inlet index. */
    int                 ua_outlet;
    int                 ua_outletIndex;
    t_symbol            *ua_label;
    struct _undoaction  *ua_next;
    struct _undoaction  *ua_previous;
    } t_undoaction;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_id undoaction_getUnique (t_undoaction *a)
{
    return a->ua_id;
}

static inline t_undotype undoaction_getType (t_undoaction *a)
{
    return a->ua_type;
}

static inline int undoaction_suspend (t_undoaction *a)
{
    return (a->ua_safe == 0);
}

static inline t_symbol *undoaction_getLabel (t_undoaction *a)
{
    return a->ua_label;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undomanager {
    int                 um_count;
    t_glist             *um_owner;
    t_clock             *um_clock;
    t_undoaction        *um_head;
    t_undoaction        *um_tail;
    } t_undomanager;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undomanager  *undomanager_new            (t_glist *owner);

void   undomanager_free                    (t_undomanager *x);

int    undomanager_hasSeparatorAtLast      (t_undomanager *x);
void   undomanager_appendSeparator         (t_undomanager *x);
void   undomanager_append                  (t_undomanager *x, t_undoaction *a);

#if defined ( PD_BUILDING_APPLICATION )

juce::String undomanager_getUndoLabel      (t_undomanager *x);
juce::String undomanager_getRedoLabel      (t_undomanager *x);

#endif

void   undomanager_undo                    (t_undomanager *x);
void   undomanager_redo                    (t_undomanager *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undosnippet {
    t_id        us_object;
    t_id        us_owner;
    int         us_z;
    t_buffer    *us_buffer;
    } t_undosnippet;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undosnippet  *undosnippet_new            (t_object *o);

void           undosnippet_free            (t_undosnippet *x);
void           undosnippet_load            (t_undosnippet *x);
void           undosnippet_update          (t_undosnippet *x);

void           undosnippet_z               (t_undosnippet *x);
void           undosnippet_front           (t_undosnippet *x);
void           undosnippet_back            (t_undosnippet *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_UNDO_H_
