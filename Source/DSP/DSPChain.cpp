#include "DSPChain.h"
#include "../Parameters.h"

DSPChain::DSPChain (juce::AudioProcessorValueTreeState& apvts)
    : apvts (apvts),
      noiseGate (apvts),
      highPassFilter (apvts),
      deEsser (apvts),
      parametricEQ (apvts),
      compressor (apvts),
      saturation (apvts),
      reverbModule (apvts),
      delayModule (apvts),
      limiter (apvts)
{
    inputGainParam  = apvts.getRawParameterValue (ParamIDs::globalInput);
    outputGainParam = apvts.getRawParameterValue (ParamIDs::globalOutput);

    modules[0] = &noiseGate;
    modules[1] = &highPassFilter;
    modules[2] = &deEsser;
    modules[3] = &parametricEQ;
    modules[4] = &compressor;
    modules[5] = &saturation;
    modules[6] = &reverbModule;
    modules[7] = &delayModule;
    modules[8] = &limiter;
}

void DSPChain::prepare (const juce::dsp::ProcessSpec& spec)
{
    for (auto* module : modules)
        module->prepare (spec);
}

void DSPChain::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);

    // Input gain
    float inputGain = juce::Decibels::decibelsToGain (inputGainParam->load());
    block.multiplyBy (inputGain);

    // Process each module in sequence
    for (auto* module : modules)
    {
        if (!module->isBypassed())
            module->process (block);
    }

    // Output gain
    float outputGain = juce::Decibels::decibelsToGain (outputGainParam->load());
    block.multiplyBy (outputGain);
}

void DSPChain::reset()
{
    for (auto* module : modules)
        module->reset();
}

void DSPChain::setHostBPM (double bpm)
{
    delayModule.setHostBPM (bpm);
}
