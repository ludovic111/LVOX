#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "Helpers/ColourPalette.h"

class FrutigerAeroLookAndFeel : public juce::LookAndFeel_V4
{
public:
    FrutigerAeroLookAndFeel();

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider&) override;

    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour&,
                               bool isMouseOver, bool isButtonDown) override;

    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                           bool isMouseOver, bool isButtonDown) override;

    void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox&) override;

    void drawGroupComponentOutline (juce::Graphics&, int w, int h,
                                    const juce::String& text,
                                    const juce::Justification&,
                                    juce::GroupComponent&) override;

    void drawLabel (juce::Graphics&, juce::Label&) override;

    juce::Font getComboBoxFont (juce::ComboBox&) override;
    juce::Font getLabelFont (juce::Label&) override;
};
