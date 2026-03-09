#include "PluginEditor.h"
#include "Parameters.h"

LVOXAudioProcessorEditor::LVOXAudioProcessorEditor (LVOXAudioProcessor& p)
    : AudioProcessorEditor (p),
      processorRef (p),
      simplePanel (p.apvts),
      advancedPanel (p.apvts),
      presetSelector (p.presetManager)
{
    setLookAndFeel (&frutigerLookAndFeel);

    addAndMakeVisible (presetSelector);

    micSelector.addItemList ({ "No Mic", "UAD Sphere LX (C800)", "Shure MV7" }, 1);
    micSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (p.apvts, ParamIDs::micSelect, micSelector);
    addAndMakeVisible (micSelector);

    addAndMakeVisible (modeToggle);
    addAndMakeVisible (simplePanel);
    addChildComponent (advancedPanel); // hidden initially

    modeToggle.onClick = [this]()
    {
        isSimpleMode = !isSimpleMode;
        simplePanel.setVisible (isSimpleMode);
        advancedPanel.setVisible (!isSimpleMode);
        modeToggle.setButtonText (isSimpleMode ? "Advanced" : "Simple");

        if (isSimpleMode)
            setSize (900, 500);
        else
            setSize (1100, 700);
    };

    setSize (900, 500);
    startTimerHz (30);
}

LVOXAudioProcessorEditor::~LVOXAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void LVOXAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background gradient
    g.setGradientFill (juce::ColourGradient (
        FrutigerColours::backgroundDark, 0, 0,
        FrutigerColours::backgroundMid, 0, (float) getHeight(),
        false));
    g.fillRect (getLocalBounds());
}

void LVOXAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Top bar: preset selector + mic selector + mode toggle
    auto topBar = bounds.removeFromTop (36);
    topBar.reduce (8, 4);
    modeToggle.setBounds (topBar.removeFromRight (100));
    topBar.removeFromRight (8);
    micSelector.setBounds (topBar.removeFromRight (180));
    topBar.removeFromRight (8);
    presetSelector.setBounds (topBar);

    bounds.reduce (8, 8);

    simplePanel.setBounds (bounds);
    advancedPanel.setBounds (bounds);
}

void LVOXAudioProcessorEditor::timerCallback()
{
    float inL = processorRef.getInputLevelL();
    float inR = processorRef.getInputLevelR();
    float outL = processorRef.getOutputLevelL();
    float outR = processorRef.getOutputLevelR();

    if (isSimpleMode)
    {
        simplePanel.getInputMeter().setLevel (inL, inR);
        simplePanel.getOutputMeter().setLevel (outL, outR);
    }
    else
    {
        advancedPanel.getInputMeter().setLevel (inL, inR);
        advancedPanel.getOutputMeter().setLevel (outL, outR);
    }
}
