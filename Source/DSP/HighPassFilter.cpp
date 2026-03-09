#include "HighPassFilter.h"

void HighPassFilterModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    for (auto& f : filters)
        f.prepare (spec);
    lastFreq = -1.0f;
    lastSlope = -1.0f;
}

void HighPassFilterModule::updateFilters()
{
    float freq = frequencyParam->load();
    float slope = slopeParam->load();

    if (freq == lastFreq && slope == lastSlope)
        return;

    lastFreq = freq;
    lastSlope = slope;

    int slopeIndex = (int) slope;
    activeFilterCount = slopeIndex + 1; // 0->1 (12dB), 1->2 (24dB), 2->4 (48dB)
    if (slopeIndex == 2) activeFilterCount = 4;

    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, freq);
    for (int i = 0; i < activeFilterCount; ++i)
        *filters[i].state = *coeffs;
}

void HighPassFilterModule::process (juce::dsp::AudioBlock<float>& block)
{
    updateFilters();
    juce::dsp::ProcessContextReplacing<float> context (block);
    for (int i = 0; i < activeFilterCount; ++i)
        filters[i].process (context);
}

void HighPassFilterModule::reset()
{
    for (auto& f : filters)
        f.reset();
    lastFreq = -1.0f;
    lastSlope = -1.0f;
}
