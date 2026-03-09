#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class ReverbPanel : public ModulePanel
{
public:
    ReverbPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob sizeKnob     { "Size" };
    GlossyKnob dampingKnob  { "Damping" };
    GlossyKnob predelayKnob { "Pre-Delay" };
    GlossyKnob lowCutKnob   { "Low Cut" };
    GlossyKnob highCutKnob  { "High Cut" };
    GlossyKnob mixKnob      { "Mix" };
};
