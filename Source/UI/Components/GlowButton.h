#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class GlowButton : public juce::Component
{
public:
    GlowButton (const juce::String& text);

    void attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID);
    void resized() override;

    juce::ToggleButton& getButton() { return button; }

private:
    juce::ToggleButton button;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};
