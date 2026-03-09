#include "MacroKnob.h"
#include "../Helpers/ColourPalette.h"

MacroKnob::MacroKnob (const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setTextBoxIsEditable (true);
    addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, FrutigerColours::textBright);
    label.setFont (juce::Font (16.0f));
    addAndMakeVisible (label);
}

void MacroKnob::attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID)
{
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, paramID, slider);
}

void MacroKnob::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds (bounds.removeFromBottom (24));
    slider.setBounds (bounds);
}
