#include "AdvancedModePanel.h"

AdvancedModePanel::AdvancedModePanel (juce::AudioProcessorValueTreeState& apvts)
    : gatePanel (apvts),
      hpfPanel (apvts),
      deesserPanel (apvts),
      eqPanel (apvts),
      compPanel (apvts),
      satPanel (apvts),
      revPanel (apvts),
      dlyPanel (apvts),
      sendPanel (apvts),
      limPanel (apvts)
{
    contentContainer.addAndMakeVisible (gatePanel);
    contentContainer.addAndMakeVisible (hpfPanel);
    contentContainer.addAndMakeVisible (deesserPanel);
    contentContainer.addAndMakeVisible (eqPanel);
    contentContainer.addAndMakeVisible (compPanel);
    contentContainer.addAndMakeVisible (satPanel);
    contentContainer.addAndMakeVisible (sendPanel);
    contentContainer.addAndMakeVisible (revPanel);
    contentContainer.addAndMakeVisible (dlyPanel);
    contentContainer.addAndMakeVisible (limPanel);

    viewport.setViewedComponent (&contentContainer, false);
    viewport.setScrollBarsShown (true, false);
    addAndMakeVisible (viewport);

    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
}

void AdvancedModePanel::paint (juce::Graphics& g)
{
    g.fillAll (FrutigerColours::backgroundDark);

    g.setColour (FrutigerColours::textBright);
    g.setFont (20.0f);
    g.drawText ("LVOX - Advanced", getLocalBounds().removeFromTop (36), juce::Justification::centred);
}

void AdvancedModePanel::resized()
{
    auto bounds = getLocalBounds();
    auto header = bounds.removeFromTop (36);

    // Meters on sides
    inputMeter.setBounds (bounds.removeFromLeft (16));
    bounds.removeFromLeft (4);
    outputMeter.setBounds (bounds.removeFromRight (16));
    bounds.removeFromRight (4);

    viewport.setBounds (bounds);

    // Layout modules vertically in content container
    int panelWidth = bounds.getWidth() - 20; // account for scrollbar
    int y = 4;
    int moduleHeight = 120;
    int eqHeight = 160;

    auto placeModule = [&] (juce::Component& comp, int height)
    {
        comp.setBounds (4, y, panelWidth, height);
        y += height + 6;
    };

    placeModule (gatePanel, moduleHeight);
    placeModule (hpfPanel, 80);
    placeModule (deesserPanel, moduleHeight);
    placeModule (eqPanel, eqHeight);
    placeModule (compPanel, moduleHeight);
    placeModule (satPanel, moduleHeight);
    placeModule (sendPanel, 80);
    placeModule (revPanel, moduleHeight);
    placeModule (dlyPanel, moduleHeight);
    placeModule (limPanel, 80);

    contentContainer.setSize (panelWidth + 8, y);
}
