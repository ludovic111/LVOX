#include "Compressor.h"

void CompressorModule::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    sidechainHPF.prepare (spec);
    envelope = 0.0f;
    currentGainReductionDB = 0.0f;
}

void CompressorModule::process (juce::dsp::AudioBlock<float>& block)
{
    const float threshDB  = thresholdParam->load();
    const float ratio     = ratioParam->load();
    const float attackMs  = attackParam->load();
    const float releaseMs = releaseParam->load();
    const float kneeDB    = kneeParam->load();
    const float makeupDB  = makeupParam->load();
    const float scFreq    = scFreqParam->load();
    const bool  scActive  = scActiveParam->load() > 0.5f;
    const float mixPct    = mixParam->load() / 100.0f;

    const float makeupGain = juce::Decibels::decibelsToGain (makeupDB);
    const float attackCoeff  = std::exp (-1.0f / (float (sampleRate) * attackMs  * 0.001f));
    const float releaseCoeff = std::exp (-1.0f / (float (sampleRate) * releaseMs * 0.001f));

    const auto numChannels = block.getNumChannels();
    const auto numSamples  = block.getNumSamples();

    // Sidechain buffer for HPF detection
    juce::AudioBuffer<float> scBuffer ((int) numChannels, (int) numSamples);
    for (size_t ch = 0; ch < numChannels; ++ch)
        scBuffer.copyFrom ((int) ch, 0, block.getChannelPointer (ch), (int) numSamples);

    if (scActive)
    {
        auto scCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, scFreq);
        *sidechainHPF.state = *scCoeffs;
        juce::dsp::AudioBlock<float> scBlock (scBuffer);
        juce::dsp::ProcessContextReplacing<float> scCtx (scBlock);
        sidechainHPF.process (scCtx);
    }

    // Store dry signal for parallel compression
    juce::AudioBuffer<float> dryBuffer ((int) numChannels, (int) numSamples);
    for (size_t ch = 0; ch < numChannels; ++ch)
        dryBuffer.copyFrom ((int) ch, 0, block.getChannelPointer (ch), (int) numSamples);

    for (size_t i = 0; i < numSamples; ++i)
    {
        // Peak detection on sidechain
        float peak = 0.0f;
        for (size_t ch = 0; ch < numChannels; ++ch)
            peak = std::max (peak, std::abs (scBuffer.getSample ((int) ch, (int) i)));

        float peakDB = juce::Decibels::gainToDecibels (peak, -100.0f);

        // Envelope follower
        if (peakDB > envelope)
            envelope = attackCoeff * envelope + (1.0f - attackCoeff) * peakDB;
        else
            envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * peakDB;

        // Gain computation with soft knee
        float gainReductionDB = 0.0f;
        float halfKnee = kneeDB / 2.0f;

        if (envelope > threshDB + halfKnee)
        {
            gainReductionDB = (envelope - threshDB) * (1.0f - 1.0f / ratio);
        }
        else if (envelope > threshDB - halfKnee)
        {
            float x = envelope - threshDB + halfKnee;
            gainReductionDB = (x * x) / (4.0f * kneeDB) * (1.0f - 1.0f / ratio);
        }

        currentGainReductionDB = gainReductionDB;
        float gain = juce::Decibels::decibelsToGain (-gainReductionDB) * makeupGain;

        for (size_t ch = 0; ch < numChannels; ++ch)
            block.setSample ((int) ch, (int) i, block.getSample ((int) ch, (int) i) * gain);
    }

    // Parallel mix
    if (mixPct < 1.0f)
    {
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
}

void CompressorModule::reset()
{
    sidechainHPF.reset();
    envelope = 0.0f;
    currentGainReductionDB = 0.0f;
}
