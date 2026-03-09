#include "SimpleModePanel.h"
#include "../Parameters.h"

SimpleModePanel::SimpleModePanel (juce::AudioProcessorValueTreeState& apvts)
    : apvts (apvts)
{
    warmthKnob.attach (apvts, ParamIDs::macroWarmth);
    presenceKnob.attach (apvts, ParamIDs::macroPresence);
    compressionKnob.attach (apvts, ParamIDs::macroCompression);
    spaceKnob.attach (apvts, ParamIDs::macroSpace);

    addAndMakeVisible (warmthKnob);
    addAndMakeVisible (presenceKnob);
    addAndMakeVisible (compressionKnob);
    addAndMakeVisible (spaceKnob);
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);

    startTimerHz (15);
}

void SimpleModePanel::setParamNormalized (const juce::String& id, float naturalValue)
{
    if (auto* param = apvts.getParameter (id))
        param->setValueNotifyingHost (param->convertTo0to1 (naturalValue));
}

void SimpleModePanel::applyMacros()
{
    float warmth     = apvts.getRawParameterValue (ParamIDs::macroWarmth)->load();
    float presence   = apvts.getRawParameterValue (ParamIDs::macroPresence)->load();
    float compression = apvts.getRawParameterValue (ParamIDs::macroCompression)->load();
    float space      = apvts.getRawParameterValue (ParamIDs::macroSpace)->load();

    // Warmth mapping
    setParamNormalized (ParamIDs::satDrive,    warmth * 0.4f);
    setParamNormalized (ParamIDs::satMix,      warmth * 0.6f);
    setParamNormalized (ParamIDs::eqBand1Gain, warmth * 0.04f);
    setParamNormalized (ParamIDs::eqBand4Gain, -warmth * 0.03f);

    // Presence mapping
    setParamNormalized (ParamIDs::eqBand3Gain, presence * 0.06f);
    setParamNormalized (ParamIDs::eqBand3Q,    2.0f - presence * 0.015f);
    setParamNormalized (ParamIDs::satTone,     presence * 0.5f);

    // Compression mapping
    setParamNormalized (ParamIDs::compThreshold, -6.0f - compression * 0.3f);
    setParamNormalized (ParamIDs::compRatio,     1.5f + compression * 0.065f);
    setParamNormalized (ParamIDs::compAttack,    20.0f - compression * 0.15f);
    setParamNormalized (ParamIDs::compRelease,   150.0f - compression);
    setParamNormalized (ParamIDs::compMakeup,    compression * 0.12f);

    // Space mapping
    setParamNormalized (ParamIDs::revMix,      space * 0.35f);
    setParamNormalized (ParamIDs::revSize,     20.0f + space * 0.6f);
    setParamNormalized (ParamIDs::revPredelay, 10.0f + space * 0.5f);
    setParamNormalized (ParamIDs::dlyMix,      space * 0.2f);
    setParamNormalized (ParamIDs::dlyFeedback, 15.0f + space * 0.3f);
    setParamNormalized (ParamIDs::dlyTime,     180.0f + space * 1.7f);
}

void SimpleModePanel::timerCallback()
{
    applyMacros();
}

void SimpleModePanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    GradientUtils::drawGlossyPanel (g, bounds, 12.0f);

    // Title
    g.setColour (FrutigerColours::textBright);
    g.setFont (20.0f);
    g.drawText ("LVOX", bounds.removeFromTop (40), juce::Justification::centred);
}

void SimpleModePanel::resized()
{
    auto bounds = getLocalBounds().reduced (16);
    bounds.removeFromTop (40); // title space

    // Meters on sides
    inputMeter.setBounds (bounds.removeFromLeft (16));
    bounds.removeFromLeft (12);
    outputMeter.setBounds (bounds.removeFromRight (16));
    bounds.removeFromRight (12);

    // 4 macro knobs centered
    auto knobWidth = bounds.getWidth() / 4;
    auto knobArea = bounds.reduced (0, 20);

    warmthKnob.setBounds      (knobArea.removeFromLeft (knobWidth));
    presenceKnob.setBounds    (knobArea.removeFromLeft (knobWidth));
    compressionKnob.setBounds (knobArea.removeFromLeft (knobWidth));
    spaceKnob.setBounds       (knobArea);
}
