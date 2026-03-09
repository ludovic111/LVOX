#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class DelayPanel : public ModulePanel
{
public:
    DelayPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob timeKnob     { "Time" };
    GlossyKnob feedbackKnob { "Feedback" };
    GlossyKnob lowCutKnob   { "Low Cut" };
    GlossyKnob highCutKnob  { "High Cut" };
    GlossyKnob mixKnob      { "Mix" };

    juce::ComboBox noteBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noteAttachment;

    juce::ToggleButton syncToggle { "Sync" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncAttachment;
};
