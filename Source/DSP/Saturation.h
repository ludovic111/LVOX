#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class SaturationModule : public DSPModule
{
public:
    SaturationModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::satBypass),
          oversampling (2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
    {
        driveParam  = getParam (ParamIDs::satDrive);
        typeParam   = getParam (ParamIDs::satType);
        toneParam   = getParam (ParamIDs::satTone);
        mixParam    = getParam (ParamIDs::satMix);
        outputParam = getParam (ParamIDs::satOutput);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

private:
    std::atomic<float>* driveParam  = nullptr;
    std::atomic<float>* typeParam   = nullptr;
    std::atomic<float>* toneParam   = nullptr;
    std::atomic<float>* mixParam    = nullptr;
    std::atomic<float>* outputParam = nullptr;

    juce::dsp::Oversampling<float> oversampling;

    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter toneFilter;
    double sampleRate = 44100.0;

    static float processTape (float x, float drive);
    static float processTube (float x, float drive);
    static float processSoftClip (float x, float drive);
    static float processHardClip (float x, float drive);
};
