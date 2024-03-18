
/* 
    A C-program for MT19937--64 (2004/9/29 version).
    Coded by Takuji Nishimura and Makoto Matsumoto.
    
    Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura, 
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

#if PD_64BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define PIZ_MT64_NN     312L
#define PIZ_MT64_MM     156L
#define PIZ_MT64_A      0xb5026f5aa96619e9ULL
#define PIZ_MT64_UPPER  0xffffffff80000000ULL
#define PIZ_MT64_LOWER  0x7fffffffULL

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void init_genrand64 (MTState64 *x, uint64_t s)
{
    x->mt_[0] = s;
    
    for (x->mti_ = 1; x->mti_ < PIZ_MT64_NN; x->mti_++) {
    //
    x->mt_[x->mti_] = 6364136223846793005ULL * (x->mt_[x->mti_ - 1] ^ (x->mt_[x->mti_ - 1] >> 62)) + x->mti_;
    //
    }
}

#if 0

static void init_genrand64ByArray (MTState64 *x, long key_length, uint64_t *key)
{
    uint64_t i, j, k;
    
    init_genrand64 (x, 19650218ULL);
    
    i = 1; j = 0;
    k = (PIZ_MT64_NN > key_length ? PIZ_MT64_NN : key_length);
    
    for (; k; k--) {
    //
    x->mt_[i] = (x->mt_[i] ^ ((x->mt_[i - 1] ^ (x->mt_[i - 1] >> 62)) * 3935559000370003845ULL)) + key[j] + j;
    i++; j++;
    if (i >= PIZ_MT64_NN) { x->mt_[0] = x->mt_[PIZ_MT64_NN - 1]; i = 1; }
    if (j >= (uint64_t)key_length) { j = 0; }
    //
    }
    
    for (k = PIZ_MT64_NN - 1; k; k--) {
    //
    x->mt_[i] = (x->mt_[i] ^ ((x->mt_[i - 1] ^ (x->mt_[i - 1] >> 62)) * 2862933555777941757ULL)) - i;
    i++;
    if (i >= PIZ_MT64_NN) { x->mt_[0] = x->mt_[PIZ_MT64_NN - 1]; i = 1; }
    //
    }

    x->mt_[0] = 1ULL << 63;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MTState64 *genrand64_new (void)
{
    MTState64 *x = NULL;
    
    if ((x = (MTState64 *)PD_MEMORY_GET (sizeof (MTState64)))) {
        init_genrand64 (x, (uint64_t)time_makeRandomSeed());
    }
        
    return x;
}

#if 0

MTState64 *genrand64_newByArray (long argc, uint64_t *argv)
{
    MTState64 *x = NULL;
    
    if ((x = (MTState64 *)PD_MEMORY_GET (sizeof (MTState64)))) {
        init_genrand64ByArray (x, argc, argv);
    }
        
    return x;
}

#endif

void genrand64_free (MTState64 *x)
{
    if (x) {
        PD_MEMORY_FREE (x);
    }
}

double genrand64_real2 (MTState64 *x)
{
    return (genrand64_int64 (x) >> 11) * (1.0 / 9007199254740992.0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

uint64_t genrand64_int64 (MTState64 *x)
{
    uint64_t y;
    static const uint64_t mag01[2] = { 0ULL, PIZ_MT64_A };      /* Static. */

    if (x->mti_ >= PIZ_MT64_NN) {
    //
    int kk;
        
    for (kk = 0; kk < PIZ_MT64_NN - PIZ_MT64_MM; kk++) {
        y = (x->mt_[kk] & PIZ_MT64_UPPER) | (x->mt_[kk + 1] & PIZ_MT64_LOWER);
        x->mt_[kk] = x->mt_[kk + PIZ_MT64_MM] ^ (y >> 1) ^ mag01[(int)(y & 1ULL)];
    }
    
    for (; kk < PIZ_MT64_NN - 1; kk++) {
        y = (x->mt_[kk] & PIZ_MT64_UPPER) | (x->mt_[kk + 1] & PIZ_MT64_LOWER);
        x->mt_[kk] = x->mt_[kk + (PIZ_MT64_MM - PIZ_MT64_NN)] ^ (y >> 1) ^ mag01[(int)(y & 1ULL)];
    }
    
    y = (x->mt_[PIZ_MT64_NN - 1] & PIZ_MT64_UPPER) | (x->mt_[0] & PIZ_MT64_LOWER);
    x->mt_[PIZ_MT64_NN - 1] = x->mt_[PIZ_MT64_MM - 1] ^ (y >> 1) ^ mag01[(int)(y & 1ULL)];

    x->mti_ = 0;
    //
    }
  
    y = x->mt_[x->mti_++];

    y ^= (y >> 29) & 0x5555555555555555ULL;
    y ^= (y << 17) & 0x71d67fffeda60000ULL;
    y ^= (y << 37) & 0xfff7eee000000000ULL;
    y ^= (y >> 43);

    return y;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_64BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
