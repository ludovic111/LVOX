#include "DeEsserPanel.h"

DeEsserPanel::DeEsserPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "De-Esser", ParamIDs::deessBypass, FrutigerColours::catFilter, 3)
{
    addAndMakeVisible (frequencyKnob);
    addAndMakeVisible (thresholdKnob);
    addAndMakeVisible (reductionKnob);
    addAndMakeVisible (attackKnob);
    addAndMakeVisible (releaseKnob);
    addAndMakeVisible (listenToggle);

    frequencyKnob.attach (apvts, ParamIDs::deessFrequency);
    thresholdKnob.attach (apvts, ParamIDs::deessThreshold);
    reductionKnob.attach (apvts, ParamIDs::deessReduction);
    attackKnob.attach    (apvts, ParamIDs::deessAttack);
    releaseKnob.attach   (apvts, ParamIDs::deessRelease);
    listenAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::deessListen, listenToggle);
}

void DeEsserPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (frequencyKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (thresholdKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (reductionKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (attackKnob)   .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (releaseKnob)  .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (listenToggle) .withWidth (80).withHeight (24));

    fb.performLayout (area);
}
