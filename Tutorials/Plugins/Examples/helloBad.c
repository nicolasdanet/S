
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Manage failure at creation time. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _hello {
    t_object x_obj;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x  = (t_hello *)spaghettis_objectNew (hello_class);

    t_error err = PD_ERROR_NONE;
    
    err |= PD_ERROR;    /* Something wrong happens. */
    
    if (err) { 
        spaghettis_objectFree ((t_object *)x);      /* It is safe to call the free method. */
        x = NULL;
    }
    
    return x;
}

static void hello_free (t_hello *x)
{
    spaghettis_post (NULL, "Clean your room, now!");
    
    /* Clean the already allocated things here. */
    /* Take care about fully vs partially constructed instance. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloBad_setup (t_symbol *s)
{
    t_symbol *name = spaghettis_symbol ("helloBad");
    
    hello_class = spaghettis_classNew (name,
        (t_newmethod)hello_new,
        (t_method)hello_free,
        sizeof (t_hello),
        CLASS_BOX | CLASS_NOINLET);
}

PD_STUB void helloBad_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
