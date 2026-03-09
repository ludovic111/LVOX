#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class MacroKnob : public juce::Component
{
public:
    MacroKnob (const juce::String& labelText);

    void attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID);
    void resized() override;

private:
    juce::Slider slider;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};
