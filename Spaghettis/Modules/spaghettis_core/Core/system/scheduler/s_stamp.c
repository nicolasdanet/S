
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int stamp_isNTP (t_symbol *s)
{
    return (s == sym_NTP);
}

t_error stamp_serialize (int argc, t_atom *argv, t_stamp *stamp)
{
    if (argc >= STAMP_SIZE) {
    //
    uint32_t hi = (uint32_t)((*stamp) >> 32);
    uint32_t lo = (uint32_t)((*stamp) & 0xffffffffULL);
    
    SET_SYMBOL (argv + 0, sym_NTP);
    SET_FLOAT (argv + 1,  hi);
    SET_FLOAT (argv + 2,  lo);
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

t_error stamp_deserialize (int argc, t_atom *argv, t_stamp *stamp)
{
    if (argc >= STAMP_SIZE && (GET_SYMBOL (argv + 0) == sym_NTP)) {
    //
    uint64_t hi = (uint64_t)GET_FLOAT (argv + 1);
    uint64_t lo = (uint64_t)GET_FLOAT (argv + 2);
    
    if (!(hi >> 32) && !(lo >> 32)) { (*stamp) = ((hi << 32) | lo); }
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

void stamp_setRandom (t_stamp *stamp)
{
    static int once = 0; static t_rand48 seed = 0;
    
    if (!once) { once = 1; seed = PD_RAND48_SEED; }
    
    uint64_t hi = PD_RAND48_UINT32 (seed);
    uint64_t lo = PD_RAND48_UINT32 (seed);
    
    (*stamp) = ((hi << 32) | lo);
}

int stamp_areEquals (t_stamp *stamp1, t_stamp *stamp2)
{
    return ((*stamp1) == (*stamp2));
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
