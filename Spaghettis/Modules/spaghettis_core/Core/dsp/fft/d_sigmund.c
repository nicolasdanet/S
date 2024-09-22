
/* Copyright (c) 2005 Miller Puckette. BSD licensed. No warranties. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../d_fft.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* This is a largely modified version of the file linked below. */

/* < https://github.com/pure-data/pure-data/blob/master/extra/sigmund%7E/sigmund%7E.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Is there any paper to explain in details that algorithm somewhere? */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for now that object is reset with encapsulation. */

// TODO: Fetch states with pending?

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *sigmund_tilde_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Parameters for Hann window. */

#define SIGMUND_W_ALPHA         0.5
#define SIGMUND_W_BETA          0.5
#define SIGMUND_N_BINS          4           /* Number of bins of negative frequency we'll need. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SIGMUND_THRESHOLD       0.6
#define SIGMUND_PITCHES         12
#define SIGMUND_SUBHARMONICS    16
#define SIGMUND_HISTOGRAM       100
#define SIGMUND_PEAKS           100
#define SIGMUND_WEIGHTS         48
#define SIGMUND_OUTLETS         5

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SIGMUND_FLT_MAX         1e20

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    OUT_PITCH           = 0,
    OUT_ENVELOPE        = 1,
    OUT_NOTE            = 2,
    OUT_PEAKS           = 3,
    OUT_TRACKS          = 4
    };

