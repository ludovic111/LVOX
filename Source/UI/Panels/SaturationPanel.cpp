#include "SaturationPanel.h"

SaturationPanel::SaturationPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Saturation", ParamIDs::satBypass)
{
    addAndMakeVisible (driveKnob);
    addAndMakeVisible (toneKnob);
    addAndMakeVisible (mixKnob);
    addAndMakeVisible (outputKnob);
    addAndMakeVisible (typeBox);

    driveKnob.attach  (apvts, ParamIDs::satDrive);
    toneKnob.attach   (apvts, ParamIDs::satTone);
    mixKnob.attach    (apvts, ParamIDs::satMix);
    outputKnob.attach (apvts, ParamIDs::satOutput);

    typeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::satType, typeBox);
}

void SaturationPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (driveKnob) .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (typeBox)   .withWidth (100).withHeight (24));
    fb.items.add (juce::FlexItem (toneKnob)  .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (mixKnob)   .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (outputKnob).withWidth (80).withHeight (90));

    fb.performLayout (area);
}
