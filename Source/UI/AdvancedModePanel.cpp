#include "AdvancedModePanel.h"

AdvancedModePanel::AdvancedModePanel (juce::AudioProcessorValueTreeState& apvts)
    : gatePanel (apvts),
      hpfPanel (apvts),
      deesserPanel (apvts),
      eqPanel (apvts),
      compPanel (apvts),
      satPanel (apvts),
      revPanel (apvts),
      dlyPanel (apvts),
      sendPanel (apvts),
      limPanel (apvts)
{
    contentContainer.addAndMakeVisible (gatePanel);
    contentContainer.addAndMakeVisible (hpfPanel);
    contentContainer.addAndMakeVisible (deesserPanel);
    contentContainer.addAndMakeVisible (eqPanel);
    contentContainer.addAndMakeVisible (compPanel);
    contentContainer.addAndMakeVisible (satPanel);
    contentContainer.addAndMakeVisible (sendPanel);
    contentContainer.addAndMakeVisible (revPanel);
    contentContainer.addAndMakeVisible (dlyPanel);
    contentContainer.addAndMakeVisible (limPanel);

    viewport.setViewedComponent (&contentContainer, false);
    viewport.setScrollBarsShown (true, false);
    addAndMakeVisible (viewport);

    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);

    inputMeter.setLabel ("IN");
    outputMeter.setLabel ("OUT");
}

void AdvancedModePanel::paint (juce::Graphics& g)
{
    g.fillAll (FrutigerColours::backgroundDark);

    // Styled header with gradient text
    auto headerBounds = getLocalBounds().removeFromTop (36).toFloat();

    // "LVOX" gradient text
    g.setFont (22.0f);
    g.setGradientFill (GradientUtils::makeAquaGradient (headerBounds));
    g.drawText ("LVOX", headerBounds.withWidth (80.0f).withX (headerBounds.getCentreX() - 70.0f),
                juce::Justification::centredRight);

    // "ADVANCED" subtitle
    g.setColour (FrutigerColours::textDim);
    g.setFont (12.0f);
    g.drawText ("ADVANCED", headerBounds.withWidth (100.0f).withX (headerBounds.getCentreX() - 70.0f + 85.0f),
                juce::Justification::centredLeft);
}

void AdvancedModePanel::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (36);

    // Meters on sides
    inputMeter.setBounds (bounds.removeFromLeft (20));
    bounds.removeFromLeft (4);
    outputMeter.setBounds (bounds.removeFromRight (20));
    bounds.removeFromRight (4);

    viewport.setBounds (bounds);

    // Layout modules vertically in content container
    int panelWidth = bounds.getWidth() - 20; // account for scrollbar
    int y = 4;
    int moduleHeight = 120;
    int eqHeight = 280;
    int gap = 10;
    int arrowGap = 14; // space for signal flow arrow

    auto placeModule = [&] (juce::Component& comp, int height, bool drawArrow = true)
    {
        comp.setBounds (4, y, panelWidth, height);
        y += height;
        if (drawArrow)
            y += arrowGap;
        else
            y += gap;
    };

    placeModule (gatePanel, moduleHeight);
    placeModule (hpfPanel, 80);
    placeModule (deesserPanel, moduleHeight);
    placeModule (eqPanel, eqHeight);
    placeModule (compPanel, moduleHeight);
    placeModule (satPanel, moduleHeight);
    placeModule (sendPanel, 80);
    placeModule (revPanel, moduleHeight);
    placeModule (dlyPanel, moduleHeight);
    placeModule (limPanel, 80, false);

    contentContainer.setSize (panelWidth + 8, y);
}

void AdvancedModePanel::paintOverChildren (juce::Graphics& g)
{
    // Draw signal flow arrows between panels in the viewport
    auto viewPos = viewport.getViewPosition();
    auto vpBounds = viewport.getBounds();
    float cx = vpBounds.getX() + vpBounds.getWidth() * 0.5f;

    // Get positions of each module panel and draw arrows between them
    juce::Component* panels[] = { &gatePanel, &hpfPanel, &deesserPanel, &eqPanel,
                                   &compPanel, &satPanel, &sendPanel, &revPanel,
                                   &dlyPanel, &limPanel };

    for (int i = 0; i < 9; ++i)
    {
        auto panelBottom = panels[i]->getBottom() - viewPos.y + vpBounds.getY();
        auto nextTop = panels[i + 1]->getY() - viewPos.y + vpBounds.getY();
        auto arrowY = (panelBottom + nextTop) * 0.5f;

        if (arrowY > vpBounds.getY() && arrowY < vpBounds.getBottom())
            GradientUtils::drawSignalFlowArrow (g, cx, arrowY);
    }
}
