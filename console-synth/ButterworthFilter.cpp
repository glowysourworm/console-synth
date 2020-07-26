#include "ButterworthFilter.h"

ButterworthFilter::ButterworthFilter(int samplingRate, float gain)
{
    this->history1 = 0.f;
    this->history2 = 0.f;
    this->history3 = 0.f;
    this->history4 = 0.f;

    this->gain = gain;
    this->SetSampleRate((float)samplingRate);
    this->Set(samplingRate / 2.0f, 0.0);
}

ButterworthFilter::~ButterworthFilter()
{

}

void ButterworthFilter::SetSampleRate(float samplingRate)
{
    float pi = 4.f * atanf(1.f);

    this->t0 = 4.f * samplingRate * samplingRate;
    this->t1 = 8.f * samplingRate * samplingRate;
    this->t2 = 2.f * samplingRate;
    this->t3 = pi / samplingRate;

    this->min_cutoff = samplingRate * 0.01f;
    this->max_cutoff = samplingRate * 0.45f;
}

void ButterworthFilter::Set(float cutoffFrequency, float resonance)
{
    if (cutoffFrequency < this->min_cutoff)
        cutoffFrequency = this->min_cutoff;
    else if (cutoffFrequency > this->max_cutoff)
        cutoffFrequency = this->max_cutoff;

    if (resonance < 0.f)
        resonance = 0.f;
    else if (resonance > 1.f)
        resonance = 1.f;

    float wp = this->t2 * tanf(this->t3 * cutoffFrequency);
    float bd, bd_tmp, b1, b2;

    resonance *= BUDDA_Q_SCALE;
    resonance += 1.f;

    b1 = (0.765367f / resonance) / wp;
    b2 = 1.f / (wp * wp);

    bd_tmp = this->t0 * b2 + 1.f;

    bd = 1.f / (bd_tmp + this->t2 * b1);

    this->gain = bd * 0.5f;

    this->coef2 = (2.f - this->t1 * b2);

    this->coef0 = this->coef2 * bd;
    this->coef1 = (bd_tmp - this->t2 * b1) * bd;

    b1 = (1.847759f / resonance) / wp;

    bd = 1.f / (bd_tmp + this->t2 * b1);

    this->gain *= bd;
    this->coef2 *= bd;
    this->coef3 = (bd_tmp - this->t2 * b1) * bd;
}

float ButterworthFilter::Apply(float sample)
{
    float output = sample * this->gain;
    float new_hist;

    output -= this->history1 * this->coef0;
    new_hist = output - this->history2 * this->coef1;

    output = new_hist + this->history1 * 2.f;
    output += this->history2;

    this->history2 = this->history1;
    this->history1 = new_hist;

    output -= this->history3 * this->coef2;
    new_hist = output - this->history4 * this->coef3;

    output = new_hist + this->history3 * 2.f;
    output += this->history4;

    this->history4 = this->history3;
    this->history3 = new_hist;

    return output;
}
