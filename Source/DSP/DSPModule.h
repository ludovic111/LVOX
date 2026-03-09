#pragma once
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

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

protected:
    juce::AudioProcessorValueTreeState& apvts;
    std::atomic<float>* bypassParam = nullptr;

    std::atomic<float>* getParam (const juce::String& id)
    {
        return apvts.getRawParameterValue (id);
    }
};
