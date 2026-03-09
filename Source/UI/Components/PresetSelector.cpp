#include "PresetSelector.h"

PresetSelector::PresetSelector (PresetManager& pm) : presetManager (pm)
{
    addAndMakeVisible (prevButton);
    addAndMakeVisible (presetBox);
    addAndMakeVisible (nextButton);
    addAndMakeVisible (saveButton);

    presetBox.setTextWhenNothingSelected ("Select Preset...");
    updatePresetList();

    presetBox.onChange = [this]()
    {
        auto idx = presetBox.getSelectedItemIndex();
        if (idx >= 0)
        {
            auto names = presetManager.getPresetNames();
            if (idx < names.size())
                presetManager.loadPreset (names[idx]);
        }
    };

    prevButton.onClick = [this]()
    {
        auto idx = presetBox.getSelectedItemIndex();
        if (idx > 0)
            presetBox.setSelectedItemIndex (idx - 1);
    };

    nextButton.onClick = [this]()
    {
        auto idx = presetBox.getSelectedItemIndex();
        if (idx < presetBox.getNumItems() - 1)
            presetBox.setSelectedItemIndex (idx + 1);
    };

    saveButton.onClick = [this]()
    {
        auto name = presetBox.getText();
        if (name.isNotEmpty())
        {
            presetManager.savePreset (name);
            updatePresetList();
        }
    };
}

void PresetSelector::updatePresetList()
{
    presetBox.clear();
    auto names = presetManager.getPresetNames();
    for (int i = 0; i < names.size(); ++i)
        presetBox.addItem (names[i], i + 1);
}

void PresetSelector::resized()
{
    auto bounds = getLocalBounds();
    prevButton.setBounds (bounds.removeFromLeft (30));
    saveButton.setBounds (bounds.removeFromRight (60));
    nextButton.setBounds (bounds.removeFromRight (30));
    bounds.removeFromLeft (4);
    bounds.removeFromRight (4);
    presetBox.setBounds (bounds);
}
