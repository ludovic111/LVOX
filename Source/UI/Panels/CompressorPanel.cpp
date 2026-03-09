#include "CompressorPanel.h"

CompressorPanel::CompressorPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Compressor", ParamIDs::compBypass, FrutigerColours::catDynamics, 5)
{
    addAndMakeVisible (thresholdKnob);
    addAndMakeVisible (ratioKnob);
    addAndMakeVisible (attackKnob);
    addAndMakeVisible (releaseKnob);
    addAndMakeVisible (kneeKnob);
    addAndMakeVisible (makeupKnob);
    addAndMakeVisible (scFreqKnob);
    addAndMakeVisible (mixKnob);
    addAndMakeVisible (scActiveToggle);
    addAndMakeVisible (grMeter);

    thresholdKnob.attach (apvts, ParamIDs::compThreshold);
    ratioKnob.attach     (apvts, ParamIDs::compRatio);
    attackKnob.attach    (apvts, ParamIDs::compAttack);
    releaseKnob.attach   (apvts, ParamIDs::compRelease);
    kneeKnob.attach      (apvts, ParamIDs::compKnee);
    makeupKnob.attach    (apvts, ParamIDs::compMakeup);
    scFreqKnob.attach    (apvts, ParamIDs::compScFreq);
    mixKnob.attach       (apvts, ParamIDs::compMix);

    scActiveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::compScActive, scActiveToggle);
}

void CompressorPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.flexWrap       = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (thresholdKnob) .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (ratioKnob)     .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (attackKnob)    .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (releaseKnob)   .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (kneeKnob)      .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (makeupKnob)    .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (scFreqKnob)    .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (mixKnob)       .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (scActiveToggle).withWidth (80).withHeight (24));
    fb.items.add (juce::FlexItem (grMeter)      .withWidth (20).withHeight (80));

    fb.performLayout (area);
}
