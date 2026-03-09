#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class HighPassPanel : public ModulePanel
{
public:
    HighPassPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob frequencyKnob { "Frequency" };
    juce::ComboBox slopeBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> slopeAttachment;
};
