#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Helpers/ColourPalette.h"

class MeterComponent : public juce::Component, public juce::Timer
{
public:
    MeterComponent();

    void setLevel (float newLevelL, float newLevelR);
    void setLevel (float newLevel);
    void setLabel (const juce::String& text) { label = text; }
    void paint (juce::Graphics& g) override;
    void timerCallback() override;

private:
    float currentLevelL = 0.0f;
    float currentLevelR = 0.0f;
    float displayLevelL = 0.0f;
    float displayLevelR = 0.0f;
    float peakLevelL = 0.0f;
    float peakLevelR = 0.0f;
    int peakHoldCounterL = 0;
    int peakHoldCounterR = 0;
    juce::String label;

    static constexpr int peakHoldFrames = 45; // ~1.5s at 30Hz
};
