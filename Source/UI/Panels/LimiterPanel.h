#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class LimiterPanel : public ModulePanel
{
public:
    LimiterPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob ceilingKnob { "Ceiling" };
    GlossyKnob releaseKnob { "Release" };
};
