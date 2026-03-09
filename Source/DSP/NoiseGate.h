#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class NoiseGateModule : public DSPModule
{
public:
    NoiseGateModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::gateBypass)
    {
        thresholdParam = getParam (ParamIDs::gateThreshold);
        ratioParam     = getParam (ParamIDs::gateRatio);
        attackParam    = getParam (ParamIDs::gateAttack);
        releaseParam   = getParam (ParamIDs::gateRelease);
        rangeParam     = getParam (ParamIDs::gateRange);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

private:
    std::atomic<float>* thresholdParam = nullptr;
    std::atomic<float>* ratioParam     = nullptr;
    std::atomic<float>* attackParam    = nullptr;
    std::atomic<float>* releaseParam   = nullptr;
    std::atomic<float>* rangeParam     = nullptr;

    float envelope = 0.0f;
    float currentGain = 1.0f;
    double sampleRate = 44100.0;
};
