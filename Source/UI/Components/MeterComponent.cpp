#include "MeterComponent.h"

MeterComponent::MeterComponent()
{
    startTimerHz (30);
}

void MeterComponent::setLevel (float newLevel)
{
    currentLevel = newLevel;
}

void MeterComponent::timerCallback()
{
    // Smooth falloff
    float target = currentLevel;
    if (target > displayLevel)
        displayLevel = target;
    else
        displayLevel = displayLevel * 0.9f + target * 0.1f;

    repaint();
}

void MeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour (FrutigerColours::knobTrack);
    g.fillRoundedRectangle (bounds, 3.0f);

    // Level bar
    auto levelHeight = bounds.getHeight() * displayLevel;
    auto levelBounds = bounds.removeFromBottom (levelHeight);

    // Green to yellow to red gradient
    juce::Colour barColour;
    if (displayLevel < 0.6f)
        barColour = FrutigerColours::greenAccent;
    else if (displayLevel < 0.85f)
        barColour = FrutigerColours::warmOrange;
    else
        barColour = FrutigerColours::warmRed;

    g.setColour (barColour);
    g.fillRoundedRectangle (levelBounds, 3.0f);

    // Glass overlay
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::glassHighlight, bounds.getX(), bounds.getY(),
        juce::Colours::transparentBlack, bounds.getRight(), bounds.getY(),
        false));
    g.fillRoundedRectangle (getLocalBounds().toFloat().withWidth (bounds.getWidth() * 0.4f), 3.0f);
}
