#pragma once
#include "../ModulePanel.h"
#include "../../Parameters.h"

class GainReductionMeter : public juce::Component
{
public:
    void setGainReduction (float grDB)
    {
        gainReductionDB = grDB;
        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (1.0f);

        // Background
        g.setColour (juce::Colour (0xff1a1a2e));
        g.fillRoundedRectangle (bounds, 3.0f);

        // GR bar (grows downward from top)
        float maxGR = 24.0f;
        float grNorm = juce::jlimit (0.0f, 1.0f, gainReductionDB / maxGR);
        float barH = bounds.getHeight() * grNorm;

        if (barH > 0.5f)
        {
            auto barBounds = bounds.removeFromTop (barH);
            g.setColour (juce::Colour (0xffff8800)); // amber
            g.fillRoundedRectangle (barBounds, 2.0f);
        }

        // dB text
        g.setColour (juce::Colours::white.withAlpha (0.8f));
        g.setFont (9.0f);
        juce::String text = (gainReductionDB > 0.1f) ? juce::String (-gainReductionDB, 1) : "0";
        g.drawText (text, getLocalBounds(), juce::Justification::centredBottom);
    }

private:
    float gainReductionDB = 0.0f;
};

class CompressorPanel : public ModulePanel
{
public:
    CompressorPanel (juce::AudioProcessorValueTreeState& apvts);

    GainReductionMeter& getGRMeter() { return grMeter; }

protected:
    void layoutControls (juce::Rectangle<int> area) override;

private:
    GlossyKnob thresholdKnob { "Threshold" };
    GlossyKnob ratioKnob     { "Ratio" };
    GlossyKnob attackKnob    { "Attack" };
    GlossyKnob releaseKnob   { "Release" };
    GlossyKnob kneeKnob      { "Knee" };
    GlossyKnob makeupKnob    { "Makeup" };
    GlossyKnob scFreqKnob    { "SC Freq" };
    GlossyKnob mixKnob       { "Mix" };

    juce::ToggleButton scActiveToggle { "SC Active" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scActiveAttachment;

    GainReductionMeter grMeter;
};
