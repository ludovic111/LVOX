#include "FrutigerAeroLookAndFeel.h"
#include "Helpers/GradientUtils.h"

FrutigerAeroLookAndFeel::FrutigerAeroLookAndFeel()
{
    setColour (juce::Slider::rotarySliderFillColourId, FrutigerColours::knobFill);
    setColour (juce::Slider::rotarySliderOutlineColourId, FrutigerColours::knobTrack);
    setColour (juce::Slider::thumbColourId, FrutigerColours::aquaLight);
    setColour (juce::Label::textColourId, FrutigerColours::textLight);
    setColour (juce::ComboBox::backgroundColourId, FrutigerColours::panelBg);
    setColour (juce::ComboBox::textColourId, FrutigerColours::textLight);
    setColour (juce::ComboBox::outlineColourId, FrutigerColours::aquaDark.withAlpha (0.5f));
    setColour (juce::PopupMenu::backgroundColourId, FrutigerColours::backgroundMid);
    setColour (juce::PopupMenu::textColourId, FrutigerColours::textLight);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, FrutigerColours::aquaDark);
    setColour (juce::TextButton::buttonColourId, FrutigerColours::panelBgLight);
    setColour (juce::TextButton::textColourOffId, FrutigerColours::textLight);
}

void FrutigerAeroLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                                 float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                                 juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (4.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Outer track
    juce::Path track;
    track.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour (FrutigerColours::knobTrack);
    g.strokePath (track, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Value arc
    juce::Path valueArc;
    valueArc.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
    g.setColour (FrutigerColours::knobFill);
    g.strokePath (valueArc, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Knob body - glossy circle
    auto knobRadius = radius * 0.7f;
    auto knobBounds = juce::Rectangle<float> (centreX - knobRadius, centreY - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);

    // Dark fill
    g.setColour (FrutigerColours::knobBg);
    g.fillEllipse (knobBounds);

    // Glass highlight on top
    auto highlightBounds = knobBounds.reduced (knobRadius * 0.15f);
    highlightBounds = highlightBounds.removeFromTop (highlightBounds.getHeight() * 0.5f);
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::glassWhite, centreX, knobBounds.getY(),
        juce::Colours::transparentBlack, centreX, centreY,
        false));
    g.fillEllipse (knobBounds.reduced (2.0f));

    // Pointer line
    juce::Path pointer;
    auto pointerLength = knobRadius * 0.6f;
    auto pointerThickness = 2.5f;
    pointer.addRoundedRectangle (-pointerThickness * 0.5f, -knobRadius + 4.0f, pointerThickness, pointerLength, 1.0f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (FrutigerColours::aquaLight);
    g.fillPath (pointer);
}

void FrutigerAeroLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                                     const juce::Colour& backgroundColour,
                                                     bool isMouseOver, bool isButtonDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (1.0f);
    auto cornerRadius = 6.0f;

    auto baseColour = backgroundColour;
    if (isButtonDown)
        baseColour = baseColour.brighter (0.2f);
    else if (isMouseOver)
        baseColour = baseColour.brighter (0.1f);

    // Glow effect for toggle buttons that are on
    if (button.getToggleState())
    {
        GradientUtils::drawGlowEffect (g, bounds, FrutigerColours::aquaGlow, 6.0f);
        baseColour = FrutigerColours::aquaDark;
    }

    g.setColour (baseColour);
    g.fillRoundedRectangle (bounds, cornerRadius);

    // Glass highlight
    auto topHalf = bounds.removeFromTop (bounds.getHeight() * 0.45f);
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::glassHighlight, topHalf.getX(), topHalf.getY(),
        juce::Colours::transparentBlack, topHalf.getX(), topHalf.getBottom(),
        false));
    g.fillRoundedRectangle (topHalf, cornerRadius);
}

void FrutigerAeroLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                                 bool isMouseOver, bool isButtonDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (2.0f);
    auto isOn = button.getToggleState();

    auto circleSize = juce::jmin (bounds.getHeight(), 20.0f);
    auto circleBounds = juce::Rectangle<float> (bounds.getX(), bounds.getCentreY() - circleSize * 0.5f, circleSize, circleSize);

    // Circle background
    g.setColour (isOn ? FrutigerColours::activeColour : FrutigerColours::bypassedColour);
    g.fillEllipse (circleBounds);

    if (isOn)
        GradientUtils::drawGlowEffect (g, circleBounds, FrutigerColours::aquaGlow, 4.0f);

    // Label text
    g.setColour (FrutigerColours::textLight);
    g.setFont (13.0f);
    auto textBounds = bounds.withLeft (circleBounds.getRight() + 6.0f);
    g.drawText (button.getButtonText(), textBounds, juce::Justification::centredLeft);
}

void FrutigerAeroLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                                             int, int, int, int, juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float> (0, 0, (float) width, (float) height);
    auto cornerRadius = 6.0f;

    g.setColour (FrutigerColours::panelBg);
    g.fillRoundedRectangle (bounds, cornerRadius);

    g.setColour (FrutigerColours::aquaDark.withAlpha (0.5f));
    g.drawRoundedRectangle (bounds.reduced (0.5f), cornerRadius, 1.0f);

    // Arrow
    auto arrowZone = juce::Rectangle<float> ((float) width - 20.0f, 0.0f, 16.0f, (float) height);
    juce::Path arrow;
    arrow.addTriangle (arrowZone.getCentreX() - 4.0f, arrowZone.getCentreY() - 2.0f,
                       arrowZone.getCentreX() + 4.0f, arrowZone.getCentreY() - 2.0f,
                       arrowZone.getCentreX(), arrowZone.getCentreY() + 3.0f);
    g.setColour (FrutigerColours::textLight);
    g.fillPath (arrow);
}

void FrutigerAeroLookAndFeel::drawGroupComponentOutline (juce::Graphics& g, int w, int h,
                                                          const juce::String& text,
                                                          const juce::Justification&,
                                                          juce::GroupComponent&)
{
    auto bounds = juce::Rectangle<float> (0, 0, (float) w, (float) h);
    GradientUtils::drawGlossyPanel (g, bounds, 8.0f);

    if (text.isNotEmpty())
    {
        g.setColour (FrutigerColours::textBright);
        g.setFont (14.0f);
        g.drawText (text, bounds.reduced (10, 4).removeFromTop (22), juce::Justification::centredLeft);
    }
}

void FrutigerAeroLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.setColour (label.findColour (juce::Label::textColourId));
    g.setFont (getLabelFont (label));
    g.drawText (label.getText(), label.getLocalBounds(), label.getJustificationType(), false);
}

juce::Font FrutigerAeroLookAndFeel::getComboBoxFont (juce::ComboBox&)
{
    return juce::Font (14.0f);
}

juce::Font FrutigerAeroLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::Font (12.0f);
}
