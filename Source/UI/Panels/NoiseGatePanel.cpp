#include "NoiseGatePanel.h"

NoiseGatePanel::NoiseGatePanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Noise Gate", ParamIDs::gateBypass, FrutigerColours::catDynamics, 1)
{
    addAndMakeVisible (thresholdKnob);
    addAndMakeVisible (ratioKnob);
    addAndMakeVisible (attackKnob);
    addAndMakeVisible (releaseKnob);
    addAndMakeVisible (rangeKnob);

    thresholdKnob.attach (apvts, ParamIDs::gateThreshold);
    ratioKnob.attach     (apvts, ParamIDs::gateRatio);
    attackKnob.attach    (apvts, ParamIDs::gateAttack);
    releaseKnob.attach   (apvts, ParamIDs::gateRelease);
    rangeKnob.attach     (apvts, ParamIDs::gateRange);
}

void NoiseGatePanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (thresholdKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (ratioKnob)    .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (attackKnob)   .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (releaseKnob)  .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (rangeKnob)    .withWidth (80).withHeight (90));

    fb.performLayout (area);
}
