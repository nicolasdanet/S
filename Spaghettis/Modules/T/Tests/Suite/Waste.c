
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "Waste.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include <stdint.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_WASTE_SIZE  65521                       /* Prime number. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static uint32_t         ttt_wasteValues[TTT_WASTE_SIZE];

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ttt_wasteInitialize (void)
{
    int i; for (i = 0; i < TTT_WASTE_SIZE; i++) { ttt_wasteValues[i] = rand(); }
}

#if TTT_CONSTRUCTOR

void ttt_wasteCtor (void)  __attribute__ ((constructor));
void ttt_wasteCtor (void)  { ttt_wasteInitialize(); }

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ttt_wasteInit (TTTWaste *q, int n)
{
    int seed  = (n > 0) ? n : 0;
    q->step_  = (seed % (TTT_WASTE_SIZE - 1)) + 1;
    q->index_ = 0;
}

void ttt_wasteTime (TTTWaste *q)
{
    uint32_t i = 0;
    
    do {
    //
    i = ttt_wasteValues[q->index_];
    q->index_ += q->step_;
    if (q->index_ >= TTT_WASTE_SIZE) {
        q->index_ -= TTT_WASTE_SIZE;
    }
    //
    } while (i & 7);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
