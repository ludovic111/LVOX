#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "DSPModule.h"
#include "MicCorrection.h"
#include "NoiseGate.h"
#include "HighPassFilter.h"
#include "DeEsser.h"
#include "ParametricEQ.h"
#include "Compressor.h"
#include "Saturation.h"
#include "LVOXReverb.h"
#include "Delay.h"
#include "Limiter.h"

class DSPChain
{
public:
    DSPChain (juce::AudioProcessorValueTreeState& apvts);

    void prepare (const juce::dsp::ProcessSpec& spec);
    void process (juce::AudioBuffer<float>& buffer);
    void reset();

    void setHostBPM (double bpm);

    CompressorModule& getCompressor() { return compressor; }
    LimiterModule& getLimiter() { return limiter; }
    float getModuleOutputLevel (int index) const
    {
        if (index >= 0 && index < 9)
            return modules[index]->getOutputLevel();
        return 0.0f;
    }

private:
    juce::AudioProcessorValueTreeState& apvts;

    std::atomic<float>* inputGainParam  = nullptr;
    std::atomic<float>* outputGainParam = nullptr;

    juce::SmoothedValue<float> smoothedInputGain  { 1.0f };
    juce::SmoothedValue<float> smoothedOutputGain { 1.0f };
    std::atomic<float>* sendModeParam   = nullptr;
    std::atomic<float>* sendRevLevelParam = nullptr;
    std::atomic<float>* sendDlyLevelParam = nullptr;

    NoiseGateModule       noiseGate;
    HighPassFilterModule  highPassFilter;
    DeEsserModule         deEsser;
    ParametricEQModule    parametricEQ;
    CompressorModule      compressor;
    SaturationModule      saturation;
    ReverbModule          reverbModule;
    DelayModule           delayModule;
    LimiterModule         limiter;

    DSPModule* modules[9];

    // Send buffers for parallel processing
    juce::AudioBuffer<float> sendRevBuffer;
    juce::AudioBuffer<float> sendDlyBuffer;

    // Mic correction
    std::atomic<float>* micSelectParam = nullptr;
    MicCorrection micCorrection;
    void updateMicCorrection();
};
