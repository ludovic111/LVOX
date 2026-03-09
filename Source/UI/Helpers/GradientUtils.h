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

    inline void drawEnhancedGlossyPanel (juce::Graphics& g, juce::Rectangle<float> bounds,
                                          juce::Colour accentColour, float cornerRadius = 10.0f)
    {
        // Drop shadow
        juce::Path shadowPath;
        shadowPath.addRoundedRectangle (bounds, cornerRadius);
        juce::DropShadow shadow (FrutigerColours::knobShadow, 6, { 0, 2 });
        shadow.drawForPath (g, shadowPath);

        // Dark translucent background (warmer)
        g.setColour (FrutigerColours::panelBg.interpolatedWith (accentColour, 0.05f));
        g.fillRoundedRectangle (bounds, cornerRadius);

        // Colored left accent bar (3px)
        auto accentBar = bounds.removeFromLeft (3.0f);
        g.setColour (accentColour);
        g.fillRoundedRectangle (accentBar, 1.5f);

        // Glass highlight on top 40%
        auto topHalf = bounds.removeFromTop (bounds.getHeight() * 0.4f);
        g.setGradientFill (juce::ColourGradient (
            FrutigerColours::glassHighlight.withAlpha (0.15f), topHalf.getX(), topHalf.getY(),
            juce::Colours::transparentBlack, topHalf.getX(), topHalf.getBottom(),
            false));
        g.fillRoundedRectangle (topHalf.reduced (1), cornerRadius);

        // Inner border
        g.setColour (accentColour.withAlpha (0.2f));
        g.drawRoundedRectangle (bounds.expanded (3.0f, 0.0f), cornerRadius, 1.0f);
    }

    inline void drawKnobShadow (juce::Graphics& g, float cx, float cy, float radius)
    {
        juce::DropShadow shadow (FrutigerColours::knobShadow, 5, { 0, 2 });
        juce::Path p;
        p.addEllipse (cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
        shadow.drawForPath (g, p);
    }

    inline void drawSignalFlowArrow (juce::Graphics& g, float cx, float y, float size = 8.0f)
    {
        juce::Path arrow;
        arrow.addTriangle (cx - size * 0.5f, y,
                           cx + size * 0.5f, y,
                           cx, y + size * 0.7f);
        g.setColour (FrutigerColours::aquaDark.withAlpha (0.4f));
        g.fillPath (arrow);
    }
}
