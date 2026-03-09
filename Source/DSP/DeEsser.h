#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class DeEsserModule : public DSPModule
{
public:
    DeEsserModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::deessBypass)
    {
        frequencyParam = getParam (ParamIDs::deessFrequency);
        thresholdParam = getParam (ParamIDs::deessThreshold);
        reductionParam = getParam (ParamIDs::deessReduction);
        attackParam    = getParam (ParamIDs::deessAttack);
        releaseParam   = getParam (ParamIDs::deessRelease);
        listenParam    = getParam (ParamIDs::deessListen);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

private:
    std::atomic<float>* frequencyParam = nullptr;
    std::atomic<float>* thresholdParam = nullptr;
    std::atomic<float>* reductionParam = nullptr;
    std::atomic<float>* attackParam    = nullptr;
    std::atomic<float>* releaseParam   = nullptr;
    std::atomic<float>* listenParam    = nullptr;

    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter bandpassFilter;
    float envelope = 0.0f;
    double sampleRate = 44100.0;
};
