#include "LVOXReverb.h"

// Dattorro plate reverb delay lengths (at 29761 Hz reference rate, scaled to actual SR)
// Based on Jon Dattorro's "Effect Design Part 1" paper

void DattorroPlate::prepare (double sampleRate)
{
    sr = sampleRate;
    float scale = (float) (sr / 29761.0);

    // Input diffusion allpasses
    inputAP[0].init ((int) (142.0f * scale), 0.75f);
    inputAP[1].init ((int) (107.0f * scale), 0.75f);
    inputAP[2].init ((int) (379.0f * scale), 0.625f);
    inputAP[3].init ((int) (277.0f * scale), 0.625f);

    // Tank channel 0
    tankAP[0].init ((int) (672.0f * scale), -0.7f);
    tankDelay1[0].init ((int) (4453.0f * scale));
    tankDelay2[0].init ((int) (3720.0f * scale));

    // Tank channel 1
    tankAP[1].init ((int) (908.0f * scale), -0.7f);
    tankDelay1[1].init ((int) (4217.0f * scale));
    tankDelay2[1].init ((int) (3163.0f * scale));

    reset();
}

void DattorroPlate::process (float inputL, float inputR, float& outL, float& outR)
{
    float mono = (inputL + inputR) * 0.5f;

    // Input diffusion
    float v = mono;
    for (auto& ap : inputAP)
        v = ap.process (v);

    // Set tank allpass feedback based on decay
    float apFb = -0.7f * juce::jlimit (0.1f, 0.99f, decay);
    tankAP[0].feedback = apFb;
    tankAP[1].feedback = apFb;

    // Set damping
    tankDamp[0].coeff = damping;
    tankDamp[1].coeff = damping;

    // Tank processing: cross-feedback between two channels
    float tank0In = v + tankDelay2[1].tap() * decay;
    float tank1In = v + tankDelay2[0].tap() * decay;

    // Channel 0: allpass → delay → damp → delay
    float t0 = tankAP[0].process (tank0In);
    tankDelay1[0].push (t0);
    float d0 = tankDamp[0].process (tankDelay1[0].tap());
    tankDelay2[0].push (d0 * decay);

    // Channel 1: allpass → delay → damp → delay
    float t1 = tankAP[1].process (tank1In);
    tankDelay1[1].push (t1);
    float d1 = tankDamp[1].process (tankDelay1[1].tap());
    tankDelay2[1].push (d1 * decay);

    // Output taps (decorrelated stereo from both tank channels)
    int s = (int) (sr / 29761.0);
    outL = tankDelay1[0].read (266 * s) + tankDelay1[0].read (2974 * s)
         - tankAP[1].buffer[(tankAP[1].writeIdx + 1333 * s) % tankAP[1].size]
         + tankDelay2[1].read (1913 * s)
         - tankDelay1[1].read (1996 * s)
         - tankAP[0].buffer[(tankAP[0].writeIdx + 1066 * s) % tankAP[0].size]
         - tankDelay2[0].read (187 * s);

    outR = tankDelay1[1].read (353 * s) + tankDelay1[1].read (3627 * s)
         - tankAP[0].buffer[(tankAP[0].writeIdx + 1228 * s) % tankAP[0].size]
         + tankDelay2[0].read (2656 * s)
         - tankDelay1[0].read (2111 * s)
         - tankAP[1].buffer[(tankAP[1].writeIdx + 335 * s) % tankAP[1].size]
         - tankDelay2[1].read (121 * s);

    outL *= 0.6f;
    outR *= 0.6f;
}

void DattorroPlate::reset()
{
    for (auto& ap : inputAP) ap.clear();
    for (auto& ap : tankAP) ap.clear();
    for (auto& d : tankDelay1) d.clear();
    for (auto& d : tankDelay2) d.clear();
    for (auto& lp : tankDamp) lp.clear();
}

// ---- ReverbModule ----

void ReverbModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    plate.prepare (spec.sampleRate);
    predelayLine.prepare (spec);
    predelayLine.setMaximumDelayInSamples ((int) (sampleRate * 0.2f + 1));
    lowCutFilter.prepare (spec);
    highCutFilter.prepare (spec);
}

void ReverbModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float size     = sizeParam->load() / 100.0f;
    const float dampVal  = dampingParam->load() / 100.0f;
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

    // Set plate reverb parameters
    plate.setDecay (0.1f + size * 0.89f); // Map 0-100% → 0.1-0.999
    plate.setDamping (dampVal);
    plate.setSize (0.5f + size * 1.5f);

    // Process through Dattorro plate
    if (numChannels >= 2)
    {
        auto* dataL = block.getChannelPointer (0);
        auto* dataR = block.getChannelPointer (1);
        for (size_t i = 0; i < numSamples; ++i)
            plate.process (dataL[i], dataR[i], dataL[i], dataR[i]);
    }
    else if (numChannels == 1)
    {
        auto* data = block.getChannelPointer (0);
        for (size_t i = 0; i < numSamples; ++i)
        {
            float outL, outR;
            plate.process (data[i], data[i], outL, outR);
            data[i] = (outL + outR) * 0.5f;
        }
    }

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
    plate.reset();
    predelayLine.reset();
    lowCutFilter.reset();
    highCutFilter.reset();
}
