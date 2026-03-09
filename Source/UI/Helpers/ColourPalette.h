#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace FrutigerColours
{
    // Main background
    inline const juce::Colour backgroundDark   { 0xFF0E1E2A };
    inline const juce::Colour backgroundMid    { 0xFF1B3A4B };

    // Translucent panels
    inline const juce::Colour panelBg          { 0xDD1B3A4B };
    inline const juce::Colour panelBgLight     { 0xBB2A5060 };
    inline const juce::Colour glassWhite       { 0x44FFFFFF };
    inline const juce::Colour glassHighlight   { 0x33FFFFFF };

    // Aqua / Blue accents
    inline const juce::Colour aquaLight        { 0xFF80E8F8 };
    inline const juce::Colour aquaDark         { 0xFF2AA4CF };
    inline const juce::Colour aquaGlow         { 0x6600D4FF };

    // Green / Nature accents
    inline const juce::Colour greenAccent      { 0xFF7AD48E };
    inline const juce::Colour greenDark        { 0xFF3A9E5C };

    // Orange / Warm tones
    inline const juce::Colour warmOrange       { 0xFFF5A623 };
    inline const juce::Colour warmRed          { 0xFFE85D5D };

    // Text
    inline const juce::Colour textLight        { 0xFFF0F8FF };
    inline const juce::Colour textDim          { 0xBBCCDDEE };
    inline const juce::Colour textBright       { 0xFFFFFFFF };

    // Knob / Slider
    inline const juce::Colour knobTrack        { 0xFF1A3040 };
    inline const juce::Colour knobFill         { 0xFF4AC6E8 };
    inline const juce::Colour knobBg           { 0xFF223344 };

    // Module status
    inline const juce::Colour bypassedColour   { 0xFF555555 };
    inline const juce::Colour activeColour     { 0xFF4AC6E8 };
}
