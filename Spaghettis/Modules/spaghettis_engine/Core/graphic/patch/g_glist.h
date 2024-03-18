
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef G_GLIST_H_
#define G_GLIST_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _glist {  
    t_object        gl_obj;                     /* MUST be the first. */
    t_object        *gl_graphics;
    t_glist         *gl_next;
    t_abstractions  *gl_abstractions;           /* Root. */
    t_environment   *gl_environment;            /* Top.  */
    t_undomanager   *gl_undomanager;
    t_symbol        *gl_name;
    t_buffer        *gl_sorterObjects;
    t_buffer        *gl_sorterIndexes;
    t_rectangle     gl_editView;
    int             gl_offsetX;
    int             gl_offsetY;
    int             gl_zoom;
    int             gl_inspectorWidth;
    int             gl_hasInspector;
    int             gl_undoEnabled;
    int             gl_isDirty;                 /* Top.  */
    int             gl_isFrozen;                /* Top.  */
    int             gl_isLoading;
    int             gl_isPasting;
    int             gl_isCloseBanged;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_glist    *glist_newPatchPop  (t_symbol *name, t_rectangle *window);
t_glist    *glist_newPatch     (t_symbol *name, t_rectangle *window);

void       glist_free          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       glist_makeObject    (t_glist *g, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       glist_close         (t_glist *g);
void       glist_save          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_glist            *glist_getRoot              (t_glist *g);
t_glist            *glist_getTop               (t_glist *g);
t_glist            *glist_getParent            (t_glist *g);
t_glist            *glist_getNext              (t_glist *g);
t_symbol           *glist_getUnexpandedName    (t_glist *g);
t_environment      *glist_getEnvironment       (t_glist *g);
t_abstractions     *glist_getAbstractions      (t_glist *g);
t_rectangle        *glist_getEditView          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_symbol           *glist_getName              (t_glist *g);
t_undomanager      *glist_getUndoManager       (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int                glist_getOffsetX            (t_glist *g);
int                glist_getOffsetY            (t_glist *g);
int                glist_getZoom               (t_glist *g);
int                glist_getInspectorWidth     (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   glist_setName                           (t_glist *g, t_symbol *name);
void   glist_setDirty                          (t_glist *g, int n);
void   glist_setFrozen                         (t_glist *g, int n);
void   glist_setEditView                       (t_glist *g, t_rectangle *r, int notify);
void   glist_setOffset                         (t_glist *g, int x, int y, int notify);
void   glist_setZoom                           (t_glist *g, int z, int notify);
void   glist_setInspector                      (t_glist *g, int hasInspector, int w, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    glist_isRoot                            (t_glist *g);
int    glist_isTop                             (t_glist *g);
int    glist_isAbstraction                     (t_glist *g);
int    glist_isSubpatch                        (t_glist *g);
int    glist_isDirty                           (t_glist *g);
int    glist_isLoading                         (t_glist *g);
int    glist_isPasting                         (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int    glist_isAbstractionOrInside             (t_glist *g);
int    glist_isFrozen                          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    glist_hasParent                         (t_glist *g);
int    glist_hasInspector                      (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   glist_loadbang                          (t_glist *g);
void   glist_closebang                         (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    glist_fileExist (t_glist *g, const char *name, const char *extension, t_fileproperties *p);
int    glist_fileOpen  (t_glist *g, const char *name, const char *extension, t_fileproperties *p);
                                                            
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int    glist_undoIsOk                          (t_glist *g);
int    glist_undoHasSeparatorAtLast            (t_glist *g);
void   glist_undoAppendSeparator               (t_glist *g);
void   glist_undoAppend                        (t_glist *g, t_undoaction *a);

void   glist_undo                              (t_glist *g);
void   glist_redo                              (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   glist_objectAdd                         (t_glist *g, t_object *o);
void   glist_objectRemove                      (t_glist *g, t_object *o);
void   glist_objectRemoveAll                   (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void   glist_objectRemoveSelected              (t_glist *g);
int    glist_objectGetNumberOf                 (t_glist *g);
int    glist_objectGetNumberOfSelected         (t_glist *g);
void   glist_objectDeselectAll                 (t_glist *g);

void   glist_objectSelect                      (t_glist *g, t_object *o);
void   glist_objectDeselect                    (t_glist *g, t_object *o);
void   glist_objectMoveBack                    (t_glist *g, t_object *o);
void   glist_objectMoveFront                   (t_glist *g, t_object *o);
void   glist_objectPosition                    (t_glist *g, t_object *o, int x, int y, int notify);
void   glist_objectSnap                        (t_glist *g, t_object *o, int notify);

int    glist_objectIsSelected                  (t_glist *g, t_object *o);
int    glist_objectGetIndexOfAmongSelected     (t_glist *g, t_object *o);

void   glist_objectMoveAtFirst                 (t_glist *g, t_object *o);
void   glist_objectMoveAtLast                  (t_glist *g, t_object *o);
void   glist_objectMoveAt                      (t_glist *g, t_object *o, int n);

void   glist_objectDisplace                    (t_glist *g, t_object *o, int dX, int dY, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int    glist_objectGetIndexOf                  (t_glist *g, t_object *o);

t_object   *glist_objectGetAt                  (t_glist *g, int n);
t_object   *glist_objectGetLast                (t_glist *g);

void   glist_objectDeleteLinesByOutlet         (t_glist *g, t_object *o, t_outlet *outlet);
void   glist_objectDeleteLinesByInlet          (t_glist *g, t_object *o, t_inlet  *inlet);
void   glist_objectDeleteLines                 (t_glist *g, t_object *o);

t_error    glist_objectConnect                 (t_glist *g, t_object *o, int m, t_object *d, int n);

void   glist_updateLinesForObject              (t_glist *glist, t_object *o);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   glist_inletSort                         (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void   glist_outletSort                        (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_GLIST_H_
