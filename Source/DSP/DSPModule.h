#pragma once
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "MicCorrection.h"

class DSPModule
{
public:
    DSPModule (juce::AudioProcessorValueTreeState& apvts, const juce::String& bypassParamID)
        : apvts (apvts)
    {
        if (bypassParamID.isNotEmpty())
            bypassParam = apvts.getRawParameterValue (bypassParamID);
    }

    virtual ~DSPModule() = default;

    virtual void prepare (const juce::dsp::ProcessSpec& spec) = 0;
    virtual void process (juce::dsp::AudioBlock<float>& block) = 0;
    virtual void reset() = 0;

    bool isBypassed() const
    {
        return bypassParam != nullptr && bypassParam->load() > 0.5f;
    }

    void setMicCorrection (const MicCorrection* mc) { micCorrection = mc; }

    float getOutputLevel() const { return outputLevel.load(); }

    void updateOutputLevel (const juce::dsp::AudioBlock<float>& block)
    {
        float peak = 0.0f;
        for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
            for (size_t i = 0; i < block.getNumSamples(); ++i)
                peak = std::max (peak, std::abs (block.getSample ((int) ch, (int) i)));
        outputLevel.store (peak);
    }

protected:
    std::atomic<float> outputLevel { 0.0f };
    const MicCorrection* micCorrection = nullptr;
    juce::AudioProcessorValueTreeState& apvts;
    std::atomic<float>* bypassParam = nullptr;

    std::atomic<float>* getParam (const juce::String& id)
    {
        return apvts.getRawParameterValue (id);
    }
};
