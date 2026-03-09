#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class SaturationPanel : public ModulePanel
{
public:
    SaturationPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob driveKnob  { "Drive" };
    GlossyKnob toneKnob   { "Tone" };
    GlossyKnob mixKnob    { "Mix" };
    GlossyKnob outputKnob { "Output" };

    juce::ComboBox typeBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
};
