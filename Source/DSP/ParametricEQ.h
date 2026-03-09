#pragma once
#include "DSPModule.h"
#include "../Parameters.h"

class ParametricEQModule : public DSPModule
{
public:
    ParametricEQModule (juce::AudioProcessorValueTreeState& apvts)
        : DSPModule (apvts, ParamIDs::eqBypass)
    {
        const char* freqIDs[]   = { ParamIDs::eqBand1Freq, ParamIDs::eqBand2Freq, ParamIDs::eqBand3Freq, ParamIDs::eqBand4Freq };
        const char* gainIDs[]   = { ParamIDs::eqBand1Gain, ParamIDs::eqBand2Gain, ParamIDs::eqBand3Gain, ParamIDs::eqBand4Gain };
        const char* qIDs[]      = { ParamIDs::eqBand1Q,    ParamIDs::eqBand2Q,    ParamIDs::eqBand3Q,    ParamIDs::eqBand4Q    };
        const char* typeIDs[]   = { ParamIDs::eqBand1Type,  ParamIDs::eqBand2Type,  ParamIDs::eqBand3Type,  ParamIDs::eqBand4Type  };
        const char* activeIDs[] = { ParamIDs::eqBand1Active, ParamIDs::eqBand2Active, ParamIDs::eqBand3Active, ParamIDs::eqBand4Active };

        for (int i = 0; i < 4; ++i)
        {
            bands[i].freqParam   = getParam (freqIDs[i]);
            bands[i].gainParam   = getParam (gainIDs[i]);
            bands[i].qParam      = getParam (qIDs[i]);
            bands[i].typeParam   = getParam (typeIDs[i]);
            bands[i].activeParam = getParam (activeIDs[i]);
            bands[i].index       = i;
        }
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (juce::dsp::AudioBlock<float>& block) override;
    void reset() override;

private:
    struct Band
    {
        std::atomic<float>* freqParam   = nullptr;
        std::atomic<float>* gainParam   = nullptr;
        std::atomic<float>* qParam      = nullptr;
        std::atomic<float>* typeParam   = nullptr;
        std::atomic<float>* activeParam = nullptr;
        int index = 0;

        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;
    };

    Band bands[4];
    double sampleRate = 44100.0;
    void updateBand (Band& band);
};
