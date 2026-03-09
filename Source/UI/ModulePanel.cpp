#include "ModulePanel.h"

ModulePanel::ModulePanel (juce::AudioProcessorValueTreeState& apvts, const juce::String& title,
                           const juce::String& bypassParamID,
                           juce::Colour categoryColour, int moduleIndex)
    : apvts (apvts), title (title), bypassButton ("Bypass"),
      categoryColour (categoryColour), moduleIndex (moduleIndex)
{
    bypassButton.attach (apvts, bypassParamID);
    addAndMakeVisible (bypassButton);
}

void ModulePanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    GradientUtils::drawEnhancedGlossyPanel (g, bounds, categoryColour, 8.0f);

    // Index badge (20x20 rounded rect with number)
    if (moduleIndex > 0)
    {
        auto badgeBounds = juce::Rectangle<float> (10.0f, 5.0f, 20.0f, 20.0f);
        g.setColour (categoryColour.withAlpha (0.6f));
        g.fillRoundedRectangle (badgeBounds, 4.0f);
        g.setColour (FrutigerColours::textBright);
        g.setFont (11.0f);
        g.drawText (juce::String (moduleIndex), badgeBounds, juce::Justification::centred);
    }

    // Title
    g.setColour (FrutigerColours::textBright);
    g.setFont (14.0f);
    auto titleX = moduleIndex > 0 ? 34.0f : 10.0f;
    g.drawText (title, juce::Rectangle<float> (titleX, 4.0f, bounds.getWidth() - titleX - 90.0f, 24.0f),
                juce::Justification::centredLeft);
}

void ModulePanel::resized()
{
    auto bounds = getLocalBounds();
    auto header = bounds.removeFromTop (28);
    bypassButton.setBounds (header.removeFromRight (80).reduced (2));
    bounds.reduce (6, 4);
    layoutControls (bounds);
}
