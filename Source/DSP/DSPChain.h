#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "DSPModule.h"
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

private:
    juce::AudioProcessorValueTreeState& apvts;

    std::atomic<float>* inputGainParam  = nullptr;
    std::atomic<float>* outputGainParam = nullptr;

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
};
