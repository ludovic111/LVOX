#include "Saturation.h"

void SaturationModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    oversampling.initProcessing ((size_t) spec.maximumBlockSize);
    toneFilter.prepare (spec);
}

float SaturationModule::processTape (float x, float drive)
{
    return std::tanh (x * drive);
}

float SaturationModule::processTube (float x, float drive)
{
    float driven = x * drive;
    return driven / (1.0f + std::abs (driven)) + 0.1f * driven * driven; // asymmetric even harmonics
}

float SaturationModule::processSoftClip (float x, float drive)
{
    float driven = x * drive;
    if (std::abs (driven) < 1.0f)
        return driven - (driven * driven * driven) / 3.0f;
    return (driven > 0.0f) ? 2.0f / 3.0f : -2.0f / 3.0f;
}

float SaturationModule::processHardClip (float x, float drive)
{
    return std::clamp (x * drive, -1.0f, 1.0f);
}

void SaturationModule::process (juce::dsp::AudioBlock<float>& block)
{
    float drivePct = driveParam->load();
    if (micCorrection != nullptr)
        drivePct = juce::jlimit (0.0f, 100.0f, drivePct + micCorrection->satDriveOffset);
    const int   type     = (int) typeParam->load();
    const float tone     = toneParam->load();
    const float mixPct   = mixParam->load() / 100.0f;
    const float outputDB = outputParam->load();

    const float drive = 1.0f + drivePct * 0.09f; // 1.0 to 10.0
    const float outputGain = juce::Decibels::decibelsToGain (outputDB);

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    // Save dry signal
    juce::AudioBuffer<float> dryBuffer ((int) numChannels, (int) numSamples);
    for (size_t ch = 0; ch < numChannels; ++ch)
        dryBuffer.copyFrom ((int) ch, 0, block.getChannelPointer (ch), (int) numSamples);

    // Upsample
    auto osBlock = oversampling.processSamplesUp (block);

    // Apply waveshaping
    for (size_t ch = 0; ch < osBlock.getNumChannels(); ++ch)
    {
        auto* data = osBlock.getChannelPointer (ch);
        for (size_t i = 0; i < osBlock.getNumSamples(); ++i)
        {
            switch (type)
            {
                case 0: data[i] = processTape     (data[i], drive); break;
                case 1: data[i] = processTube     (data[i], drive); break;
                case 2: data[i] = processSoftClip (data[i], drive); break;
                case 3: data[i] = processHardClip (data[i], drive); break;
            }
        }
    }

    // Downsample
    oversampling.processSamplesDown (block);

    // Tone control (tilt EQ)
    float toneFreq = 1000.0f;
    float toneGain = tone / 100.0f * 6.0f; // +/- 6dB
    auto toneCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, toneFreq, 0.7f, juce::Decibels::decibelsToGain (toneGain));
    *toneFilter.state = *toneCoeffs;
    juce::dsp::ProcessContextReplacing<float> toneCtx (block);
    toneFilter.process (toneCtx);

    // Apply output gain and dry/wet mix
    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        for (size_t i = 0; i < numSamples; ++i)
        {
            float wet = block.getSample ((int) ch, (int) i) * outputGain;
            float dry = dryBuffer.getSample ((int) ch, (int) i);
            block.setSample ((int) ch, (int) i, dry + mixPct * (wet - dry));
        }
    }
}

void SaturationModule::reset()
{
    oversampling.reset();
    toneFilter.reset();
}
