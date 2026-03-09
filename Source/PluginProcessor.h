#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Parameters.h"
#include "DSP/DSPChain.h"
#include "Preset/PresetManager.h"

class LVOXAudioProcessor : public juce::AudioProcessor
{
public:
    LVOXAudioProcessor();
    ~LVOXAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override  { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;
    PresetManager presetManager;

    // A/B comparison
    bool isSlotA() const { return currentSlotA; }
    void switchSlot()
    {
        auto& storeSlot = currentSlotA ? slotA : slotB;
        storeSlot = apvts.copyState();
        currentSlotA = !currentSlotA;
        auto& loadSlot = currentSlotA ? slotA : slotB;
        if (loadSlot.isValid())
            apvts.replaceState (loadSlot);
    }
    void copyAtoB()
    {
        slotA = apvts.copyState();
        slotB = slotA.createCopy();
    }

    float getInputLevel() const  { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }

    float getCompressorGainReduction() const { return compGainReduction.load(); }
    float getModuleOutputLevel (int index) const { return dspChain.getModuleOutputLevel (index); }

    float getInputLevelL() const  { return inputLevelL.load(); }
    float getInputLevelR() const  { return inputLevelR.load(); }
    float getOutputLevelL() const { return outputLevelL.load(); }
    float getOutputLevelR() const { return outputLevelR.load(); }

private:
    DSPChain dspChain;

    bool currentSlotA = true;
    juce::ValueTree slotA;
    juce::ValueTree slotB;

    std::atomic<float> compGainReduction { 0.0f };
    std::atomic<float> inputLevel  { 0.0f };
    std::atomic<float> outputLevel { 0.0f };
    std::atomic<float> inputLevelL  { 0.0f };
    std::atomic<float> inputLevelR  { 0.0f };
    std::atomic<float> outputLevelL { 0.0f };
    std::atomic<float> outputLevelR { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LVOXAudioProcessor)
};
