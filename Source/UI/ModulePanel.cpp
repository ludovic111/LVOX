#include "ModulePanel.h"

ModulePanel::ModulePanel (juce::AudioProcessorValueTreeState& apvts, const juce::String& title, const juce::String& bypassParamID)
    : apvts (apvts), title (title), bypassButton ("Bypass")
{
    bypassButton.attach (apvts, bypassParamID);
    addAndMakeVisible (bypassButton);
}

void ModulePanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    GradientUtils::drawGlossyPanel (g, bounds, 8.0f);

    // Title
    g.setColour (FrutigerColours::textBright);
    g.setFont (14.0f);
    g.drawText (title, bounds.reduced (10, 4).removeFromTop (24), juce::Justification::centredLeft);
}

void ModulePanel::resized()
{
    auto bounds = getLocalBounds();
    auto header = bounds.removeFromTop (28);
    bypassButton.setBounds (header.removeFromRight (80).reduced (2));
    bounds.reduce (6, 4);
    layoutControls (bounds);
}
