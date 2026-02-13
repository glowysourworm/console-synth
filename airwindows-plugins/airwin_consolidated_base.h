/*
 * AirwinConslidated - an adaptation of the airwindows effect suite for
 * open source clients
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#ifndef AIRWIN_CONSOLIDATED_AIRWINBASE_H
#define AIRWIN_CONSOLIDATED_AIRWINBASE_H

// Windows SDK Patch:  8.1 -> 10.x  (This should be fixed by editing project settings! But, this 
// one was the bulk of the build errors)
// ---------------------------------------------------------------------------------------------
#ifndef _MATH_DEFINES_DEFINED
#define _MATH_DEFINES_DEFINED

#include <corecrt.h>

#pragma warning(push)
#pragma warning(disable: _UCRT_DISABLED_WARNINGS)
_UCRT_DISABLE_CLANG_WARNINGS

// Definitions of useful mathematical constants
//
// Define _USE_MATH_DEFINES before including <math.h> to expose these macro
// definitions for common math constants.  These are placed under an #ifdef
// since these commonly-defined names are not part of the C or C++ standards
#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

_UCRT_RESTORE_CLANG_WARNINGS
#pragma warning(pop) // _UCRT_DISABLED_WARNINGS
#endif // _MATH_DEFINES_DEFINED
// -------------------------------------------------------------------------

#include <cstdint>
#include <cassert>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <cstdio>
#include <exception>
#include <string>

typedef int32_t VstInt32;
typedef int32_t VstPlugCategory;
static constexpr int32_t kPlugCategEffect{1};
static constexpr int32_t kVstMaxProgNameLen{64};
static constexpr int32_t kVstMaxParamStrLen{32};
static constexpr int32_t kVstMaxProductStrLen{32};
static constexpr int32_t kVstMaxVendorStrLen{32};

struct AirwinConsolidatedBase
{
    AirwinConsolidatedBase(float samplingRate, int, int) 
    {
        sampleRate = samplingRate;
    }
    virtual ~AirwinConsolidatedBase() = default;

    float sampleRate;
    void setSampleRate(float sr) { sampleRate = sr; }
    float getSampleRate()
    {
        if (sampleRate < 2000)
            throw std::exception("Sampling rate initialization error (Airwindows Plugin Registry)");

        return sampleRate;
    }

    virtual bool getEffectName(char *name) { return false; }
    virtual void setNumInputs(int) {}
    virtual void setNumOutputs(int) {}
    virtual void setUniqueID(int) {}
    virtual bool canProcessReplacing() { return true; }
    virtual bool canDoubleReplacing() { return false; }
    virtual void programsAreChunks(bool) {}

    virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) = 0;
    virtual void processDoubleReplacing(double **inputs, double **outputs,
                                        VstInt32 sampleFrames) = 0;

    virtual float
    getParameter(VstInt32 index) = 0; // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index,
                              float value) = 0; // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index,
                                   char *text) = 0; // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index,
                                  char *text) = 0; // label for the parameter (eg dB)
    virtual void getParameterDisplay(VstInt32 index,
                                     char *text) = 0; // text description of the current value

    virtual bool parameterTextToValue(VstInt32 index, const char *text, float &value)
    {
        return false;
    }
    virtual bool canConvertParameterTextToValue(VstInt32 index) { return false; }
};

typedef AirwinConsolidatedBase AudioEffectX;
typedef AudioEffectX AudioEffect;

inline void vst_strncpy(char *dst, const char *src, int n) { strncpy(dst, src, n); }
inline void float2string(float f, char *txt, int n) { snprintf(txt, n, "%8.4f", f); }
bool string2float(const char *txt, float &f);

inline void int2string(int i, char *t, size_t num) { snprintf(t, num, "%d", i); }
inline void dB2string(float value, char *t, size_t num)
{
    if (value <= 0.00001) // -100 dB, show -inf from that point onwards
        vst_strncpy(t, "-inf", num);
    else
        float2string((float)(20.0 * log10(value)), t, num);
}
bool string2dBNorm(const char *txt, float &f);

#endif
