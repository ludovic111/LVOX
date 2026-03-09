#include "GlowButton.h"

GlowButton::GlowButton (const juce::String& text)
{
    button.setButtonText (text);
    addAndMakeVisible (button);
}

void GlowButton::attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID)
{
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, paramID, button);
}

void GlowButton::resized()
{
    button.setBounds (getLocalBounds());
}
