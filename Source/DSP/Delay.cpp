#include "Delay.h"

void DelayModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    delayLine.prepare (spec);
    delayLine.setMaximumDelayInSamples ((int) (sampleRate * 2.0 + 1));
    feedbackBuffer.resize (spec.numChannels, 0.0f);

    // Prepare per-channel feedback filters
    fbLowCutFilters.resize (spec.numChannels);
    fbHighCutFilters.resize (spec.numChannels);
    for (auto& f : fbLowCutFilters)
        f.reset();
    for (auto& f : fbHighCutFilters)
        f.reset();
}

float DelayModule::getDelayTimeMs() const
{
    bool sync = syncParam->load() > 0.5f;

    if (!sync)
        return timeParam->load();

    int noteIndex = (int) noteParam->load();
    // Note divisions: 1/4, 1/8, 1/8D, 1/8T, 1/16, 1/16D, 1/16T
    static const float noteMultipliers[] = { 1.0f, 0.5f, 0.75f, 1.0f/3.0f, 0.25f, 0.375f, 1.0f/6.0f };
    float beatMs = 60000.0f / (float) hostBPM;
    return beatMs * noteMultipliers[noteIndex];
}

void DelayModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float feedback = feedbackParam->load() / 100.0f;
    const float lowCut   = lowCutParam->load();
    const float highCut  = highCutParam->load();
    const float mixPct   = mixParam->load() / 100.0f;
    const float delayMs  = getDelayTimeMs();

    float delaySamples = delayMs * 0.001f * (float) sampleRate;
    delaySamples = std::clamp (delaySamples, 1.0f, (float) (sampleRate * 2.0));
    delayLine.setDelay (delaySamples);

    // Update feedback filter coefficients
    auto lcCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, lowCut);
    auto hcCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, highCut);

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        if (ch < fbLowCutFilters.size())
        {
            fbLowCutFilters[ch].coefficients = lcCoeffs;
            fbHighCutFilters[ch].coefficients = hcCoeffs;
        }
    }

    for (size_t i = 0; i < numSamples; ++i)
    {
        for (size_t ch = 0; ch < numChannels; ++ch)
        {
            float input = block.getSample ((int) ch, (int) i);
            float delayed = delayLine.popSample ((int) ch);

            // Apply feedback filters to the feedback signal
            float fbSample = delayed * feedback;
            if (ch < fbLowCutFilters.size())
            {
                fbSample = fbLowCutFilters[ch].processSample (fbSample);
                fbSample = fbHighCutFilters[ch].processSample (fbSample);
            }

            delayLine.pushSample ((int) ch, input + fbSample);

            // Mix dry/wet
            block.setSample ((int) ch, (int) i, input + delayed * mixPct);
        }
    }
}

void DelayModule::reset()
{
    delayLine.reset();
    for (auto& f : fbLowCutFilters)
        f.reset();
    for (auto& f : fbHighCutFilters)
        f.reset();
    std::fill (feedbackBuffer.begin(), feedbackBuffer.end(), 0.0f);
}
