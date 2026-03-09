#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Helpers/ColourPalette.h"

class MeterComponent : public juce::Component, public juce::Timer
{
public:
    MeterComponent();

    void setLevel (float newLevel);
    void paint (juce::Graphics& g) override;
    void timerCallback() override;

private:
    float currentLevel = 0.0f;
    float displayLevel = 0.0f;
};
