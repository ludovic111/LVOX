#pragma once
#include "PluginProcessor.h"
#include "UI/FrutigerAeroLookAndFeel.h"
#include "UI/SimpleModePanel.h"
#include "UI/AdvancedModePanel.h"
#include "UI/Components/PresetSelector.h"
#include "UI/Helpers/ColourPalette.h"

class LVOXAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit LVOXAudioProcessorEditor (LVOXAudioProcessor&);
    ~LVOXAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    LVOXAudioProcessor& processorRef;

    FrutigerAeroLookAndFeel frutigerLookAndFeel;

    SimpleModePanel   simplePanel;
    AdvancedModePanel advancedPanel;
    PresetSelector    presetSelector;

    juce::ComboBox micSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> micSelectorAttachment;

    juce::TextButton modeToggle { "Advanced" };
    bool isSimpleMode = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LVOXAudioProcessorEditor)
};
