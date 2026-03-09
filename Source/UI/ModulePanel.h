#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "Components/GlossyKnob.h"
#include "Components/GlowButton.h"
#include "Helpers/ColourPalette.h"
#include "Helpers/GradientUtils.h"

class ModulePanel : public juce::Component
{
public:
    ModulePanel (juce::AudioProcessorValueTreeState& apvts, const juce::String& title, const juce::String& bypassParamID);

    void paint (juce::Graphics& g) override;
    void resized() override;

protected:
    juce::AudioProcessorValueTreeState& apvts;
    juce::String title;
    GlowButton bypassButton;

    virtual void layoutControls (juce::Rectangle<int> area) = 0;
};
