#include "LimiterPanel.h"

LimiterPanel::LimiterPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Limiter", ParamIDs::limBypass)
{
    addAndMakeVisible (ceilingKnob);
    addAndMakeVisible (releaseKnob);

    ceilingKnob.attach (apvts, ParamIDs::limCeiling);
    releaseKnob.attach (apvts, ParamIDs::limRelease);
}

void LimiterPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (ceilingKnob).withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (releaseKnob).withWidth (80).withHeight (90));

    fb.performLayout (area);
}
