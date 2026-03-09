#include "LVOXReverb.h"

void ReverbModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    reverb.prepare (spec);
    predelayLine.prepare (spec);
    predelayLine.setMaximumDelayInSamples ((int) (sampleRate * 0.2f + 1));
    lowCutFilter.prepare (spec);
    highCutFilter.prepare (spec);
}

void ReverbModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float size     = sizeParam->load() / 100.0f;
    const float damping  = dampingParam->load() / 100.0f;
    const float preMs    = predelayParam->load();
    const float lowCut   = lowCutParam->load();
    const float highCut  = highCutParam->load();
    const float mixPct   = mixParam->load() / 100.0f;

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    // Save dry signal
    juce::AudioBuffer<float> dryBuffer ((int) numChannels, (int) numSamples);
    for (size_t ch = 0; ch < numChannels; ++ch)
        dryBuffer.copyFrom ((int) ch, 0, block.getChannelPointer (ch), (int) numSamples);

    // Pre-delay
    float preSamples = preMs * 0.001f * (float) sampleRate;
    predelayLine.setDelay (preSamples);
    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        auto* data = block.getChannelPointer (ch);
        for (size_t i = 0; i < numSamples; ++i)
        {
            predelayLine.pushSample ((int) ch, data[i]);
            data[i] = predelayLine.popSample ((int) ch);
        }
    }

    // Reverb
    juce::dsp::Reverb::Parameters params;
    params.roomSize   = size;
    params.damping    = damping;
    params.wetLevel   = 1.0f;
    params.dryLevel   = 0.0f;
    params.width      = 1.0f;
    params.freezeMode = 0.0f;
    reverb.setParameters (params);
    juce::dsp::ProcessContextReplacing<float> reverbCtx (block);
    reverb.process (reverbCtx);

    // Post-reverb filters
    auto lcCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, lowCut);
    *lowCutFilter.state = *lcCoeffs;
    auto hcCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, highCut);
    *highCutFilter.state = *hcCoeffs;

    juce::dsp::ProcessContextReplacing<float> filterCtx (block);
    lowCutFilter.process (filterCtx);
    highCutFilter.process (filterCtx);

    // Dry/wet mix
    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        for (size_t i = 0; i < numSamples; ++i)
        {
            float wet = block.getSample ((int) ch, (int) i);
            float dry = dryBuffer.getSample ((int) ch, (int) i);
            block.setSample ((int) ch, (int) i, dry + mixPct * (wet - dry));
        }
    }
}

void ReverbModule::reset()
{
    reverb.reset();
    predelayLine.reset();
    lowCutFilter.reset();
    highCutFilter.reset();
}
