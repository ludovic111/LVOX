#include "GlossyKnob.h"
#include "../Helpers/ColourPalette.h"

GlossyKnob::GlossyKnob (const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 16);
    slider.setTextBoxIsEditable (true);
    addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, FrutigerColours::textDim);
    label.setFont (juce::Font (11.0f));
    addAndMakeVisible (label);
}

void GlossyKnob::attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID)
{
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, paramID, slider);
}

void GlossyKnob::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds (bounds.removeFromBottom (16));
    slider.setBounds (bounds);
}
