
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef X_TEXT_H_
#define X_TEXT_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _textbuffer {
    t_object        tb_obj;                 /* Must be the first. */
    t_buffer        *tb_buffer;
    t_glist         *tb_owner;
    } t_textbuffer;

typedef struct _textclient {
    t_object        tc_obj;                 /* Must be the first. */
    t_symbol        *tc_name;
    } t_textclient;

/* Also used by textfile object. */

typedef struct _qlist {
    t_textbuffer    ql_textbuffer;          /* Must be the first. */
    int             ql_indexOfMessage;
    int             ql_waitCount;
    int             ql_flagRewound;
    int             ql_flagReentrant;
    t_float         ql_unitValue;
    t_symbol        *ql_unitName;
    t_pd            *ql_target;
    t_glist         *ql_owner;
    t_outlet        *ql_outletLeft;
    t_outlet        *ql_outletRight;
    t_clock         *ql_clock;
    } t_qlist;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEXTCLIENT_NAME(x)                  &((x)->tc_name)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   textbuffer_init             (t_textbuffer *x);
PD_LOCAL void   textbuffer_free             (t_textbuffer *x);

PD_LOCAL void   textbuffer_read             (t_textbuffer *x, t_symbol *s);
PD_LOCAL void   textbuffer_write            (t_textbuffer *x, t_symbol *s);

PD_LOCAL t_glist    *textbuffer_getOwner    (t_textbuffer *x);
PD_LOCAL t_buffer   *textbuffer_getBuffer   (t_textbuffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    textclient_init         (t_textclient *x, int *argc, t_atom **argv);
PD_LOCAL void       textclient_update       (t_textclient *x);

PD_LOCAL t_glist    *textclient_fetchOwner  (t_textclient *x);
PD_LOCAL t_buffer   *textclient_fetchBuffer (t_textclient *x);

PD_LOCAL t_symbol   *textclient_getName     (t_textclient *x);
PD_LOCAL void       textclient_setName      (t_textclient *x, t_symbol *s);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       *textget_new            (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textset_new            (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textinsert_new         (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textdelete_new         (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textsize_new           (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textfromlist_new       (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *texttolist_new         (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textsearch_new         (t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       *textsequence_new       (t_symbol *s, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       qlist_rewind            (t_qlist *x);
PD_LOCAL void       qlist_clear             (t_qlist *x);
PD_LOCAL void       qlist_set               (t_qlist *x, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       qlist_add               (t_qlist *x, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       qlist_append            (t_qlist *x, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       qlist_restore           (t_qlist *x, t_symbol *s, int argc, t_atom *argv);
PD_LOCAL void       qlist_read              (t_qlist *x, t_symbol *name);
PD_LOCAL void       qlist_write             (t_qlist *x, t_symbol *name);

PD_LOCAL t_buffer   *qlist_functionData     (t_object *z, int flags);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // X_TEXT_H_
