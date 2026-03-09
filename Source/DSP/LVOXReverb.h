#pragma once
#include "DSPModule.h"
#include "../Parameters.h"
#include <array>

// Dattorro plate reverb implementation
class DattorroPlate
{
public:
    void prepare (double sampleRate);
    void process (float inputL, float inputR, float& outL, float& outR);
    void reset();

    void setDecay (float d)   { decay = juce::jlimit (0.0f, 0.999f, d); }
    void setDamping (float d) { damping = juce::jlimit (0.0f, 1.0f, d); }
    void setSize (float s)    { sizeScale = juce::jlimit (0.3f, 2.0f, s); }

private:
    double sr = 44100.0;
    float decay = 0.7f;
    float damping = 0.5f;
    float sizeScale = 1.0f;

    // Allpass filter
    struct Allpass
    {
        std::vector<float> buffer;
        int writeIdx = 0;
        int size = 1;
        float feedback = 0.5f;

        void init (int numSamples, float fb = 0.5f)
        {
            size = std::max (1, numSamples);
            buffer.assign (size, 0.0f);
            writeIdx = 0;
            feedback = fb;
        }

        float process (float input)
        {
            float delayed = buffer[writeIdx];
            float v = input + delayed * feedback;
            buffer[writeIdx] = v;
            writeIdx = (writeIdx + 1) % size;
            return delayed - v * feedback;
        }

        void clear() { std::fill (buffer.begin(), buffer.end(), 0.0f); writeIdx = 0; }
    };

    // Simple delay line
    struct DelayBuf
    {
        std::vector<float> buffer;
        int writeIdx = 0;
        int size = 1;

        void init (int numSamples)
        {
            size = std::max (1, numSamples);
            buffer.assign (size, 0.0f);
            writeIdx = 0;
        }

        void push (float val)
        {
            buffer[writeIdx] = val;
            writeIdx = (writeIdx + 1) % size;
        }

        float read (int offset) const
        {
            int idx = (writeIdx - offset + size * 2) % size;
            return buffer[idx];
        }

        float tap() const { return buffer[writeIdx]; }
        void clear() { std::fill (buffer.begin(), buffer.end(), 0.0f); writeIdx = 0; }
    };

    // One-pole lowpass for damping
    struct OnePole
    {
        float z1 = 0.0f;
        float coeff = 0.5f;

        float process (float input)
        {
            z1 = input * (1.0f - coeff) + z1 * coeff;
            return z1;
        }

        void clear() { z1 = 0.0f; }
    };

    // Input diffusion (4 allpasses)
    Allpass inputAP[4];

    // Tank: 2 channels, each with allpass + delay + damping + delay
    Allpass tankAP[2];
    DelayBuf tankDelay1[2];
    DelayBuf tankDelay2[2];
    OnePole tankDamp[2];
};

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

    DattorroPlate plate;
    juce::dsp::DelayLine<float> predelayLine { 44100 };

    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter lowCutFilter;
    IIRFilter highCutFilter;

    double sampleRate = 44100.0;
};
