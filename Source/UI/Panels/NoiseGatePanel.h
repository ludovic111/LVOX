#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class NoiseGatePanel : public ModulePanel
{
public:
    NoiseGatePanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob thresholdKnob { "Threshold" };
    GlossyKnob ratioKnob     { "Ratio" };
    GlossyKnob attackKnob    { "Attack" };
    GlossyKnob releaseKnob   { "Release" };
    GlossyKnob rangeKnob     { "Range" };
};
