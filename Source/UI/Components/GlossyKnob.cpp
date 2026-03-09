#include "GlossyKnob.h"
#include "../Helpers/ColourPalette.h"

GlossyKnob::GlossyKnob (const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, FrutigerColours::textDim);
    label.setFont (juce::Font (11.0f));
    addAndMakeVisible (label);

    valueLabel.setJustificationType (juce::Justification::centred);
    valueLabel.setColour (juce::Label::textColourId, FrutigerColours::textBright);
    valueLabel.setFont (juce::Font (10.0f));
    valueLabel.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (valueLabel);

    slider.onDragStart = [this]() { valueLabel.setAlpha (1.0f); stopTimer(); };
    slider.onDragEnd   = [this]() { startTimer (1000); }; // hide after 1s
    slider.onValueChange = [this]()
    {
        valueLabel.setText (slider.getTextFromValue (slider.getValue()), juce::dontSendNotification);
    };
}

void GlossyKnob::attach (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID)
{
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, paramID, slider);
    // Set initial value text
    valueLabel.setText (slider.getTextFromValue (slider.getValue()), juce::dontSendNotification);
    valueLabel.setAlpha (0.0f); // hidden initially
}

void GlossyKnob::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds (bounds.removeFromBottom (16));
    auto valueBounds = bounds.removeFromBottom (14);
    valueLabel.setBounds (valueBounds);
    slider.setBounds (bounds);
}

void GlossyKnob::timerCallback()
{
    // After drag ends and 1s passes, hide the value
    valueLabel.setAlpha (0.0f);
    stopTimer();
}
