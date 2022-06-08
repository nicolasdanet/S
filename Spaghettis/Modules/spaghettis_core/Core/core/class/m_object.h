
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_OBJECT_H_
#define M_OBJECT_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    object_setSnappedX                  (t_object *x, int n);
PD_LOCAL int    object_setSnappedY                  (t_object *x, int n);

PD_LOCAL void   object_setBuffer                    (t_object *x, t_buffer *b);     /* Acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    object_isDummy                      (t_object *x);      /* A badly created box object. */
PD_LOCAL int    object_isCanvas                     (t_object *x);

PD_LOCAL int    object_isObject                     (t_object *x);      /* NOT a comment, message, or atom. */
PD_LOCAL int    object_isComment                    (t_object *x);
PD_LOCAL int    object_isMessage                    (t_object *x);
PD_FORCE int    object_isAtom                       (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    object_hasDsp                       (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_outconnect   *object_connect             (t_object *src, int m, t_object *dest, int n, t_glist *g);

PD_LOCAL t_error        object_disconnect           (t_object *src, int m, t_object *dest, int n, t_glist *g);

PD_FORCE void           object_help                 (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    object_getNumberOfInlets            (t_object *x);
PD_LOCAL int    object_getNumberOfOutlets           (t_object *x);
PD_LOCAL int    object_getNumberOfSignalInlets      (t_object *x);
PD_LOCAL int    object_getNumberOfSignalOutlets     (t_object *x);
PD_LOCAL int    object_getIndexAsSignalOfInlet      (t_object *x, int m);
PD_LOCAL int    object_getIndexAsSignalOfOutlet     (t_object *x, int m);
PD_FORCE int    object_isSignalInlet                (t_object *x, int m);
PD_LOCAL int    object_isSignalOutlet               (t_object *x, int m);

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL juce::String object_getTypeOfInlets        (t_object *x);
PD_LOCAL juce::String object_getTypeOfOutlets       (t_object *x);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   object_save                         (t_object *x, t_buffer *b, int flags);
PD_LOCAL void   object_saveIdentifiers              (t_object *x, t_buffer *b, int flags);
PD_LOCAL void   object_serializeLabel               (t_object *x, t_buffer *b);
PD_LOCAL void   object_distributeAtomsOnInlets      (t_object *x, int argc, t_atom *argv);
PD_LOCAL void   object_setSignalValues              (t_object *x, int argc, t_atom *argv);

PD_LOCAL t_buffer        *object_functionData       (t_object *z, int flags);

PD_LOCAL t_float64Atomic *object_getSignalAtIndex   (t_object *x, int m);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_id   object_getUnique                    (t_object *x);
PD_LOCAL void   object_setUnique                    (t_object *x, t_id u);
PD_LOCAL void   object_changeUnique                 (t_object *x, t_id u);
PD_LOCAL void   object_serializeUnique              (t_object *x, t_symbol *s, t_buffer *b);

PD_LOCAL t_id   object_getSource                    (t_object *x);
PD_LOCAL void   object_setSource                    (t_object *x, t_id u);
PD_LOCAL void   object_changeSource                 (t_object *x, t_id u);
PD_LOCAL void   object_serializeSource              (t_object *x, t_symbol *s, t_buffer *b);

PD_LOCAL t_id   object_getNative                    (t_object *x);
PD_LOCAL void   object_setNative                    (t_object *x, t_id u);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Required to fetch the unexpanded form at instantiation. */

PD_LOCAL t_symbol   *object_getUnexpandedNameAt     (t_object *x, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_float64Atomic *object_getFirstInletSignal (t_object *x)
{
    return &x->g_f;
}

static inline t_buffer *object_getBuffer (t_object *x)
{
    PD_ASSERT (x->g_buffer != NULL); return x->g_buffer;
}

static inline t_inlet *object_getInlets (t_object *x)
{
    return x->g_inlets;
}

static inline t_outlet *object_getOutlets (t_object *x)
{
    return x->g_outlets;
}

static inline int object_getX (t_object *x)
{
    return x->g_x;
}

static inline int object_getY (t_object *x)
{
    return x->g_y;
}

static inline t_point object_getPoint (t_object *x)
{
    return point_make (object_getX (x), object_getY (x));
}

static inline t_symbol *object_getLabel (t_object *x)
{
    return (x->g_label == NULL ? &s_ : x->g_label);
}

static inline int object_hasLabel (t_object *x)
{
    return (object_getLabel (x) != &s_);
}

static inline int object_isSelected (t_object *x)
{
    return x->g_selected;
}

static inline t_objecttype object_getType (t_object *x)
{
    return x->g_type;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void object_setX (t_object *x, int n)
{
    x->g_x = n;
}

static inline void object_setY (t_object *x, int n)
{
    x->g_y = n;
}

static inline void object_setLabel (t_object *x, t_symbol *s)
{
    x->g_label = s;
}

static inline void object_setSelected (t_object *x, int n)
{
    x->g_selected = (n != 0);
}

static inline void object_setType (t_object *x, t_objecttype n)
{
    x->g_type = n;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_OBJECT_H_
