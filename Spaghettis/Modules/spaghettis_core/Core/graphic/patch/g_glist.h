
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
    t_glist         *gl_parent;
    t_glist         *gl_next;
    t_abstractions  *gl_abstractions;
    t_environment   *gl_environment;
    t_undomanager   *gl_undomanager;
    t_symbol        *gl_name;
    t_buffer        *gl_sorterObjects;
    t_buffer        *gl_sorterIndexes;
    t_rectangle     gl_editWindow;
    int             gl_undoEnabled;
    int             gl_isDirty;
    int             gl_isFrozen;
    int             gl_isLoading;
    int             gl_isDeleting;
    int             gl_isCloseBanged;
    int             gl_isOpened;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_glist    *glist_newPatchPop  (t_symbol *name, t_rectangle *window, int isOpened);
PD_LOCAL t_glist    *glist_newPatch     (t_symbol *name, t_rectangle *window, int isOpened);

PD_LOCAL void       glist_free          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       glist_makeObject    (t_glist *g, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       glist_close         (t_glist *g);
PD_GUARD void       glist_save          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
PD_LOCAL t_glist            *glist_getRoot              (t_glist *g);
PD_LOCAL t_glist            *glist_getTop               (t_glist *g);
PD_LOCAL t_glist            *glist_getParent            (t_glist *g);
PD_LOCAL t_glist            *glist_getNext              (t_glist *g);
PD_GUARD t_symbol           *glist_getName              (t_glist *g);
PD_LOCAL t_symbol           *glist_getUnexpandedName    (t_glist *g);
PD_LOCAL t_environment      *glist_getEnvironment       (t_glist *g);
PD_LOCAL t_undomanager      *glist_getUndoManager       (t_glist *g);
PD_LOCAL t_abstractions     *glist_getAbstractions      (t_glist *g);
PD_LOCAL t_rectangle        *glist_getEditWindow        (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   glist_setName                           (t_glist *g, t_symbol *name);
PD_LOCAL void   glist_setDirty                          (t_glist *g, int n);
PD_LOCAL void   glist_setFrozen                         (t_glist *g, int n);
PD_LOCAL void   glist_setEditWindow                     (t_glist *g, t_rectangle *r);
PD_LOCAL void   glist_setOpened                         (t_glist *g, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    glist_isRoot                            (t_glist *g);
PD_LOCAL int    glist_isTop                             (t_glist *g);
PD_LOCAL int    glist_isAbstraction                     (t_glist *g);
PD_LOCAL int    glist_isAbstractionOrInside             (t_glist *g);
PD_LOCAL int    glist_isSubpatch                        (t_glist *g);
PD_GUARD int    glist_isDirty                           (t_glist *g);
PD_LOCAL int    glist_isFrozen                          (t_glist *g);
PD_LOCAL int    glist_isLoading                         (t_glist *g);
PD_LOCAL int    glist_isOpened                          (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    glist_hasParent                         (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   glist_loadbang                          (t_glist *g);
PD_LOCAL void   glist_closebang                         (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    glist_fileExist (t_glist *g, const char *name, const char *extension, t_fileproperties *p);
PD_LOCAL int    glist_fileOpen  (t_glist *g, const char *name, const char *extension, t_fileproperties *p);
                                                            
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    glist_undoIsOk                          (t_glist *g);
PD_LOCAL void   glist_undoAppendSeparator               (t_glist *g);
PD_LOCAL int    glist_undoHasSeparatorAtLast            (t_glist *g);
PD_LOCAL void   glist_undoAppend                        (t_glist *g, t_undoaction *a);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   glist_objectAdd                         (t_glist *g, t_object *y);
PD_LOCAL void   glist_objectRemove                      (t_glist *g, t_object *y);
PD_LOCAL void   glist_objectRemoveAll                   (t_glist *g);
PD_LOCAL void   glist_objectRemoveSelected              (t_glist *g);

PD_LOCAL int    glist_objectGetNumberOf                 (t_glist *g);
PD_LOCAL int    glist_objectGetNumberOfSelected         (t_glist *g);

PD_FORCE void   glist_objectSelect                      (t_glist *g, t_object *y);
PD_FORCE void   glist_objectSelectAll                   (t_glist *g);
PD_FORCE void   glist_objectDeselect                    (t_glist *g, t_object *y);
PD_FORCE void   glist_objectDeselectAll                 (t_glist *g);
PD_LOCAL int    glist_objectIsSelected                  (t_glist *g, t_object *y);

PD_LOCAL void   glist_objectMoveAtFirst                 (t_glist *g, t_object *y);
PD_LOCAL void   glist_objectMoveAtLast                  (t_glist *g, t_object *y);
PD_LOCAL void   glist_objectMoveAt                      (t_glist *g, t_object *y, int n);
PD_FORCE void   glist_objectMoveFront                   (t_glist *g, t_object *y);
PD_FORCE void   glist_objectMoveBackward                (t_glist *g, t_object *y);

PD_LOCAL int    glist_objectGetIndexOf                  (t_glist *g, t_object *y);
PD_LOCAL int    glist_objectGetIndexOfAmongSelected     (t_glist *g, t_object *y);

PD_LOCAL t_object   *glist_objectGetAt                  (t_glist *g, int n);
PD_LOCAL t_object   *glist_objectGetLast                (t_glist *g);

PD_LOCAL void   glist_objectDisplace                    (t_glist *g, t_object *y, int deltaX, int deltaY);
PD_FORCE void   glist_objectSnap                        (t_glist *g, t_object *y);

PD_LOCAL void   glist_objectDeleteLinesByOutlet         (t_glist *g, t_object *o, t_outlet *outlet);
PD_LOCAL void   glist_objectDeleteLinesByInlet          (t_glist *g, t_object *o, t_inlet  *inlet);
PD_LOCAL void   glist_objectDeleteLines                 (t_glist *g, t_object *o);

PD_LOCAL t_error    glist_objectConnect                 (t_glist *g, t_object *o, int m, t_object *d, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   glist_inletSort                         (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void   glist_outletSort                        (t_glist *g);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_GLIST_H_
