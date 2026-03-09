#include "NoiseGate.h"

void NoiseGateModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    envelope = 0.0f;
    currentGain = 1.0f;
}

void NoiseGateModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float thresholdDB = thresholdParam->load();
    const float ratio       = ratioParam->load();
    const float attackMs    = attackParam->load();
    const float releaseMs   = releaseParam->load();
    const float rangeDB     = rangeParam->load();

    const float threshold = juce::Decibels::decibelsToGain (thresholdDB);
    const float rangeLin  = juce::Decibels::decibelsToGain (rangeDB);

    const float attackCoeff  = std::exp (-1.0f / (float (sampleRate) * attackMs  * 0.001f));
    const float releaseCoeff = std::exp (-1.0f / (float (sampleRate) * releaseMs * 0.001f));

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    for (size_t i = 0; i < numSamples; ++i)
    {
        // Peak detection across channels
        float peak = 0.0f;
        for (size_t ch = 0; ch < numChannels; ++ch)
            peak = std::max (peak, std::abs (block.getSample ((int) ch, (int) i)));

        // Envelope follower
        if (peak > envelope)
            envelope = attackCoeff * envelope + (1.0f - attackCoeff) * peak;
        else
            envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * peak;

        // Gain computation
        float targetGain = 1.0f;
        if (envelope < threshold)
        {
            float dbBelow = juce::Decibels::gainToDecibels (envelope) - thresholdDB;
            float dbReduction = dbBelow * (1.0f - 1.0f / ratio);
            targetGain = juce::Decibels::decibelsToGain (dbReduction);
            targetGain = std::max (targetGain, rangeLin);
        }

        // Smooth gain changes
        currentGain = currentGain * 0.999f + targetGain * 0.001f;

        for (size_t ch = 0; ch < numChannels; ++ch)
            block.setSample ((int) ch, (int) i, block.getSample ((int) ch, (int) i) * currentGain);
    }
}

void NoiseGateModule::reset()
{
    envelope = 0.0f;
    currentGain = 1.0f;
}
