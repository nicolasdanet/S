
/* 
    A C-program for MT19937,
    coded by Takuji Nishimura and Makoto Matsumoto.

    Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
    All rights reserved.                          

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    
    1.  Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

    2.  Redistributions in binary form must reproduce the above 
        copyright notice, this list of conditions and the following 
        disclaimer in the documentation and/or other materials provided 
        with the distribution.
 
    3.  The names of its contributors may not be used to endorse or 
        promote products derived from this software without specific 
        prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT 
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Any feedback is very welcome.
    m-mat @ math.sci.hiroshima-u.ac.jp (remove space)

*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html > */
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_32BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PIZ_MT32_N      624L
#define PIZ_MT32_M      397L
#define PIZ_MT32_A      0x9908b0dfUL
#define PIZ_MT32_UPPER  0x80000000UL
#define PIZ_MT32_LOWER  0x7fffffffUL

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void init_genrand32 (MTState32 *x, uint32_t s)
{
    x->mt_[0] = s;
    
    for (x->mti_ = 1; x->mti_ < PIZ_MT32_N; x->mti_++) {
        x->mt_[x->mti_] = (1812433253UL * (x->mt_[x->mti_ - 1] ^ (x->mt_[x->mti_ - 1] >> 30)) + x->mti_);
    }
}

#if 0

static void init_genrand32ByArray (MTState32 *x, long key_length, uint32_t *init_key)
{
    int i, j, k;
    
    init_genrand32 (x, 19650218UL);
    
    i = 1; j = 0;
    k = (PIZ_MT32_N > key_length ? PIZ_MT32_N : key_length);
    
    for (; k; k--) {
        x->mt_[i] = (x->mt_[i] ^ ((x->mt_[i - 1] ^ (x->mt_[i - 1] >> 30)) * 1664525UL)) + init_key[j] + j;
        i++; j++;
        if (i >= PIZ_MT32_N) { x->mt_[0] = x->mt_[PIZ_MT32_N - 1]; i = 1; }
        if (j >= key_length) { j = 0; }
    }
    for (k = PIZ_MT32_N - 1; k; k--) {
        x->mt_[i] = (x->mt_[i] ^ ((x->mt_[i - 1] ^ (x->mt_[i - 1] >> 30)) * 1566083941UL)) - i;
        i++;
        if (i >= PIZ_MT32_N) { x->mt_[0] = x->mt_[PIZ_MT32_N - 1]; i = 1; }
    }

    x->mt_[0] = 0x80000000UL;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MTState32 *genrand32_new (void)
{
    MTState32 *x = NULL;
    
    if ((x = (MTState32 *)PD_MEMORY_GET (sizeof (MTState32)))) {
        init_genrand32 (x, (uint32_t)time_makeRandomSeed());
    }
        
    return x;
}

#if 0

MTState32 *genrand32_newByArray (long argc, uint32_t *argv)
{
    MTState32 *x = NULL;
    
    if ((x = (MTState32 *)PD_MEMORY_GET (sizeof (MTState32)))) {
        init_genrand32ByArray (x, argc, argv);
    }
        
    return x;
}

#endif

void genrand32_free (MTState32 *x)
{
    if (x) {
        PD_MEMORY_FREE (x);
    }
}

#if 0

double genrand32_real2 (MTState32 *x)
{
    return genrand32_int32 (x) * (1.0 / 4294967296.0); 
}

#endif

double genrand32_res53 (MTState32 *x)        /* Version due to Isaku Wada. */
{ 
    uint32_t a = genrand32_int32 (x) >> 5;
    uint32_t b = genrand32_int32 (x) >> 6;
    
    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0); 
} 

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

uint32_t genrand32_int32 (MTState32 *x)
{
    uint32_t y;
    static const uint32_t mag01[2] = { 0x0UL, PIZ_MT32_A };     /* Static. */

    if (x->mti_ >= PIZ_MT32_N) {
    //
    int kk;

    for (kk = 0; kk < PIZ_MT32_N - PIZ_MT32_M; kk++) {
        y = (x->mt_[kk] & PIZ_MT32_UPPER) | (x->mt_[kk + 1] & PIZ_MT32_LOWER);
        x->mt_[kk] = x->mt_[kk + PIZ_MT32_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    
    for (; kk < PIZ_MT32_N - 1; kk++) {
        y = (x->mt_[kk] & PIZ_MT32_UPPER) | (x->mt_[kk + 1] & PIZ_MT32_LOWER);
        x->mt_[kk] = x->mt_[kk + (PIZ_MT32_M - PIZ_MT32_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    
    y = (x->mt_[PIZ_MT32_N - 1] & PIZ_MT32_UPPER) | (x->mt_[0] & PIZ_MT32_LOWER);
    x->mt_[PIZ_MT32_N - 1] = x->mt_[PIZ_MT32_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    x->mti_ = 0;
    //
    }
  
    y = x->mt_[x->mti_++];

    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_32BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
