#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class SendBusPanel : public ModulePanel
{
public:
    SendBusPanel (juce::AudioProcessorValueTreeState& apvts);

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob revSendKnob { "Rev Send" };
    GlossyKnob dlySendKnob { "Dly Send" };
};
