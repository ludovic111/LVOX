#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class PresetManager
{
public:
    PresetManager (juce::AudioProcessorValueTreeState& apvts);

    void loadPreset (const juce::String& name);
    void savePreset (const juce::String& name);
    void deletePreset (const juce::String& name);
    juce::StringArray getPresetNames() const;

    static juce::File getPresetsDirectory();

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::StringArray factoryPresetNames;

    void createFactoryPresets();
    juce::File getPresetFile (const juce::String& name) const;
};
