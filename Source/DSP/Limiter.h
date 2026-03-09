#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class LimiterModule : public DSPModule
{
public:
    LimiterModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::limBypass)
    {
        ceilingParam = getParam (ParamIDs::limCeiling);
        releaseParam = getParam (ParamIDs::limRelease);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

    int getLatencySamples() const { return lookAheadSamples; }

private:
    std::atomic<float>* ceilingParam = nullptr;
    std::atomic<float>* releaseParam = nullptr;

    float gainReduction = 1.0f;
    double sampleRate = 44100.0;

    juce::dsp::DelayLine<float> lookAheadDelay { 512 };
    int lookAheadSamples = 0;
    static constexpr float lookAheadMs = 1.0f;
};
