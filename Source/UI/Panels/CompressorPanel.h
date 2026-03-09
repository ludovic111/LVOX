#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class CompressorPanel : public ModulePanel
{
public:
    CompressorPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob thresholdKnob { "Threshold" };
    GlossyKnob ratioKnob     { "Ratio" };
    GlossyKnob attackKnob    { "Attack" };
    GlossyKnob releaseKnob   { "Release" };
    GlossyKnob kneeKnob      { "Knee" };
    GlossyKnob makeupKnob    { "Makeup" };
    GlossyKnob scFreqKnob    { "SC Freq" };
    GlossyKnob mixKnob       { "Mix" };

    juce::ToggleButton scActiveToggle { "SC Active" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scActiveAttachment;
};
