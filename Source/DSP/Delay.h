#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class DelayModule : public DSPModule
{
public:
    DelayModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::dlyBypass)
    {
        timeParam     = getParam (ParamIDs::dlyTime);
        syncParam     = getParam (ParamIDs::dlySync);
        noteParam     = getParam (ParamIDs::dlyNote);
        feedbackParam = getParam (ParamIDs::dlyFeedback);
        lowCutParam   = getParam (ParamIDs::dlyLowCut);
        highCutParam  = getParam (ParamIDs::dlyHighCut);
        mixParam      = getParam (ParamIDs::dlyMix);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

    void setHostBPM (double bpm) { hostBPM = bpm; }

private:
    std::atomic<float>* timeParam     = nullptr;
    std::atomic<float>* syncParam     = nullptr;
    std::atomic<float>* noteParam     = nullptr;
    std::atomic<float>* feedbackParam = nullptr;
    std::atomic<float>* lowCutParam   = nullptr;
    std::atomic<float>* highCutParam  = nullptr;
    std::atomic<float>* mixParam      = nullptr;

    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine { 96000 };

    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter fbLowCut;
    IIRFilter fbHighCut;

    std::vector<float> feedbackBuffer;
    double sampleRate = 44100.0;
    double hostBPM = 120.0;

    float getDelayTimeMs() const;
};
