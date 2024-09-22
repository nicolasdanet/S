
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_FFT_OOURA_H_
#define D_FFT_OOURA_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.kurims.kyoto-u.ac.jp/~ooura/fft.html > */

/* fft.zip (75KB) updated: 2006/12/28 */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that a maximum sized FFT is done at startup. */
/* Thus we assumed that Ooura's FFT is thread-safe next. */
/* Please let me know if it is not the case. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define FFT_MINIMUM         4
#define FFT_MAXIMUM         65536

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern int      *ooura_ip; 
extern double   *ooura_w;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void cdft       (int, int, double *, int *, double *);
void rdft       (int, int, double *, int *, double *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void ooura_complexFFT (t_FFTState *x,
    PD_RESTRICTED real,
    PD_RESTRICTED imaginary,
    int n,
    int type)
{
    double *t = x->s_cache;
    int i;
    
    for (i = 0; i < n; i++) {
    //
    t[(i * 2) + 0] = (double)real[i]; 
    t[(i * 2) + 1] = (double)imaginary[i];
    //
    }
    
    cdft (n * 2, type, t, ooura_ip, ooura_w);
    
    for (i = 0; i < n; i++) {
    //
    real[i]      = (t_sample)t[(i * 2) + 0]; 
    imaginary[i] = (t_sample)t[(i * 2) + 1];
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Imitate Ron Mayer API. */

/* Ron Mayer's real FFT have imaginary coefficients in reversed order. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static inline void fft_realFFT64 (t_FFTState *x, int n, t_float *s)
{
    double *t = x->s_cache;
    int i, half = (n >> 1);
    
    for (i = 0; i < n; i++) { t[i] = (double)s[i]; }
    
    rdft (n, 1, t, ooura_ip, ooura_w);
    
    s[0]     = (t_float)t[0];
    s[half]  = (t_float)t[1];
    
    for (i = 1; i < half; i++) {
    //
    s[i]     = (t_float)t[(i * 2) + 0];
    s[n - i] = (t_float)t[(i * 2) + 1];
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void fft_realFFT32 (t_FFTState *x, int n, PD_RESTRICTED s)
{
    double *t = x->s_cache;
    int i, half = (n >> 1);
    
    for (i = 0; i < n; i++) { t[i] = (double)s[i]; }
        
    rdft (n, 1, t, ooura_ip, ooura_w);
    
    s[0]     = (t_sample)t[0];
    s[half]  = (t_sample)t[1];
    
    for (i = 1; i < half; i++) {
    //
    s[i]     = (t_sample)t[(i * 2) + 0];
    s[n - i] = (t_sample)t[(i * 2) + 1];
    //
    }
}

static inline void fft_realInverseFFT32 (t_FFTState *x, int n, PD_RESTRICTED s)
{
    double *t = x->s_cache;
    int i, half = (n >> 1);
    
    t[0] = (double)s[0];
    t[1] = (double)s[half];
    
    for (i = 1; i < half; i++) {
    //
    t[(i * 2) + 0] = (double)s[i];
    t[(i * 2) + 1] = (double)s[n - i];
    //
    }
            
    rdft (n, -1, t, ooura_ip, ooura_w);
    
    for (i = 0; i < n; i++) { s[i] = (t_sample)(2.0 * t[i]); }
}

static inline void fft_complexFFT32 (t_FFTState *x, int n, PD_RESTRICTED real, PD_RESTRICTED imaginary)
{
    ooura_complexFFT (x, real, imaginary, n, -1);
}

static inline void fft_complexInverseFFT32 (t_FFTState *x, int n, PD_RESTRICTED real, PD_RESTRICTED imaginary)
{
    ooura_complexFFT (x, real, imaginary, n, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_FFT_OOURA_H_
