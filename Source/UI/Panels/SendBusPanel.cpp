#include "SendBusPanel.h"

SendBusPanel::SendBusPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Send / Bus", ParamIDs::sendMode, FrutigerColours::catEffects, 0)
{
    addAndMakeVisible (revSendKnob);
    addAndMakeVisible (dlySendKnob);

    revSendKnob.attach (apvts, ParamIDs::sendRevLevel);
    dlySendKnob.attach (apvts, ParamIDs::sendDlyLevel);
}

void SendBusPanel::layoutControls (juce::Rectangle<int> area)
{
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignItems     = juce::FlexBox::AlignItems::center;

    fb.items.add (juce::FlexItem (revSendKnob).withWidth (100).withHeight (90));
    fb.items.add (juce::FlexItem (dlySendKnob).withWidth (100).withHeight (90));

    fb.performLayout (area);
}
