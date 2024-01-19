
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void items_init (t_items *items)
{
    int j; for (j = 0; j < ITEMS_ELEMENTS; j++) { items->i_items[j] = 0U; }
}

void items_setAtIndex (t_items *items, int i)
{
    if (i < ITEMS_SIZE) {
    //
    int j = i / ITEMS_ELEMENTS_SIZE;
    int t = i % ITEMS_ELEMENTS_SIZE;
    
    items->i_items[j] |= (1U << t);
    //
    } else { PD_BUG; }
}

int items_isSetAtIndex (t_items *items, int i)
{
    if (i < ITEMS_SIZE) {
    //
    uint32_t k = 0U;

    int j = i / ITEMS_ELEMENTS_SIZE;
    int t = i % ITEMS_ELEMENTS_SIZE;
    
    k = items->i_items[j];

    k >>= t;
    k  &= 1U;
    
    return (k != 0U);
    //
    } else { PD_BUG; return 0; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
