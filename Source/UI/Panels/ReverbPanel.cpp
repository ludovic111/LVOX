#include "ReverbPanel.h"

ReverbPanel::ReverbPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Reverb", ParamIDs::revBypass)
{
    addAndMakeVisible (sizeKnob);
    addAndMakeVisible (dampingKnob);
    addAndMakeVisible (predelayKnob);
    addAndMakeVisible (lowCutKnob);
    addAndMakeVisible (highCutKnob);
    addAndMakeVisible (mixKnob);

    sizeKnob.attach     (apvts, ParamIDs::revSize);
    dampingKnob.attach  (apvts, ParamIDs::revDamping);
    predelayKnob.attach (apvts, ParamIDs::revPredelay);
    lowCutKnob.attach   (apvts, ParamIDs::revLowCut);
    highCutKnob.attach  (apvts, ParamIDs::revHighCut);
    mixKnob.attach      (apvts, ParamIDs::revMix);
}

void ReverbPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (sizeKnob)    .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (dampingKnob) .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (predelayKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (lowCutKnob)  .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (highCutKnob) .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (mixKnob)     .withWidth (80).withHeight (90));

    fb.performLayout (area);
}
