#include "ParametricEQ.h"

void ParametricEQModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    for (auto& band : bands)
        band.filter.prepare (spec);
}

void ParametricEQModule::updateBand (Band& band)
{
    float freq = band.freqParam->load();
    float gain = band.gainParam->load();
    float q    = band.qParam->load();
    int type   = (int) band.typeParam->load();

    if (micCorrection != nullptr)
    {
        float offset = 0.0f;
        switch (band.index)
        {
            case 0: offset = micCorrection->eqBand1GainOffset; break;
            case 1: offset = micCorrection->eqBand2GainOffset; break;
            case 2: offset = micCorrection->eqBand3GainOffset; break;
            case 3: offset = micCorrection->eqBand4GainOffset; break;
        }
        gain = juce::jlimit (-18.0f, 18.0f, gain + offset);
    }

    juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>> coeffs;

    switch (type)
    {
        case 0: // Peak
            coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
        case 1: // Low Shelf
            coeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
        case 2: // High Shelf
            coeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
        default:
            coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
    }

    *band.filter.state = *coeffs;
}

void ParametricEQModule::process (juce::dsp::AudioBlock<float>& block)
{
    juce::dsp::ProcessContextReplacing<float> context (block);

    for (auto& band : bands)
    {
        if (band.activeParam->load() > 0.5f)
        {
            updateBand (band);
            band.filter.process (context);
        }
    }
}

void ParametricEQModule::reset()
{
    for (auto& band : bands)
        band.filter.reset();
}
