#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Preset/PresetManager.h"

class PresetSelector : public juce::Component
{
public:
    PresetSelector (PresetManager& pm);

    void resized() override;
    void updatePresetList();

private:
    PresetManager& presetManager;
    juce::ComboBox presetBox;
    juce::TextButton prevButton { "<" };
    juce::TextButton nextButton { ">" };
    juce::TextButton saveButton { "Save" };
};
