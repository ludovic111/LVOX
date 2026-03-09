#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "ColourPalette.h"

namespace GradientUtils
{
    inline void drawGlossyPanel (juce::Graphics& g, juce::Rectangle<float> bounds, float cornerRadius = 10.0f)
    {
        // Dark translucent background
        g.setColour (FrutigerColours::panelBg);
        g.fillRoundedRectangle (bounds, cornerRadius);

        // Glass highlight on top half
        auto topHalf = bounds.removeFromTop (bounds.getHeight() * 0.4f);
        g.setGradientFill (juce::ColourGradient (
            FrutigerColours::glassHighlight, topHalf.getX(), topHalf.getY(),
            juce::Colours::transparentBlack, topHalf.getX(), topHalf.getBottom(),
            false));
        g.fillRoundedRectangle (topHalf.reduced (1), cornerRadius);

        // Subtle border
        g.setColour (FrutigerColours::aquaDark.withAlpha (0.3f));
        g.drawRoundedRectangle (bounds, cornerRadius, 1.0f);
    }

    inline void drawGlowEffect (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour colour, float radius = 8.0f)
    {
        juce::Path glowPath;
        glowPath.addRoundedRectangle (bounds, 6.0f);
        juce::DropShadow glow (colour.withAlpha (0.4f), (int) radius, { 0, 0 });
        glow.drawForPath (g, glowPath);
    }

    inline juce::ColourGradient makeAquaGradient (juce::Rectangle<float> bounds)
    {
        return juce::ColourGradient (
            FrutigerColours::aquaLight, bounds.getX(), bounds.getY(),
            FrutigerColours::aquaDark, bounds.getX(), bounds.getBottom(),
            false);
    }
}
