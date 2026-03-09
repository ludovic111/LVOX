#include "HighPassPanel.h"

HighPassPanel::HighPassPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "High-Pass Filter", ParamIDs::hpfBypass, FrutigerColours::catFilter, 2)
{
    addAndMakeVisible (frequencyKnob);
    addAndMakeVisible (slopeBox);

    frequencyKnob.attach (apvts, ParamIDs::hpfFrequency);
    slopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::hpfSlope, slopeBox);
}

void HighPassPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (frequencyKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (slopeBox)     .withWidth (100).withHeight (24));

    fb.performLayout (area);
}
