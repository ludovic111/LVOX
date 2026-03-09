#include "MeterComponent.h"
#include "../Helpers/GradientUtils.h"

MeterComponent::MeterComponent()
{
    startTimerHz (30);
}

void MeterComponent::setLevel (float newLevelL, float newLevelR)
{
    currentLevelL = newLevelL;
    currentLevelR = newLevelR;
}

void MeterComponent::setLevel (float newLevel)
{
    currentLevelL = newLevel;
    currentLevelR = newLevel;
}

void MeterComponent::timerCallback()
{
    // Smooth falloff for L
    if (currentLevelL > displayLevelL)
        displayLevelL = currentLevelL;
    else
        displayLevelL = displayLevelL * 0.9f + currentLevelL * 0.1f;

    // Smooth falloff for R
    if (currentLevelR > displayLevelR)
        displayLevelR = currentLevelR;
    else
        displayLevelR = displayLevelR * 0.9f + currentLevelR * 0.1f;

    // Peak hold L
    if (displayLevelL >= peakLevelL)
    {
        peakLevelL = displayLevelL;
        peakHoldCounterL = peakHoldFrames;
    }
    else if (peakHoldCounterL > 0)
    {
        --peakHoldCounterL;
    }
    else
    {
        peakLevelL = peakLevelL * 0.95f;
    }

    // Peak hold R
    if (displayLevelR >= peakLevelR)
    {
        peakLevelR = displayLevelR;
        peakHoldCounterR = peakHoldFrames;
    }
    else if (peakHoldCounterR > 0)
    {
        --peakHoldCounterR;
    }
    else
    {
        peakLevelR = peakLevelR * 0.95f;
    }

    repaint();
}

void MeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Reserve space for label at bottom
    auto labelArea = bounds.removeFromBottom (label.isNotEmpty() ? 14.0f : 0.0f);

    // Background
    g.setColour (FrutigerColours::meterBg);
    g.fillRoundedRectangle (bounds, 3.0f);

    // Two meter bars side by side
    auto barWidth = (bounds.getWidth() - 2.0f) * 0.5f;
    auto leftBar = bounds.removeFromLeft (barWidth);
    bounds.removeFromLeft (2.0f);
    auto rightBar = bounds;

    auto drawBar = [&] (juce::Rectangle<float> bar, float level, float peak)
    {
        // Vertical gradient fill
        auto levelHeight = bar.getHeight() * level;
        auto levelBounds = bar.removeFromBottom (levelHeight);

        if (levelHeight > 0.5f)
        {
            juce::ColourGradient grad (
                FrutigerColours::warmRed, levelBounds.getX(), levelBounds.getY(),
                FrutigerColours::greenAccent, levelBounds.getX(), levelBounds.getBottom(),
                false);
            grad.addColour (0.6, FrutigerColours::warmOrange);
            g.setGradientFill (grad);
            g.fillRoundedRectangle (levelBounds, 2.0f);
        }

        // Peak hold indicator (2px line)
        if (peak > 0.01f)
        {
            auto peakY = bar.getBottom() - bar.getHeight() * peak;
            g.setColour (FrutigerColours::aquaLight.withAlpha (0.8f));
            g.fillRect (bar.getX(), peakY, bar.getWidth(), 2.0f);
        }
    };

    drawBar (leftBar, displayLevelL, peakLevelL);
    drawBar (rightBar, displayLevelR, peakLevelR);

    // Glass overlay
    auto fullBounds = getLocalBounds().toFloat();
    if (label.isNotEmpty())
        fullBounds.removeFromBottom (14.0f);
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::glassHighlight.withAlpha (0.1f), fullBounds.getX(), fullBounds.getY(),
        juce::Colours::transparentBlack, fullBounds.getRight(), fullBounds.getY(),
        false));
    g.fillRoundedRectangle (fullBounds.withWidth (fullBounds.getWidth() * 0.4f), 3.0f);

    // Label
    if (label.isNotEmpty())
    {
        g.setColour (FrutigerColours::textDim);
        g.setFont (10.0f);
        g.drawText (label, labelArea, juce::Justification::centred);
    }
}
