#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class DeEsserPanel : public ModulePanel
{
public:
    DeEsserPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob frequencyKnob { "Frequency" };
    GlossyKnob thresholdKnob { "Threshold" };
    GlossyKnob reductionKnob { "Reduction" };
    GlossyKnob attackKnob    { "Attack" };
    GlossyKnob releaseKnob   { "Release" };

    juce::ToggleButton listenToggle { "Listen" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> listenAttachment;
};
