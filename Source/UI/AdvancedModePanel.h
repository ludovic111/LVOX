#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "Panels/NoiseGatePanel.h"
#include "Panels/HighPassPanel.h"
#include "Panels/DeEsserPanel.h"
#include "Panels/EQPanel.h"
#include "Panels/CompressorPanel.h"
#include "Panels/SaturationPanel.h"
#include "Panels/ReverbPanel.h"
#include "Panels/DelayPanel.h"
#include "Panels/LimiterPanel.h"
#include "Components/MeterComponent.h"
#include "Helpers/ColourPalette.h"
#include "Helpers/GradientUtils.h"

class AdvancedModePanel : public juce::Component
{
public:
    AdvancedModePanel (juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics& g) override;
    void resized() override;

    MeterComponent& getInputMeter()  { return inputMeter; }
    MeterComponent& getOutputMeter() { return outputMeter; }

private:
    NoiseGatePanel   gatePanel;
    HighPassPanel    hpfPanel;
    DeEsserPanel     deesserPanel;
    EQPanel          eqPanel;
    CompressorPanel  compPanel;
    SaturationPanel  satPanel;
    ReverbPanel      revPanel;
    DelayPanel       dlyPanel;
    LimiterPanel     limPanel;

    MeterComponent inputMeter;
    MeterComponent outputMeter;

    juce::Viewport viewport;
    juce::Component contentContainer;
};
