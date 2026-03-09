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
    ModulePanel (juce::AudioProcessorValueTreeState& apvts, const juce::String& title,
                 const juce::String& bypassParamID,
                 juce::Colour categoryColour = FrutigerColours::aquaDark,
                 int moduleIndex = 0);

    void setActivityLevel (float level) { activityLevel = level; repaint(); }

    void paint (juce::Graphics& g) override;
    void resized() override;

protected:
    juce::AudioProcessorValueTreeState& apvts;
    juce::String title;
    GlowButton bypassButton;
    juce::Colour categoryColour;
    int moduleIndex;

    float activityLevel = 0.0f;

    virtual void layoutControls (juce::Rectangle<int> area) = 0;
};
