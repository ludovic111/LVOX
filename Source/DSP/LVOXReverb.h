#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class ReverbModule : public DSPModule
{
public:
    ReverbModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::revBypass)
    {
        sizeParam     = getParam (ParamIDs::revSize);
        dampingParam  = getParam (ParamIDs::revDamping);
        predelayParam = getParam (ParamIDs::revPredelay);
        lowCutParam   = getParam (ParamIDs::revLowCut);
        highCutParam  = getParam (ParamIDs::revHighCut);
        mixParam      = getParam (ParamIDs::revMix);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

private:
    std::atomic<float>* sizeParam     = nullptr;
    std::atomic<float>* dampingParam  = nullptr;
    std::atomic<float>* predelayParam = nullptr;
    std::atomic<float>* lowCutParam   = nullptr;
    std::atomic<float>* highCutParam  = nullptr;
    std::atomic<float>* mixParam      = nullptr;

    juce::dsp::Reverb reverb;
    juce::dsp::DelayLine<float> predelayLine { 44100 };

    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter lowCutFilter;
    IIRFilter highCutFilter;

    double sampleRate = 44100.0;
};
