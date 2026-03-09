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

    // Value arc glow (second pass, wider + transparent)
    g.setColour (FrutigerColours::knobFill.withAlpha (0.15f));
    g.strokePath (valueArc, juce::PathStrokeType (6.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Knob body
    auto knobRadius = radius * 0.7f;
    auto knobBounds = juce::Rectangle<float> (centreX - knobRadius, centreY - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);

    // Drop shadow under knob
    GradientUtils::drawKnobShadow (g, centreX, centreY + 1.5f, knobRadius);

    // Radial gradient body (brighter center → darker edge)
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::knobBg.brighter (0.25f), centreX, centreY,
        FrutigerColours::knobBg.darker (0.15f), centreX, centreY - knobRadius,
        true));
    g.fillEllipse (knobBounds);

    // Outer ring stroke
    auto ringAlpha = slider.isMouseOver() ? 0.5f : 0.3f;
    g.setColour (FrutigerColours::knobRing.withAlpha (ringAlpha));
    g.drawEllipse (knobBounds.reduced (0.5f), 1.0f);

    // Specular glass highlight (upper-left quadrant)
    auto highlightRadius = knobRadius * 0.7f;
    auto hlCx = centreX - knobRadius * 0.2f;
    auto hlCy = centreY - knobRadius * 0.25f;
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::glassWhite.withAlpha (0.35f), hlCx, hlCy,
        juce::Colours::transparentBlack, hlCx + highlightRadius, hlCy + highlightRadius,
        true));
    g.fillEllipse (knobBounds.reduced (2.0f));

    // Pointer line
    juce::Path pointer;
    auto pointerLength = knobRadius * 0.6f;
    auto pointerThickness = 2.5f;
    pointer.addRoundedRectangle (-pointerThickness * 0.5f, -knobRadius + 4.0f, pointerThickness, pointerLength, 1.0f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (FrutigerColours::aquaLight);
    g.fillPath (pointer);

    // Pointer tip dot + glow
    auto tipX = centreX + (knobRadius - 6.0f) * std::sin (angle);
    auto tipY = centreY - (knobRadius - 6.0f) * std::cos (angle);
    g.setColour (FrutigerColours::aquaLight.withAlpha (0.3f));
    g.fillEllipse (tipX - 4.0f, tipY - 4.0f, 8.0f, 8.0f);
    g.setColour (FrutigerColours::aquaLight);
    g.fillEllipse (tipX - 2.0f, tipY - 2.0f, 4.0f, 4.0f);
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

    // Pill switch dimensions
    auto pillHeight = juce::jmin (bounds.getHeight(), 18.0f);
    auto pillWidth = pillHeight * 1.8f;
    auto pillBounds = juce::Rectangle<float> (bounds.getX(), bounds.getCentreY() - pillHeight * 0.5f, pillWidth, pillHeight);
    auto cornerRadius = pillHeight * 0.5f;

    // Pill background
    g.setColour (isOn ? FrutigerColours::activeColour.withAlpha (0.6f) : FrutigerColours::bypassedColour.withAlpha (0.5f));
    g.fillRoundedRectangle (pillBounds, cornerRadius);

    // Pill border
    g.setColour (isOn ? FrutigerColours::activeColour : FrutigerColours::bypassedColour.brighter (0.2f));
    g.drawRoundedRectangle (pillBounds, cornerRadius, 1.0f);

    // Glow when on
    if (isOn)
    {
        GradientUtils::drawGlowEffect (g, pillBounds, FrutigerColours::aquaGlow, 4.0f);
    }

    // Sliding thumb
    auto thumbDiameter = pillHeight - 4.0f;
    auto thumbX = isOn ? pillBounds.getRight() - thumbDiameter - 2.0f : pillBounds.getX() + 2.0f;
    auto thumbY = pillBounds.getCentreY() - thumbDiameter * 0.5f;
    auto thumbBounds = juce::Rectangle<float> (thumbX, thumbY, thumbDiameter, thumbDiameter);

    g.setColour (juce::Colours::white);
    g.fillEllipse (thumbBounds);

    // Thumb highlight
    g.setGradientFill (juce::ColourGradient (
        juce::Colours::white.withAlpha (0.8f), thumbBounds.getCentreX(), thumbBounds.getY(),
        juce::Colours::white.withAlpha (0.3f), thumbBounds.getCentreX(), thumbBounds.getBottom(),
        false));
    g.fillEllipse (thumbBounds.reduced (1.0f));

    // Label text
    g.setColour (FrutigerColours::textLight);
    g.setFont (13.0f);
    auto textBounds = bounds.withLeft (pillBounds.getRight() + 6.0f);
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