enum {
    MODE_STREAM         = 1,
    MODE_BLOCK          = 2,
    MODE_TABLE          = 3
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _peak {
    t_float             p_frequency;
    t_float             p_amplitude;
    t_float             p_amplitudeReal;
    t_float             p_amplitudeImaginary;
    t_float             p_salience;
    int                 p_tmp;
    } t_peak;

typedef struct _histogrampoint {
    t_float             h_frequency;
    t_float             h_power;
    } t_histogrampoint;

typedef struct _notefinder {
    t_float             n_age;
    t_float             n_frequencyHigh;
    t_float             n_frequencyLow;
    int                 n_peaked;
    t_histogrampoint    n_histogram[SIGMUND_HISTOGRAM];
    int                 n_histogramPhase;
    } t_notefinder;

typedef struct _variableout {
    int                 v_type;
    t_outlet            *v_outlet;
    } t_variableout;

typedef struct _sigmundhelper {
    int                 x_points;
    int                 x_found;
    t_float             x_frequency;
    t_float             x_power;
    t_float             x_frequencyNote;
    t_float             x_sampleRate;
    t_float             x_frequencyPerBin;
    } t_sigmundhelper;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _sigmund_tilde {
    t_object            x_obj;                      /* MUST be the first. */
    t_float             x_maximumFrequency;
    t_float             x_vibrato;
    t_float             x_stableTime;
    t_float             x_growth;
    t_float             x_minimumPower;
    t_float             x_parameter1;
    t_float             x_parameter2;
    t_float             x_sampleRate;
    int                 x_dismissed;
    int                 x_mode;
    int                 x_points;
    int                 x_numberOfPeaks;
    int                 x_index;
    int                 x_count;
    int                 x_hop;
    int                 x_hasPitch;
    int                 x_hasNote;
    int                 x_hasTrack;
    int                 x_outletsSize;
    int                 x_cache1Size;
    int                 x_cache2Size;
    int                 x_cache3Size;
    t_FFTState          x_state;
    t_variableout       x_outlets[SIGMUND_OUTLETS];
    t_fifo32            *x_fifo;
    t_notefinder        *x_finder;
    t_peak              *x_peaks;
    t_peak              *x_tracks;
    t_float             *x_cache1;
    t_float             *x_cache2;
    t_float             *x_cache3;
    t_float             *x_buffer;
    t_clock             *x_clock;
    } t_sigmund_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float *sigmund_tilde_reserve1 (t_sigmund_tilde *, int);
static t_float *sigmund_tilde_reserve2 (t_sigmund_tilde *, int);
static t_float *sigmund_tilde_reserve3 (t_sigmund_tilde *, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float sigmund_tilde_sinc (t_float theta, t_float sinTheta)
{
    return (PD_ABS (theta) < 0.003) ? 1.0 : (sinTheta / theta);
}

static t_float sigmund_tilde_windowHannMagnitude (t_float piDetune, t_float sinPiDetune)
{
    t_float t0 = sigmund_tilde_sinc (piDetune,         sinPiDetune);
    t_float t1 = sigmund_tilde_sinc (piDetune + PD_PI, sinPiDetune);
    t_float t2 = sigmund_tilde_sinc (piDetune - PD_PI, sinPiDetune);
    
    return (SIGMUND_W_ALPHA * t0) - (0.5 * SIGMUND_W_BETA * (t1 + t2));
}

static t_float sigmund_tilde_windowMagnitude (t_float piDetune, t_float cosPiDetune)
{
    t_float t0 = sigmund_tilde_sinc (piDetune + (PD_PI / 2),  cosPiDetune);
    t_float t1 = sigmund_tilde_sinc (piDetune - (PD_PI / 2), -cosPiDetune);
    
    return t0 + t1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int sigmund_tilde_proceedRawCompareFrequency (const void *p1, const void *p2)
{
    t_float f1 = (*(t_peak **)p1)->p_frequency;
    t_float f2 = (*(t_peak **)p2)->p_frequency;
    
    if (f1 > f2) { return 1; } else if (f1 < f2) { return -1; }
    
    return 0;
}

static void sigmund_tilde_proceedRawTweak (t_sigmund_tilde *x,
    int n,
    t_float *real,
    t_float *imag,
    int     count,
    t_float frequencyPerBin)
{
    int i, j, k;
    
    t_peak *sorted[SIGMUND_PEAKS + 1] = { 0 };
    t_peak negativePeak;
    
    PD_ASSERT (count < SIGMUND_PEAKS + 1);
    
    for (i = 0; i < count; i++) { sorted[i + 1] = &x->x_peaks[i]; }
    
    qsort (sorted + 1, count, sizeof (t_peak *), sigmund_tilde_proceedRawCompareFrequency);
    
    negativePeak.p_frequency          = -sorted[1]->p_frequency;
    negativePeak.p_amplitudeReal      =  sorted[1]->p_amplitudeReal;
    negativePeak.p_amplitudeImaginary = -sorted[1]->p_amplitudeImaginary;
    
    sorted[0] = &negativePeak;
    
    for (i = 1; i <= count; i++) {
    //
    int bin = ((sorted[i]->p_frequency / frequencyPerBin) + 0.5);

    if (bin >= 0 && bin < n - 2) {
    //
    t_float ampReal[3] = { 0 };
    t_float ampImag[3] = { 0 };
    
    ampReal[0] = real[bin - 2]; ampImag[0] = imag[bin - 2];
    ampReal[1] = real[bin + 0]; ampImag[1] = imag[bin + 0];
    ampReal[2] = real[bin + 2]; ampImag[2] = imag[bin + 2];
        
    for (j = 0; j < (i == count ? 1 : 2); j++) {
    //
    t_peak *neighbor = sorted[(i - 1) + (2 * j)];
    
    for (k = 0; k < 3; k++) {
    
        const t_float neighborReal = n * neighbor->p_amplitudeReal;
        const t_float neighborImag = n * neighbor->p_amplitudeImaginary;
        const t_float phasePerBin  = (n - 0.5) / n;
        const t_float delta = PD_HALF_PI * ((bin + (2 * k) - 2) - (neighbor->p_frequency / frequencyPerBin));
        const t_float sx = sigmund_tilde_sinc (delta, sin (delta));
        const t_float phaseReal = cos (delta * phasePerBin);
        const t_float phaseImag = sin (delta * phasePerBin);
        
        ampReal[k] -= sx * (phaseReal * neighborReal - phaseImag * neighborImag);
        ampImag[k] -= sx * (phaseImag * neighborReal + phaseReal * neighborImag);
    }
    //
    }

    const t_float wReal  = SIGMUND_W_ALPHA * ampReal[1] - (0.5 * SIGMUND_W_BETA) * (ampReal[0] + ampReal[2]);
    const t_float wImag  = SIGMUND_W_ALPHA * ampImag[1] - (0.5 * SIGMUND_W_BETA) * (ampImag[0] + ampImag[2]);
    const t_float wPower = wReal * wReal + wImag * wImag;
    const t_float f0     = SIGMUND_W_BETA * (ampReal[0] - ampReal[2]);
    const t_float f1     = SIGMUND_W_BETA * (ampReal[0] + ampReal[2]);
    const t_float g0     = SIGMUND_W_BETA * (ampImag[0] - ampImag[2]);
    const t_float g1     = SIGMUND_W_BETA * (ampImag[0] + ampImag[2]);
    const t_float f2     = f0 * (2.0 * SIGMUND_W_ALPHA * ampReal[1] - f1);
    const t_float g2     = g0 * (2.0 * SIGMUND_W_ALPHA * ampImag[1] - g1);
    const t_float t0     = (f2 + g2) / (4.0 * wPower);
    const t_float detune = PD_CLAMP (t0, -0.5, 0.5);
    
    const t_float piDetune    = PD_PI * detune;
    const t_float sinPiDetune = sin (piDetune);
    const t_float cosPiDetune = cos (piDetune);
    const t_float correction  = (1.0 / n) * (1.0 / sigmund_tilde_windowHannMagnitude (piDetune, sinPiDetune));
    const t_float ampOut      = correction * sqrt (wPower);
    const t_float ampOutReal  = correction * (wReal * cosPiDetune - wImag * sinPiDetune);
    const t_float ampOutImag  = correction * (wReal * sinPiDetune + wImag * cosPiDetune);

    sorted[i]->p_frequency          = (bin + 2 * detune) * frequencyPerBin;
    sorted[i]->p_amplitude          = ampOut;
    sorted[i]->p_amplitudeReal      = ampOutReal;
    sorted[i]->p_amplitudeImaginary = ampOutImag;
    //
    }
    //
    }
}

static void sigmund_tilde_proceedRawRemask (t_float *mask, int numberOfBins, int j, t_float maximumPower)
{
    int i;
    int start = PD_MAX (2, j - 50);
    int end   = PD_MIN (j + 50, numberOfBins);
    
    for (i = start; i < end; i++) {
    //
    int d = i - j; t_float t = 0.0;

    if (d < 2 && d > -2) { t = 2.0 * maximumPower; }
    else {
        t = maximumPower / (1.0 + (t_float)d * (t_float)d * (t_float)d * (t_float)d);
    }
    
    if (t > mask[i]) { mask[i] = t; }
    //
    }
}

static void sigmund_tilde_proceedRaw (t_sigmund_tilde *x, t_float *array, t_sigmundhelper *h)
{
    const int n        = h->x_points;
    int numberOfBins   = PD_MIN (n - SIGMUND_N_BINS, (int)(x->x_maximumFrequency / h->x_frequencyPerBin));
    t_float totalPower = 0.0;
    
    int i, count = 0;
    
    /* Allocate all memory required in one chunk. */
    
    t_float *buffer = sigmund_tilde_reserve2 (x, ((n * 6) + (SIGMUND_N_BINS + SIGMUND_N_BINS)));
    
    t_float *t     = buffer;                                                // 2 * n
    t_float *mask  = buffer + n * 2;                                        // n
    t_float *power = buffer + n * 3;                                        // n
    t_float *real  = buffer + n * 4 + SIGMUND_N_BINS;                       // n + SIGMUND_N_BINS
    t_float *imag  = buffer + n * 5 + SIGMUND_N_BINS + SIGMUND_N_BINS;      // n + SIGMUND_N_BINS
    
    for (i = 0; i < n; i++)     { t[i]    = array[i];     }
    for (i = n; i < 2 * n; i++) { t[i]    = 0.0;          }
    for (i = 0; i < n; i++)     { mask[i] = 0.0;          }
    
    /* FFT. */
    
    PD_ASSERT (n * 2 <= FFT_MAXIMUM);
    
    fft_realFFT64 (&x->x_state, n * 2, t);
    
    for (i = 0; i < n; i++)     { real[i] = t[i];         }
    for (i = 1; i < n - 1; i++) { imag[i] = t[n * 2 - i]; }                 /* Ron Mayer's FFT API. */
    
    imag[0] = imag[n - 1] = 0.0;
    
    real[-1] = real[1]; imag[-1] = -imag[1];
    real[-2] = real[2]; imag[-2] = -imag[2];
    real[-3] = real[3]; imag[-3] = -imag[3];
    real[-4] = real[4]; imag[-4] = -imag[4];

    /* Power. */
    
    for (i = 0; i < numberOfBins; i++) {
    //
    t_float wReal = real[i + 1] - real[i - 1];
    t_float wImag = imag[i + 1] - imag[i - 1];
    
    power[i] = wReal * wReal + wImag * wImag;
    
    if (i >= 2) { totalPower += power[i]; }
    //
    }
    
    power[numberOfBins] = power[numberOfBins + 1] = 0.0;
    
    h->x_power = 0.5 * totalPower * (1.0 / n) * (1.0 / n);

    /* Peaks. */
    
    for (count = 0; count < x->x_numberOfPeaks; count++) {
    //
    int j = -1; t_float maximumPower = 0.0;
    
    for (i = 2; i < numberOfBins; i++) {
        if (power[i] > maximumPower && power[i] > mask[i]) {
            if (power[i] > SIGMUND_THRESHOLD * (power[i - 2] + power[i + 2])) {
                maximumPower = power[i];
                j = i;
            }
        }
    }

    if (totalPower <= 0.0 || maximumPower < (1e-10 * totalPower) || j < 0) { break; }
    else {
    //
    sigmund_tilde_proceedRawRemask (mask, numberOfBins, j, maximumPower);
    
    const t_float wReal       = real[j + 1] - real[j - 1];
    const t_float wImag       = imag[j + 1] - imag[j - 1];
    const t_float wPower      = wReal * wReal + wImag * wImag;
    const t_float t0          = (real[j + 1] * real[j + 1] - real[j - 1] * real[j - 1]);
    const t_float t1          = (imag[j + 1] * imag[j + 1] - imag[j - 1] * imag[j - 1]);
    const t_float t2          = (t0 + t1) / (2 * wPower);
    const t_float detune      = PD_CLAMP (t2, -0.5, 0.5);
    const t_float piDetune    = PD_PI * detune;
    const t_float sinPiDetune = sin (piDetune);
    const t_float cosPiDetune = cos (piDetune);
    const t_float correction  = 1.0 / sigmund_tilde_windowMagnitude (piDetune, cosPiDetune);
    const t_float ampOut      = correction * sqrt (wPower);
    const t_float ampOutReal  = correction * (wReal * cosPiDetune - wImag * sinPiDetune);
    const t_float ampOutImag  = correction * (wReal * sinPiDetune + wImag * cosPiDetune);

    /* The frequency is the sum of the bin frequency and detuning. */

    x->x_peaks[count].p_frequency          = (j + 2 * detune) * h->x_frequencyPerBin;
    x->x_peaks[count].p_amplitude          = (1.0 / n) * ampOut;
    x->x_peaks[count].p_amplitudeReal      = (1.0 / n) * ampOutReal;
    x->x_peaks[count].p_amplitudeImaginary = (1.0 / n) * ampOutImag;
    //
    }
    //
    }
    
    if (count) {
        sigmund_tilde_proceedRawTweak (x, n, real, imag, count, h->x_frequencyPerBin);
        sigmund_tilde_proceedRawTweak (x, n, real, imag, count, h->x_frequencyPerBin);
    }
    
    for (i = 0; i < count; i++) {
        x->x_peaks[i].p_salience = math_powerToDecibel (x->x_peaks[i].p_amplitude);
    }
    
    h->x_found = count;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Finding fundamental pitch. */

static void sigmund_tilde_proceedPitch (t_sigmund_tilde *x, t_sigmundhelper *h)
{
    t_float frequency = 0.0; int count = h->x_found;
    
    if (count) {
    //
    t_peak *peaks[SIGMUND_PITCHES] = { NULL };
    
    int weightsSize   = SIGMUND_WEIGHTS * sys_ilog2 (h->x_points);
    t_float *weights  = sigmund_tilde_reserve3 (x, weightsSize);            /* Already zeroed. */
    t_float sumWeight = 0.0;
    
    int i, j, k, salient;
    
    for (i = 0; i < count; i++) { x->x_peaks[i].p_tmp = 0; }
    
    for (salient = 0; salient < SIGMUND_PITCHES; salient++) {
    
        t_peak *p = NULL; t_float t = -SIGMUND_FLT_MAX;
        
        for (j = 0; j < count; j++) {
            if (x->x_peaks[j].p_tmp == 0 && x->x_peaks[j].p_salience > t) {
                t = x->x_peaks[j].p_salience;
                p = &x->x_peaks[j];
            }
        }
        
        if (p) { peaks[salient] = p; p->p_tmp = 1; }
        else {
            break;
        }
    }

    for (i = 0; i < salient; i++) {
        
        t_float t = SIGMUND_WEIGHTS / PD_LOG_TWO;
        t_float weightIndex = t * log (peaks[i]->p_frequency / (2.0 * h->x_frequencyPerBin));
        t_float loudness = pow (peaks[i]->p_amplitude, x->x_parameter2);

        for (j = 0; j < SIGMUND_SUBHARMONICS; j++) {
        
            int start = (weightIndex - t * log (j + 1.0)) - 0.5;
            int end   = start + 2;
            
            if (end < 0) { break; }
            else if (end < weightsSize) {
                for (k = PD_MAX (0, start); k <= end; k++) {
                    weights[k] += loudness * x->x_parameter1 / (x->x_parameter1 + j);
                }
            }
        }
        
        sumWeight += loudness;
    }
    
    int bestBin        = -1;
    t_float bestWeight = -SIGMUND_FLT_MAX;

    for (i = 0; i < weightsSize; i++) {
        if (weights[i] > bestWeight) { bestWeight = weights[i]; bestBin = i; }
    }
    
    if (bestWeight < sumWeight * 0.4) { bestBin = -1; }
    
    if (bestBin >= 0) {
    //
    t_float sumFrequency = 0.0;
    t_float bestBinFloat = (t_float)bestBin;
    
    if (bestBin > 0 && bestBin < weightsSize - 1) {
        const t_float t0 = (weights[bestBin + 1] - weights[bestBin - 1]);
        const t_float t1 = (weights[bestBin + 1] + weights[bestBin - 1] + weights[bestBin]);
        bestBinFloat += t0 / t1;
    }
    
    frequency = 2.0 * h->x_frequencyPerBin * exp ((PD_LOG_TWO / SIGMUND_WEIGHTS) * bestBinFloat);
    
    sumWeight = 0.0;
    
    for (i = 0; i < salient; i++) {
    
        t_float harmonic = peaks[i]->p_frequency / frequency;
        t_float hInt     = ((int)(0.5 + harmonic));
        t_float hFrac    = (harmonic - hInt);

        if (hInt >= 1.0 && hInt <= 16.0) {
            if (PD_ABS (hFrac) < (0.015 * hInt)) {
                t_float weight = peaks[i]->p_amplitude * hInt;
                sumWeight    += weight;
                sumFrequency += weight * peaks[i]->p_frequency / hInt;
            }
        }
    }
    
    if (sumWeight > 0.0) { frequency = sumFrequency / sumWeight; }
    //
    }
    //
    }
    
    h->x_frequency = frequency;
}

/* Parse continuous pitch into note starts. */

static void sigmund_tilde_proceedNote (t_sigmund_tilde *z, t_sigmundhelper *h)
{
    /* Calculate frequency ratio between allowable vibrato extremes. */
    /* E.g. twice the vibrato deviation from center. */
    
    t_notefinder *x = z->x_finder;
    
    t_float frequency      = h->x_frequency;
    t_float power          = h->x_power;
    
    t_float powerThreshold = exp (PD_LOG_TEN * 0.1 * (z->x_minimumPower - 100.0));
    int temp               = 1 + z->x_stableTime * 0.001 * h->x_sampleRate / (t_float)z->x_hop;
    int stablePeriod       = PD_CLAMP (temp, 1, SIGMUND_HISTOGRAM - 1);
    t_float vibrato        = exp ((2.0 * PD_LOG_TWO / 12) * z->x_vibrato);
    
    int i, start;

    if (++x->n_histogramPhase == SIGMUND_HISTOGRAM) { x->n_histogramPhase = 0; }
    
    x->n_histogram[x->n_histogramPhase].h_frequency = frequency;
    x->n_histogram[x->n_histogramPhase].h_power     = power;
    x->n_age++;
    
    h->x_frequencyNote = 0.0;

    /* Look for shorter notes than stable period in length. */
    /* The amplitude must rise and then fall while the pitch holds steady. */
    
    if (x->n_frequencyHigh <= 0.0 && x->n_age > stablePeriod) {
    //
    t_float maximumPower            = 0.0;
    t_float frequencyAtMaximumPower = 0.0;
    t_float hiFrequency             = -SIGMUND_FLT_MAX;
    t_float loFrequency             =  SIGMUND_FLT_MAX;
    
    start = x->n_histogramPhase - stablePeriod + 1;
    
    if (start < 0) { start += SIGMUND_HISTOGRAM; }
    
    int k = start;
    
    for (i = 0; i < stablePeriod; i++) {
    //
    if (x->n_histogram[k].h_frequency <= 0)  { break; }
    
    if (x->n_histogram[k].h_power > maximumPower) {
        maximumPower            = x->n_histogram[k].h_power;
        frequencyAtMaximumPower = x->n_histogram[k].h_frequency;
    }
    
    if (x->n_histogram[k].h_frequency > hiFrequency) { hiFrequency = x->n_histogram[k].h_frequency; }
    if (x->n_histogram[k].h_frequency < loFrequency) { loFrequency = x->n_histogram[k].h_frequency; }
    
    if (hiFrequency > loFrequency * vibrato) { break; }
    
    if (maximumPower > power * z->x_growth) {
    if (maximumPower > x->n_histogram[start].h_power * z->x_growth) {
    if (hiFrequency  < vibrato * loFrequency) {
    if (frequencyAtMaximumPower > 0) {
    if (maximumPower > powerThreshold) {
    
        x->n_age           = 0.0;
        x->n_frequencyHigh = frequencyAtMaximumPower;
        x->n_frequencyLow  = frequencyAtMaximumPower;
        x->n_peaked        = 0;
        
        h->x_frequencyNote = frequencyAtMaximumPower;
        
        return;
    }
    }
    }
    }
    }
    
    if (++k >= SIGMUND_HISTOGRAM) { k = 0; }
    //
    }
    //
    }
    
    if (x->n_frequencyHigh > 0.0) {
    //
    /* Test if we're within vibrato range, and if so update range. */
    
    if (frequency * vibrato >= x->n_frequencyHigh && x->n_frequencyLow * vibrato >= frequency) {
        if (frequency > x->n_frequencyHigh) { x->n_frequencyHigh = frequency; }
        if (frequency < x->n_frequencyLow)  { x->n_frequencyLow  = frequency; }
        
    } else if (x->n_frequencyHigh > 0.0 && x->n_age > stablePeriod) {
    
        /* Clear the note if we've been out of range at least 1/2 the last analyses. */
        
        int n = 0;
        int k = x->n_histogramPhase;
        
        for (i = 0; i < stablePeriod + 1; i++) {
        
            if (--k < 0) { k = SIGMUND_HISTOGRAM - 1; }
            
            if (x->n_histogram[k].h_frequency * vibrato <= x->n_frequencyHigh) {
            if (x->n_histogram[k].h_frequency > x->n_frequencyLow * vibrato)   {
                n++;
            }
            }
        }
        
        if (n * 2 >= stablePeriod + 1) {
            x->n_frequencyHigh = 0.0;
            x->n_frequencyLow  = 0.0;
            x->n_age = 0;
        }
    }
    //
    }

    start = x->n_histogramPhase - stablePeriod;
    
    if (start < 0) { start += SIGMUND_HISTOGRAM; }

    /* Look for envelope attacks. */

    if (x->n_frequencyHigh > 0.0 && x->n_peaked) {
    
        t_float thresold = x->n_histogram[start].h_power * exp ((PD_LOG_TEN * 0.1) * z->x_growth);
        
        if (frequency > 0.0 && power > PD_MAX (thresold, powerThreshold)) {
            x->n_age           = 0.0;
            x->n_frequencyHigh = 0.0;
            x->n_frequencyLow  = 0.0;
            x->n_peaked        = 0;
        }
        
    } else if (!x->n_peaked) {
    
        if (x->n_histogram[start].h_power > PD_MAX (power, powerThreshold)) {
            x->n_peaked = 1;
        }
    }

    /* Test for a new note using a stability criterion. */

    if (frequency > 0.0) {
    //
    if (x->n_frequencyHigh <= 0 || frequency > x->n_frequencyHigh || frequency < x->n_frequencyLow) {
    //
    t_float hiFrequency  = frequency;
    t_float loFrequency  = frequency;
    t_float maximumPower = x->n_histogram[x->n_histogramPhase].h_power;

    int k = x->n_histogramPhase;

    for (i = 0; i < stablePeriod - 1; i++) {
    
        if (--k < 0) { k = SIGMUND_HISTOGRAM - 1; }
        
        if (x->n_histogram[k].h_frequency > hiFrequency) { hiFrequency  = x->n_histogram[k].h_frequency; }
        if (x->n_histogram[k].h_frequency < loFrequency) { loFrequency  = x->n_histogram[k].h_frequency; }
        if (x->n_histogram[k].h_power > maximumPower)    { maximumPower = x->n_histogram[k].h_power; }
    }

    if (loFrequency > 0.0 && hiFrequency <= vibrato * loFrequency && maximumPower > powerThreshold) {
        
        /* Report new note. */
        
        t_float sumFrequency = 0.0;
        t_float sumPower     = 0.0;
        
        k = x->n_histogramPhase;
        
        for (i = 0; i < stablePeriod; i++) {
        
            sumPower     += x->n_histogram[k].h_power;
            sumFrequency += x->n_histogram[k].h_power * x->n_histogram[k].h_frequency;
        
            if (--k < 0) { k = SIGMUND_HISTOGRAM - 1; }
        }
        
        h->x_frequencyNote = (sumPower > 0.0 ? sumFrequency / sumPower : 0.0);
        
        x->n_age           = 0.0;
        x->n_frequencyHigh = h->x_frequencyNote;
        x->n_frequencyLow  = h->x_frequencyNote;
        x->n_peaked        = 0;
        
        return;
    }
    //
    }
    //
    }
    
    PD_ASSERT (h->x_frequencyNote == 0.0);
    
    return;
}

/* Gather peak lists into sinusoidal tracks. */

static void sigmund_tilde_proceedTrack (t_sigmund_tilde *x, t_sigmundhelper *h)
{
    int i, j, count = h->x_found;
    
    t_float maximumError = 4.0 * h->x_frequencyPerBin;
    
    for (i = 0; i < x->x_numberOfPeaks; i++) { x->x_tracks[i].p_tmp = -1; }
    
    /* Match each peak with the closest previous peak (but no two to the same one). */
    
    for (i = 0; i < count; i++) {
    //
    t_float bestError = SIGMUND_FLT_MAX;
    
    int k = -1;
    
    x->x_peaks[i].p_tmp = -1;
    
    for (j = 0; j < x->x_numberOfPeaks; j++) {
        if (x->x_tracks[j].p_amplitude != 0.0) {
            t_float f = PD_ABS (x->x_peaks[i].p_frequency - x->x_tracks[j].p_frequency);
            if (f < bestError) {
                bestError = f; k = j;
            }
        }
    }
    
    if (k != -1 && bestError < maximumError && x->x_tracks[k].p_tmp < 0) {
        x->x_tracks[k]       = x->x_peaks[i];
        x->x_tracks[k].p_tmp = 0;
        x->x_peaks[i].p_tmp  = 0;
    }
    //
    }
    
    /* Unmatched peaks assigned to free peaks. */
    
    for (i = 0; i < count; i++) {
    //
    if (x->x_peaks[i].p_tmp < 0) {
        for (j = 0; j < x->x_numberOfPeaks; j++) {
            if (x->x_tracks[j].p_tmp < 0) {
                x->x_tracks[j]       = x->x_peaks[i];
                x->x_tracks[j].p_tmp = 1;
                x->x_peaks[i].p_tmp  = 0;
                break;
            }
        }
    }
    //
    }
    
    for (i = 0; i < x->x_numberOfPeaks; i++) {
        if (x->x_tracks[i].p_tmp == -1) { x->x_tracks[i].p_amplitude = 0.0; }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sigmund_tilde_proceedOut (t_sigmund_tilde *x, t_sigmundhelper *h)
{
    int i, count;
    
    for (count = x->x_outletsSize - 1; count >= 0; count--) {
    //
    t_variableout *v = &x->x_outlets[count];
    
    switch (v->v_type) {
    //
    case OUT_PITCH    : outlet_float (v->v_outlet, math_frequencyToMidi (h->x_frequency));
                        break;
    case OUT_ENVELOPE : outlet_float (v->v_outlet, math_powerToDecibel (h->x_power));
                        break;
    case OUT_NOTE     : if (h->x_frequencyNote > 0.0) {
                            outlet_float (v->v_outlet, math_frequencyToMidi (h->x_frequencyNote));
                        }
                        break;
    case OUT_PEAKS    :
        for (i = 0; i < h->x_found; i++) {
            t_atom a[5];
            SET_FLOAT (a + 0, i);
            SET_FLOAT (a + 1, x->x_peaks[i].p_frequency);
            SET_FLOAT (a + 2, 2 * x->x_peaks[i].p_amplitude);
            SET_FLOAT (a + 3, 2 * x->x_peaks[i].p_amplitudeReal);
            SET_FLOAT (a + 4, 2 * x->x_peaks[i].p_amplitudeImaginary);
            outlet_list (v->v_outlet, 5, a);
        }
        break;
    case OUT_TRACKS   :
        for (i = 0; i < x->x_numberOfPeaks; i++) {
            t_atom a[4];
            SET_FLOAT (a + 0, i);
            SET_FLOAT (a + 1, x->x_tracks[i].p_frequency);
            SET_FLOAT (a + 2, 2 * x->x_tracks[i].p_amplitude);
            SET_FLOAT (a + 3, x->x_tracks[i].p_tmp);
            outlet_list (v->v_outlet, 4, a);
        }
        break;
    //
    }
    //
    }
}

static void sigmund_tilde_proceed (t_sigmund_tilde *x, int points, t_float *array, t_float sampleRate)
{
    t_sigmundhelper h = { points, 0, 0, 0, 0, sampleRate, (0.5 * sampleRate / points) };
    
    fft_stateInitialize (&x->x_state, points);
    
    sigmund_tilde_proceedRaw (x, array, &h);
    
    PD_ASSERT (x->x_hasPitch || !x->x_hasNote);
    
    /* Below order matters. */
    
    if (x->x_hasPitch) { sigmund_tilde_proceedPitch (x, &h); }
    if (x->x_hasNote)  { sigmund_tilde_proceedNote (x, &h);  }
    if (x->x_hasTrack) { sigmund_tilde_proceedTrack (x, &h); }
    
    sigmund_tilde_proceedOut (x, &h);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sigmund_tilde_taskProceed (t_sigmund_tilde *x)
{
    sigmund_tilde_proceed (x, x->x_points, x->x_buffer, x->x_sampleRate);
    
    if (x->x_hop >= x->x_points) { x->x_index = 0; x->x_count = x->x_hop - x->x_points; }
    else {
        x->x_index = x->x_points - x->x_hop;
        x->x_count = 0;
        memmove (x->x_buffer, x->x_buffer + x->x_hop, x->x_index * sizeof (t_float));
    }
}

static void sigmund_tilde_task (t_sigmund_tilde *x)
{
    if (!x->x_dismissed) {
    //
    int available = fifo32_getAvailableRead (x->x_fifo);
    
    while (available-- > 0) {
    //
    t_sample t; fifo32_read (x->x_fifo, &t, 1);
    
    if (x->x_count > 0) { x->x_count--; }
    else {
        PD_ASSERT (x->x_index < x->x_points);
        t_float *fp = x->x_buffer + x->x_index;
        *fp = (t_float)t;
        x->x_index++;
        if (x->x_index == x->x_points) { sigmund_tilde_taskProceed (x); }
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_int *sigmund_tilde_perform (t_int *w)
{
    t_sigmund_tilde *x = (t_sigmund_tilde *)(w[1]);
    PD_RESTRICTED in   = (t_sample *)(w[2]);
    int n = (int)(w[3]);

    if (fifo32_getAvailableWrite (x->x_fifo) >= n) { fifo32_write (x->x_fifo, in, n); }

    clock_delay (x->x_clock, 0.0);
    
    return (w + 4);
}

static void sigmund_tilde_dsp (t_sigmund_tilde *x, t_signal **sp)
{
    if (x->x_mode == MODE_STREAM) {
    //
    x->x_sampleRate = sp[0]->s_sampleRate;
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add3 (sigmund_tilde_perform, x, sp[0]->s_vector, sp[0]->s_vectorSize);
    //
    }
}

static void sigmund_tilde_list (t_sigmund_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 3) {
    //
    t_garray *a        = NULL;
    t_symbol *name     = atom_getSymbol (argv + 0);
    int points         = atom_getFloat (argv + 1);
    int onset          = atom_getFloat (argv + 2);
    t_float sampleRate = atom_getFloat (argv + 3);
    
    points      = (int)PD_NEXT_POWER_2 (PD_CLAMP (points, 64, FFT_MAXIMUM / 2));
    onset       = PD_MAX (0, onset);
    sampleRate  = PD_MAX (1.0, sampleRate);
    
    t_float *buffer = sigmund_tilde_reserve1 (x, points);
    
    if ((a = garray_fetch (name))) {
    //
    int size = 0; t_word *data = NULL;
    
    garray_getData (a, &size, &data);
    
    if (points + onset > size) { error_invalid (cast_object (x), sym_sigmund__tilde__, sym_array); }
    else {
        int i;
        for (i = 0; i < points; i++) {
            buffer[i] = w_getFloat (data + i + onset);
        }
        sigmund_tilde_proceed (x, points, buffer, sampleRate);
    }
    //
    } else { error_canNotFind (cast_object (x), sym_array, name); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *sigmund_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_sigmund_tilde *x = (t_sigmund_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void sigmund_tilde_functionDismiss (t_object *z)
{
    t_sigmund_tilde *x = (t_sigmund_tilde *)z; x->x_dismissed = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float *sigmund_tilde_reserve1 (t_sigmund_tilde *x, int n)
{
    PD_ASSERT (n > 0);
    
    if (n > x->x_cache1Size) {
    //
    PD_MEMORY_FREE (x->x_cache1); x->x_cache1 = (t_float *)PD_MEMORY_GET (n * sizeof (t_float));
    //
    }
    
    x->x_cache1Size = n;
    
    return x->x_cache1;
}

static t_float *sigmund_tilde_reserve2 (t_sigmund_tilde *x, int n)
{
    PD_ASSERT (n > 0);
    
    if (n > x->x_cache2Size) {
    //
    PD_MEMORY_FREE (x->x_cache2); x->x_cache2 = (t_float *)PD_MEMORY_GET (n * sizeof (t_float));
    //
    }
    
    x->x_cache2Size = n;
    
    return x->x_cache2;
}

static t_float *sigmund_tilde_reserve3 (t_sigmund_tilde *x, int n)
{
    PD_ASSERT (n > 0);
    
    if (n > x->x_cache3Size) {
    //
    PD_MEMORY_FREE (x->x_cache3); x->x_cache3 = (t_float *)PD_MEMORY_GET (n * sizeof (t_float));
    //
    } else {
    //
    memset (x->x_cache3, 0, x->x_cache3Size * sizeof (t_float));
    //
    }
    
    x->x_cache3Size = n;
    
    return x->x_cache3;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *sigmund_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_sigmund_tilde *x = (t_sigmund_tilde *)pd_new (sigmund_tilde_class);
    
    x->x_maximumFrequency = (t_float)1000000;
    x->x_vibrato          = (t_float)1;
    x->x_stableTime       = (t_float)50;
    x->x_growth           = (t_float)7;
    x->x_minimumPower     = (t_float)50;
    x->x_parameter1       = 6.0;                        /* Harmonics. */
    x->x_parameter2       = 0.5;                        /* Power. */
    x->x_sampleRate       = 1.0;
    x->x_mode             = MODE_STREAM;
    x->x_points           = 1024;
    x->x_numberOfPeaks    = 20;
    x->x_hop              = 512;

    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    t_float f   = atom_getFloatAtIndex (1, argc, argv);
    
    if (t == sym___dash__table) {
        x->x_mode = MODE_TABLE;
        argc--; argv++;
        
    } else if (t == sym___dash__stream) {
        x->x_mode = MODE_STREAM;
        argc--; argv++;
        
    } else if (argc > 1 && t == sym___dash__points)     {
        x->x_points = (int)PD_NEXT_POWER_2 (PD_CLAMP (f, 128, FFT_MAXIMUM / 2));
        argc -= 2; argv += 2;

    } else if (argc > 1 && t == sym___dash__hop)        {
        x->x_hop    = (int)PD_NEXT_POWER_2 (PD_CLAMP (f, 64,  FFT_MAXIMUM / 2));
        argc -= 2; argv += 2;

    } else if (argc > 1 && t == sym___dash__peaks)      {
        x->x_numberOfPeaks = PD_CLAMP ((int)f, 1, SIGMUND_PEAKS);
        argc -= 2; argv += 2;
        
    } else if (argc > 1 && t == sym___dash__frequency)  {
        x->x_maximumFrequency = PD_MAX (0.0, f);
        argc -= 2; argv += 2;

    } else if (argc > 1 && t == sym___dash__vibrato)    {
        x->x_vibrato = PD_MAX (0.0, f);
        argc -= 2; argv += 2;

    } else if (argc > 1 && t == sym___dash__time)       {
        x->x_stableTime = PD_MAX (0.0, f);
        argc -= 2; argv += 2;

    } else if (argc > 1 && t == sym___dash__growth)     {
        x->x_growth = PD_MAX (0.0, f);
        argc -= 2; argv += 2;

    } else if (argc > 1 && t == sym___dash__power)      {
        x->x_minimumPower = PD_MAX (0.0, f);
        argc -= 2; argv += 2;
        
    } else {
        break;
    }
    //
    }
    
    error__options (cast_object (x), s, argc, argv);
    
    while (argc > 0) {
    //
    t_symbol *t = atom_getSymbolAtIndex (0, argc, argv);
    
    if (x->x_outletsSize >= SIGMUND_OUTLETS) { break; }
    
    if (t == sym_pitch) {
        x->x_outlets[x->x_outletsSize].v_outlet = outlet_newFloat (cast_object (x));
        x->x_outlets[x->x_outletsSize].v_type   = OUT_PITCH;
        x->x_outletsSize++;
        x->x_hasPitch = 1;
        argc--; argv++;
        
    } else if (t == sym_envelope) {
        x->x_outlets[x->x_outletsSize].v_outlet = outlet_newFloat (cast_object (x));
        x->x_outlets[x->x_outletsSize].v_type   = OUT_ENVELOPE;
        x->x_outletsSize++;
        argc--; argv++;
        
    } else if (t == sym_note) {
        x->x_outlets[x->x_outletsSize].v_outlet = outlet_newFloat (cast_object (x));
        x->x_outlets[x->x_outletsSize].v_type   = OUT_NOTE;
        x->x_outletsSize++;
        x->x_hasPitch = 1;
        x->x_hasNote  = 1;
        argc--; argv++;
        
    } else if (t == sym_peaks) {
        x->x_outlets[x->x_outletsSize].v_outlet = outlet_newList (cast_object (x));
        x->x_outlets[x->x_outletsSize].v_type   = OUT_PEAKS;
        x->x_outletsSize++;
        argc--; argv++;
        
    } else if (t == sym_tracks) {
        x->x_outlets[x->x_outletsSize].v_outlet = outlet_newList (cast_object (x));
        x->x_outlets[x->x_outletsSize].v_type   = OUT_TRACKS;
        x->x_outletsSize++;
        x->x_hasTrack = 1;
        argc--; argv++;
        
    } else {
        break;
    }
    //
    }
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    /* Default is continuous pitch and envelope. */
    
    if (x->x_outletsSize == 0) {
    //
    x->x_outlets[0].v_outlet = outlet_newFloat (cast_object (x));
    x->x_outlets[0].v_type   = OUT_PITCH;
    x->x_outlets[1].v_outlet = outlet_newFloat (cast_object (x));
    x->x_outlets[1].v_type   = OUT_ENVELOPE;
    x->x_outletsSize         = 2;
    x->x_hasPitch            = 1;
    //
    }
    
    PD_ASSERT (PD_IS_POWER_2 (x->x_hop));
    PD_ASSERT (PD_IS_POWER_2 (x->x_points));
    
    x->x_fifo   = fifo32_new();
    x->x_finder = (t_notefinder *)PD_MEMORY_GET (sizeof (t_notefinder));
    x->x_peaks  = (t_peak *)PD_MEMORY_GET (x->x_numberOfPeaks * sizeof (t_peak));
    x->x_tracks = (t_peak *)PD_MEMORY_GET (x->x_numberOfPeaks * sizeof (t_peak));
    x->x_cache1 = (t_float *)PD_MEMORY_GET (0);
    x->x_cache2 = (t_float *)PD_MEMORY_GET (0);
    x->x_cache3 = (t_float *)PD_MEMORY_GET (0);
    x->x_buffer = (t_float *)PD_MEMORY_GET (x->x_points * sizeof (t_float));
    x->x_clock  = clock_newSafe ((void *)x, (t_method)sigmund_tilde_task);
    
    return x;
}

static void sigmund_tilde_free (t_sigmund_tilde *x)
{
    fft_stateRelease (&x->x_state);
    
    clock_free (x->x_clock);
    
    PD_MEMORY_FREE (x->x_buffer);
    PD_MEMORY_FREE (x->x_cache3);
    PD_MEMORY_FREE (x->x_cache2);
    PD_MEMORY_FREE (x->x_cache1);
    PD_MEMORY_FREE (x->x_tracks);
    PD_MEMORY_FREE (x->x_peaks);
    PD_MEMORY_FREE (x->x_finder);
    
    fifo32_free (x->x_fifo);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void sigmund_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_sigmund__tilde__,
            (t_newmethod)sigmund_tilde_new,
            (t_method)sigmund_tilde_free,
            sizeof (t_sigmund_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_GIMME,
            A_NULL);
    
    class_addDsp (c, (t_method)sigmund_tilde_dsp);
    
    class_addList (c, (t_method)sigmund_tilde_list);
        
    class_setDataFunction (c, sigmund_tilde_functionData);
    class_setDismissFunction (c, sigmund_tilde_functionDismiss);
    
    sigmund_tilde_class = c;
}

void sigmund_tilde_destroy (void)
{
    class_free (sigmund_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
