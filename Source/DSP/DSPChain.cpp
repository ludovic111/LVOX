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
    inputGainParam    = apvts.getRawParameterValue (ParamIDs::globalInput);
    outputGainParam   = apvts.getRawParameterValue (ParamIDs::globalOutput);
    sendModeParam     = apvts.getRawParameterValue (ParamIDs::sendMode);
    sendRevLevelParam = apvts.getRawParameterValue (ParamIDs::sendRevLevel);
    sendDlyLevelParam = apvts.getRawParameterValue (ParamIDs::sendDlyLevel);

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

    sendRevBuffer.setSize ((int) spec.numChannels, (int) spec.maximumBlockSize);
    sendDlyBuffer.setSize ((int) spec.numChannels, (int) spec.maximumBlockSize);
}

void DSPChain::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);

    // Input gain
    float inputGain = juce::Decibels::decibelsToGain (inputGainParam->load());
    block.multiplyBy (inputGain);

    bool sendMode = sendModeParam->load() > 0.5f;

    if (!sendMode)
    {
        // Inline mode: process all modules in series (original behavior)
        for (auto* module : modules)
        {
            if (!module->isBypassed())
                module->process (block);
        }
    }
    else
    {
        // Send mode: process gate→HPF→deesser→EQ→comp→sat inline,
        // then reverb & delay in parallel sends, then limiter
        for (int i = 0; i < 6; ++i) // modules 0-5: gate through saturation
        {
            if (!modules[i]->isBypassed())
                modules[i]->process (block);
        }

        const auto numChannels = buffer.getNumChannels();
        const auto numSamples  = buffer.getNumSamples();

        float revSendLevel = sendRevLevelParam->load() / 100.0f;
        float dlySendLevel = sendDlyLevelParam->load() / 100.0f;

        // Create send copies
        sendRevBuffer.setSize (numChannels, numSamples, false, false, true);
        sendDlyBuffer.setSize (numChannels, numSamples, false, false, true);

        for (int ch = 0; ch < numChannels; ++ch)
        {
            sendRevBuffer.copyFrom (ch, 0, buffer, ch, 0, numSamples);
            sendDlyBuffer.copyFrom (ch, 0, buffer, ch, 0, numSamples);
        }

        // Scale send buffers by send levels
        sendRevBuffer.applyGain (revSendLevel);
        sendDlyBuffer.applyGain (dlySendLevel);

        // Process reverb on send buffer (100% wet — reverb uses its own mix param
        // but in send mode we process it fully and the send level controls amount)
        if (!reverbModule.isBypassed())
        {
            juce::dsp::AudioBlock<float> revBlock (sendRevBuffer);
            reverbModule.process (revBlock);
        }

        // Process delay on send buffer
        if (!delayModule.isBypassed())
        {
            juce::dsp::AudioBlock<float> dlyBlock (sendDlyBuffer);
            delayModule.process (dlyBlock);
        }

        // Sum send returns into dry signal
        for (int ch = 0; ch < numChannels; ++ch)
        {
            buffer.addFrom (ch, 0, sendRevBuffer, ch, 0, numSamples);
            buffer.addFrom (ch, 0, sendDlyBuffer, ch, 0, numSamples);
        }

        // Limiter
        if (!limiter.isBypassed())
            limiter.process (block);
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
