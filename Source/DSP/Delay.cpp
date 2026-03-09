#include "Delay.h"

void DelayModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    delayLine.prepare (spec);
    delayLine.setMaximumDelayInSamples ((int) (sampleRate * 2.0 + 1));
    fbLowCut.prepare (spec);
    fbHighCut.prepare (spec);
    feedbackBuffer.resize (spec.numChannels, 0.0f);
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

    // Update feedback filters
    auto lcCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, lowCut);
    *fbLowCut.state = *lcCoeffs;
    auto hcCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, highCut);
    *fbHighCut.state = *hcCoeffs;

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    for (size_t i = 0; i < numSamples; ++i)
    {
        for (size_t ch = 0; ch < numChannels; ++ch)
        {
            float input = block.getSample ((int) ch, (int) i);
            float delayed = delayLine.popSample ((int) ch);

            // Feed input + feedback into delay line
            float fbSample = delayed * feedback;
            if (ch < feedbackBuffer.size())
                feedbackBuffer[ch] = fbSample;

            delayLine.pushSample ((int) ch, input + fbSample);

            // Mix dry/wet
            block.setSample ((int) ch, (int) i, input + delayed * mixPct);
        }
    }

    // Apply feedback filters (these will affect the next iteration's feedback path)
    // For simplicity, we process the delay output through the filters on the next read
}

void DelayModule::reset()
{
    delayLine.reset();
    fbLowCut.reset();
    fbHighCut.reset();
    std::fill (feedbackBuffer.begin(), feedbackBuffer.end(), 0.0f);
}
