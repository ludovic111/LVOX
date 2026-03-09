#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class CompressorModule : public DSPModule
{
public:
    CompressorModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::compBypass)
    {
        thresholdParam = getParam (ParamIDs::compThreshold);
        ratioParam     = getParam (ParamIDs::compRatio);
        attackParam    = getParam (ParamIDs::compAttack);
        releaseParam   = getParam (ParamIDs::compRelease);
        kneeParam      = getParam (ParamIDs::compKnee);
        makeupParam    = getParam (ParamIDs::compMakeup);
        scFreqParam    = getParam (ParamIDs::compScFreq);
        scActiveParam  = getParam (ParamIDs::compScActive);
        mixParam       = getParam (ParamIDs::compMix);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

    float getGainReduction() const { return currentGainReductionDB; }

private:
    std::atomic<float>* thresholdParam = nullptr;
    std::atomic<float>* ratioParam     = nullptr;
    std::atomic<float>* attackParam    = nullptr;
    std::atomic<float>* releaseParam   = nullptr;
    std::atomic<float>* kneeParam      = nullptr;
    std::atomic<float>* makeupParam    = nullptr;
    std::atomic<float>* scFreqParam    = nullptr;
    std::atomic<float>* scActiveParam  = nullptr;
    std::atomic<float>* mixParam       = nullptr;

    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter sidechainHPF;

    float envelope = 0.0f;
    float currentGainReductionDB = 0.0f;
    double sampleRate = 44100.0;
};
