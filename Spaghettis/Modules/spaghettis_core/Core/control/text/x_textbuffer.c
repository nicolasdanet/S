
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_text.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void textbuffer_init (t_textbuffer *x)
{
    x->tb_buffer = buffer_new();
    x->tb_owner  = instance_contextGetCurrent();
}

PD_LOCAL void textbuffer_free (t_textbuffer *x)
{
    buffer_free (x->tb_buffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_glist *textbuffer_getOwner (t_textbuffer *x)
{
    return x->tb_owner;
}

PD_LOCAL t_buffer *textbuffer_getBuffer (t_textbuffer *x)
{
    return x->tb_buffer;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void textbuffer_read (t_textbuffer *x, t_symbol *s)
{
    if (buffer_fileRead (x->tb_buffer, s, x->tb_owner, cast_object (x))) {
        error_failsToRead (cast_object (x), s);
    }
}

PD_LOCAL void textbuffer_write (t_textbuffer *x, t_symbol *s)
{
    if (buffer_fileWrite (x->tb_buffer, s, environment_getDirectory (glist_getEnvironment (x->tb_owner)))) {
        error_failsToWrite (cast_object (x), s);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
