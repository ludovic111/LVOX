#include "DeEsser.h"

void DeEsserModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    bandpassFilter.prepare (spec);
    envelope = 0.0f;
}

void DeEsserModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float freq      = frequencyParam->load();
    const float threshDB  = thresholdParam->load();
    const float maxRedDB  = reductionParam->load();
    const bool  listen    = listenParam->load() > 0.5f;

    const float threshold = juce::Decibels::decibelsToGain (threshDB);
    const float q = 2.0f;

    // Update bandpass coefficients
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, freq, q);
    *bandpassFilter.state = *coeffs;

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    // Create a copy for sidechain detection
    juce::AudioBuffer<float> scBuffer ((int) numChannels, (int) numSamples);
    for (size_t ch = 0; ch < numChannels; ++ch)
        scBuffer.copyFrom ((int) ch, 0, block.getChannelPointer (ch), (int) numSamples);

    juce::dsp::AudioBlock<float> scBlock (scBuffer);
    juce::dsp::ProcessContextReplacing<float> scContext (scBlock);
    bandpassFilter.process (scContext);

    if (listen)
    {
        // Solo sidechain signal
        for (size_t ch = 0; ch < numChannels; ++ch)
            for (size_t i = 0; i < numSamples; ++i)
                block.setSample ((int) ch, (int) i, scBlock.getSample ((int) ch, (int) i));
        return;
    }

    const float releaseCoeff = std::exp (-1.0f / (float (sampleRate) * 0.01f));

    for (size_t i = 0; i < numSamples; ++i)
    {
        float scPeak = 0.0f;
        for (size_t ch = 0; ch < numChannels; ++ch)
            scPeak = std::max (scPeak, std::abs (scBlock.getSample ((int) ch, (int) i)));

        if (scPeak > envelope)
            envelope = scPeak;
        else
            envelope = releaseCoeff * envelope;

        float gainReduction = 1.0f;
        if (envelope > threshold)
        {
            float overDB = juce::Decibels::gainToDecibels (envelope) - threshDB;
            float reductionDB = std::min (overDB, maxRedDB);
            gainReduction = juce::Decibels::decibelsToGain (-reductionDB);
        }

        for (size_t ch = 0; ch < numChannels; ++ch)
            block.setSample ((int) ch, (int) i, block.getSample ((int) ch, (int) i) * gainReduction);
    }
}

void DeEsserModule::reset()
{
    bandpassFilter.reset();
    envelope = 0.0f;
}
