#include "PresetManager.h"
#include "../Parameters.h"

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& apvts) : apvts (apvts)
{
    auto dir = getPresetsDirectory();
    if (!dir.exists())
        dir.createDirectory();

    factoryPresetNames = { "Podcast", "Rap Vocal", "Singing Lead", "Background Vocal", "Radio Voice" };
    createFactoryPresets();
}

juce::File PresetManager::getPresetsDirectory()
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
               .getChildFile ("LVOX")
               .getChildFile ("Presets");
}

juce::File PresetManager::getPresetFile (const juce::String& name) const
{
    return getPresetsDirectory().getChildFile (name + ".xml");
}

void PresetManager::createFactoryPresets()
{
    // Create factory presets if they don't exist
    auto setParam = [this] (const juce::String& id, float value)
    {
        if (auto* param = apvts.getParameter (id))
            param->setValueNotifyingHost (param->convertTo0to1 (value));
    };

    // Podcast preset
    if (!getPresetFile ("Podcast").exists())
    {
        setParam (ParamIDs::macroWarmth, 40.0f);
        setParam (ParamIDs::macroPresence, 55.0f);
        setParam (ParamIDs::macroCompression, 60.0f);
        setParam (ParamIDs::macroSpace, 0.0f);
        setParam (ParamIDs::hpfFrequency, 120.0f);
        setParam (ParamIDs::gateBypass, 0.0f);
        setParam (ParamIDs::compScActive, 1.0f);
        savePreset ("Podcast");
    }

    // Rap Vocal preset
    if (!getPresetFile ("Rap Vocal").exists())
    {
        setParam (ParamIDs::macroWarmth, 60.0f);
        setParam (ParamIDs::macroPresence, 70.0f);
        setParam (ParamIDs::macroCompression, 75.0f);
        setParam (ParamIDs::macroSpace, 25.0f);
        setParam (ParamIDs::compAttack, 3.0f);
        savePreset ("Rap Vocal");
    }

    // Singing Lead preset
    if (!getPresetFile ("Singing Lead").exists())
    {
        setParam (ParamIDs::macroWarmth, 50.0f);
        setParam (ParamIDs::macroPresence, 50.0f);
        setParam (ParamIDs::macroCompression, 45.0f);
        setParam (ParamIDs::macroSpace, 40.0f);
        setParam (ParamIDs::revPredelay, 40.0f);
        setParam (ParamIDs::dlySync, 1.0f);
        savePreset ("Singing Lead");
    }

    // Background Vocal preset
    if (!getPresetFile ("Background Vocal").exists())
    {
        setParam (ParamIDs::macroWarmth, 35.0f);
        setParam (ParamIDs::macroPresence, 30.0f);
        setParam (ParamIDs::macroCompression, 50.0f);
        setParam (ParamIDs::macroSpace, 65.0f);
        setParam (ParamIDs::eqBand3Gain, -2.0f);
        savePreset ("Background Vocal");
    }

    // Radio Voice preset
    if (!getPresetFile ("Radio Voice").exists())
    {
        setParam (ParamIDs::macroWarmth, 70.0f);
        setParam (ParamIDs::macroPresence, 80.0f);
        setParam (ParamIDs::macroCompression, 80.0f);
        setParam (ParamIDs::macroSpace, 5.0f);
        setParam (ParamIDs::compRatio, 8.0f);
        setParam (ParamIDs::satDrive, 50.0f);
        setParam (ParamIDs::hpfFrequency, 200.0f);
        savePreset ("Radio Voice");
    }
}

void PresetManager::loadPreset (const juce::String& name)
{
    auto file = getPresetFile (name);
    if (!file.existsAsFile())
        return;

    auto xml = juce::XmlDocument::parse (file);
    if (xml == nullptr)
        return;

    auto tree = juce::ValueTree::fromXml (*xml);
    if (tree.isValid())
        apvts.replaceState (tree);
}

void PresetManager::savePreset (const juce::String& name)
{
    auto file = getPresetFile (name);
    auto state = apvts.copyState();
    auto xml = state.createXml();
    if (xml != nullptr)
        xml->writeTo (file);
}

void PresetManager::deletePreset (const juce::String& name)
{
    if (factoryPresetNames.contains (name))
        return; // don't delete factory presets

    auto file = getPresetFile (name);
    if (file.existsAsFile())
        file.deleteFile();
}

juce::StringArray PresetManager::getPresetNames() const
{
    juce::StringArray names;

    // Factory presets first
    for (auto& name : factoryPresetNames)
        names.add (name);

    // User presets
    auto dir = getPresetsDirectory();
    for (auto& file : dir.findChildFiles (juce::File::findFiles, false, "*.xml"))
    {
        auto name = file.getFileNameWithoutExtension();
        if (!names.contains (name))
            names.add (name);
    }

    return names;
}
