
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_ITEMS_H_
#define M_ITEMS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define ITEMS_SIZE              4096
#define ITEMS_ELEMENTS          128
#define ITEMS_ELEMENTS_SIZE     32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Simple bounded structure to collect indexes on the stack. */

typedef struct _items { uint32_t i_items[ITEMS_ELEMENTS]; } t_items;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void   items_init              (t_items *items);
void   items_setAtIndex        (t_items *items, int i);
int    items_isSetAtIndex      (t_items *items, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_ITEMS_H_