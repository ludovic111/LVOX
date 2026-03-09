#include "Limiter.h"

void LimiterModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    gainReduction = 1.0f;
}

void LimiterModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float ceilingDB = ceilingParam->load();
    const float releaseMs = releaseParam->load();

    const float ceiling = juce::Decibels::decibelsToGain (ceilingDB);
    const float releaseCoeff = std::exp (-1.0f / (float (sampleRate) * releaseMs * 0.001f));

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    for (size_t i = 0; i < numSamples; ++i)
    {
        // Find peak across channels
        float peak = 0.0f;
        for (size_t ch = 0; ch < numChannels; ++ch)
            peak = std::max (peak, std::abs (block.getSample ((int) ch, (int) i)));

        // Compute target gain
        float targetGain = 1.0f;
        if (peak > ceiling && peak > 0.0f)
            targetGain = ceiling / peak;

        // Smooth: instant attack, smooth release
        if (targetGain < gainReduction)
            gainReduction = targetGain;
        else
            gainReduction = releaseCoeff * gainReduction + (1.0f - releaseCoeff) * targetGain;

        // Apply
        for (size_t ch = 0; ch < numChannels; ++ch)
            block.setSample ((int) ch, (int) i, block.getSample ((int) ch, (int) i) * gainReduction);
    }
}

void LimiterModule::reset()
{
    gainReduction = 1.0f;
}
