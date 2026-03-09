#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class HighPassFilterModule : public DSPModule
{
public:
    HighPassFilterModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::hpfBypass)
    {
        frequencyParam = getParam (ParamIDs::hpfFrequency);
        slopeParam     = getParam (ParamIDs::hpfSlope);
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

private:
    void updateFilters();

    std::atomic<float>* frequencyParam = nullptr;
    std::atomic<float>* slopeParam     = nullptr;

    // Up to 4 cascaded 2nd-order sections for 48 dB/oct
    using Filter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    Filter filters[4];
    int activeFilterCount = 2;
    double sampleRate = 44100.0;
    float lastFreq = 0.0f;
    float lastSlope = -1.0f;
};
