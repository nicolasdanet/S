
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

int    object_setSnappedX                  (t_object *x, int n);
int    object_setSnappedY                  (t_object *x, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void   object_setBuffer                    (t_object *x, t_buffer *b);     /* Acquires ownership. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    object_isDummy                      (t_object *x);      /* A badly created box object. */
int    object_isCanvas                     (t_object *x);

int    object_isObject                     (t_object *x);      /* NOT a comment, message, or atom. */
int    object_isComment                    (t_object *x);
int    object_isMessage                    (t_object *x);
// int    object_isAtom                    (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    object_hasDsp                       (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_outconnect   *object_connect             (t_object *src, int m, t_object *dest, int n, t_glist *g);

t_error        object_disconnect           (t_object *src, int m, t_object *dest, int n, t_glist *g);

void           object_help                 (t_object *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    object_getNumberOfInlets            (t_object *x);
int    object_getNumberOfOutlets           (t_object *x);
int    object_getNumberOfSignalInlets      (t_object *x);
int    object_getNumberOfSignalOutlets     (t_object *x);
int    object_getIndexAsSignalOfInlet      (t_object *x, int m);
int    object_getIndexAsSignalOfOutlet     (t_object *x, int m);
//int  object_isSignalInlet                (t_object *x, int m);
int    object_isSignalOutlet               (t_object *x, int m);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

juce::String   object_getTypeOfInlets      (t_object *x);
juce::String   object_getTypeOfOutlets     (t_object *x);

juce::String   object_getBufferAsString    (t_object* x);
bool           object_setBufferWithString  (t_object* x, const juce::String&);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   object_save                         (t_object *x, t_buffer *b, int flags);
void   object_saveIdentifiers              (t_object *x, t_buffer *b, int flags);
void   object_distributeAtomsOnInlets      (t_object *x, int argc, t_atom *argv);
void   object_setSignalValues              (t_object *x, int argc, t_atom *argv);

t_buffer        *object_functionData       (t_object *z, int flags);

t_float64Atomic *object_getSignalAtIndex   (t_object *x, int m);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_id   object_getUnique                    (t_object *x);
t_id   object_getSource                    (t_object *x);
t_id   object_getNative                    (t_object *x);

void   object_setUnique                    (t_object *x, t_id u);
void   object_setSource                    (t_object *x, t_id u);
void   object_setNative                    (t_object *x, t_id u);

void   object_changeUnique                 (t_object *x, t_id u);
void   object_changeSource                 (t_object *x, t_id u);

void   object_serializeUnique              (t_object *x, t_symbol *s, t_buffer *b);
void   object_serializeSource              (t_object *x, t_symbol *s, t_buffer *b);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Required to fetch the unexpanded form at instantiation. */

t_symbol   *object_getUnexpandedNameAt     (t_object *x, int i);

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

static inline t_glist *object_getOwner (t_object *x)
{
    return x->g_owner;
}

static inline int object_getX (t_object *x)
{
    return x->g_x;
}

static inline int object_getY (t_object *x)
{
    return x->g_y;
}

static inline t_point object_getPosition (t_object *x)
{
    return point_make (object_getX (x), object_getY (x));
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

static inline void object_setOwner (t_object *x, t_glist *g)
{
    x->g_owner = g;
}

static inline void object_setX (t_object *x, int n)
{
    x->g_x = n;
}

static inline void object_setY (t_object *x, int n)
{
    x->g_y = n;
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
