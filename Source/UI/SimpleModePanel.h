#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "Components/MacroKnob.h"
#include "Components/MeterComponent.h"
#include "Helpers/ColourPalette.h"
#include "Helpers/GradientUtils.h"

class SimpleModePanel : public juce::Component, public juce::Timer
{
public:
    SimpleModePanel (juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    void applyMacros();

    MeterComponent& getInputMeter()  { return inputMeter; }
    MeterComponent& getOutputMeter() { return outputMeter; }

private:
    juce::AudioProcessorValueTreeState& apvts;

    MacroKnob warmthKnob      { "Warmth" };
    MacroKnob presenceKnob    { "Presence" };
    MacroKnob compressionKnob { "Compression" };
    MacroKnob spaceKnob       { "Space" };

    MeterComponent inputMeter;
    MeterComponent outputMeter;

    void setParamNormalized (const juce::String& id, float naturalValue);
};
