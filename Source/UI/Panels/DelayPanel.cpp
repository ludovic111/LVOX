#include "DelayPanel.h"

DelayPanel::DelayPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Delay", ParamIDs::dlyBypass)
{
    addAndMakeVisible (timeKnob);
    addAndMakeVisible (feedbackKnob);
    addAndMakeVisible (lowCutKnob);
    addAndMakeVisible (highCutKnob);
    addAndMakeVisible (mixKnob);
    addAndMakeVisible (noteBox);
    addAndMakeVisible (syncToggle);

    timeKnob.attach     (apvts, ParamIDs::dlyTime);
    feedbackKnob.attach (apvts, ParamIDs::dlyFeedback);
    lowCutKnob.attach   (apvts, ParamIDs::dlyLowCut);
    highCutKnob.attach  (apvts, ParamIDs::dlyHighCut);
    mixKnob.attach      (apvts, ParamIDs::dlyMix);

    noteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::dlyNote, noteBox);
    syncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>   (apvts, ParamIDs::dlySync, syncToggle);
}

void DelayPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.flexWrap       = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (timeKnob)     .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (feedbackKnob) .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (lowCutKnob)   .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (highCutKnob)  .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (mixKnob)      .withWidth (80).withHeight (90));
    fb.items.add (juce::FlexItem (noteBox)      .withWidth (100).withHeight (24));
    fb.items.add (juce::FlexItem (syncToggle)   .withWidth (80).withHeight (24));

    fb.performLayout (area);
}
