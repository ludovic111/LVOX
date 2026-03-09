#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class GlossyKnob : public juce::Component, private juce::Timer
{
public:
    GlossyKnob (const juce::String& labelText);

    void attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID);
    void resized() override;

    juce::Slider& getSlider() { return slider; }

private:
    juce::Slider slider;
    juce::Label label;
    juce::Label valueLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    void timerCallback() override;
};
