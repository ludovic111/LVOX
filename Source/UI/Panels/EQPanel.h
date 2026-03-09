#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class EQPanel : public ModulePanel
{
public:
    EQPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    struct BandControls
    {
        GlossyKnob freqKnob;
        GlossyKnob gainKnob;
        GlossyKnob qKnob;
        juce::ComboBox typeBox;
        juce::ToggleButton activeToggle { "On" };

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   activeAttachment;

        BandControls (const juce::String& bandName)
            : freqKnob ("Freq " + bandName),
              gainKnob ("Gain " + bandName),
              qKnob    ("Q " + bandName)
        {}
    };

    juce::OwnedArray<BandControls> bands;
};
