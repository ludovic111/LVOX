#include "PresetManager.h"
#include "../Parameters.h"

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& apvts) : apvts (apvts)
{
    auto dir = getPresetsDirectory();
    if (!dir.exists())
        dir.createDirectory();

    factoryPresetNames = { "Podcast", "Rap Vocal", "Singing Lead", "Background Vocal", "Radio Voice",
                           "Lo-Fi Vocal", "Bright Pop", "Intimate ASMR", "Aggressive Rock",
                           "Choir Stack", "Telephone", "Dreamy Ethereal", "RnB Smooth" };
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

    // Lo-Fi Vocal preset — warm, filtered, vinyl-like character
    if (!getPresetFile ("Lo-Fi Vocal").exists())
    {
        setParam (ParamIDs::macroWarmth, 80.0f);
        setParam (ParamIDs::macroPresence, 20.0f);
        setParam (ParamIDs::macroCompression, 55.0f);
        setParam (ParamIDs::macroSpace, 15.0f);
        setParam (ParamIDs::satDrive, 60.0f);
        setParam (ParamIDs::satType, 0.0f); // Tape
        setParam (ParamIDs::satTone, -40.0f);
        setParam (ParamIDs::eqBand4Gain, -6.0f); // roll off highs
        setParam (ParamIDs::eqBand1Gain, 3.0f); // low shelf boost
        setParam (ParamIDs::hpfFrequency, 60.0f);
        savePreset ("Lo-Fi Vocal");
    }

    // Bright Pop preset — bright, polished, radio-ready
    if (!getPresetFile ("Bright Pop").exists())
    {
        setParam (ParamIDs::macroWarmth, 30.0f);
        setParam (ParamIDs::macroPresence, 75.0f);
        setParam (ParamIDs::macroCompression, 65.0f);
        setParam (ParamIDs::macroSpace, 20.0f);
        setParam (ParamIDs::hpfFrequency, 100.0f);
        setParam (ParamIDs::eqBand3Gain, 4.0f); // hi-mid presence
        setParam (ParamIDs::eqBand3Freq, 4000.0f);
        setParam (ParamIDs::eqBand4Gain, 2.0f); // air
        setParam (ParamIDs::deessThreshold, -18.0f);
        setParam (ParamIDs::compAttack, 8.0f);
        setParam (ParamIDs::compRatio, 4.0f);
        savePreset ("Bright Pop");
    }

    // Intimate ASMR preset — minimal processing, close and present
    if (!getPresetFile ("Intimate ASMR").exists())
    {
        setParam (ParamIDs::macroWarmth, 25.0f);
        setParam (ParamIDs::macroPresence, 35.0f);
        setParam (ParamIDs::macroCompression, 30.0f);
        setParam (ParamIDs::macroSpace, 0.0f);
        setParam (ParamIDs::hpfFrequency, 80.0f);
        setParam (ParamIDs::gateThreshold, -50.0f);
        setParam (ParamIDs::compThreshold, -15.0f);
        setParam (ParamIDs::compRatio, 2.0f);
        setParam (ParamIDs::compAttack, 15.0f);
        setParam (ParamIDs::satBypass, 1.0f); // bypass saturation
        setParam (ParamIDs::deessThreshold, -15.0f);
        savePreset ("Intimate ASMR");
    }

    // Aggressive Rock preset — heavy compression and distortion
    if (!getPresetFile ("Aggressive Rock").exists())
    {
        setParam (ParamIDs::macroWarmth, 75.0f);
        setParam (ParamIDs::macroPresence, 85.0f);
        setParam (ParamIDs::macroCompression, 90.0f);
        setParam (ParamIDs::macroSpace, 10.0f);
        setParam (ParamIDs::hpfFrequency, 150.0f);
        setParam (ParamIDs::compThreshold, -30.0f);
        setParam (ParamIDs::compRatio, 10.0f);
        setParam (ParamIDs::compAttack, 2.0f);
        setParam (ParamIDs::compRelease, 50.0f);
        setParam (ParamIDs::satDrive, 70.0f);
        setParam (ParamIDs::satType, 3.0f); // Hard Clip
        setParam (ParamIDs::eqBand2Gain, -3.0f); // scoop mids
        setParam (ParamIDs::eqBand3Gain, 5.0f); // push upper mids
        savePreset ("Aggressive Rock");
    }

    // Choir Stack preset — for layered/stacked background vocals
    if (!getPresetFile ("Choir Stack").exists())
    {
        setParam (ParamIDs::macroWarmth, 40.0f);
        setParam (ParamIDs::macroPresence, 25.0f);
        setParam (ParamIDs::macroCompression, 55.0f);
        setParam (ParamIDs::macroSpace, 55.0f);
        setParam (ParamIDs::hpfFrequency, 200.0f);
        setParam (ParamIDs::eqBand3Gain, -3.0f); // reduce presence to sit back
        setParam (ParamIDs::eqBand1Gain, -2.0f); // thin out lows
        setParam (ParamIDs::revSize, 70.0f);
        setParam (ParamIDs::revPredelay, 30.0f);
        setParam (ParamIDs::compRatio, 3.0f);
        savePreset ("Choir Stack");
    }

    // Telephone preset — band-passed, compressed, lo-fi
    if (!getPresetFile ("Telephone").exists())
    {
        setParam (ParamIDs::macroWarmth, 45.0f);
        setParam (ParamIDs::macroPresence, 60.0f);
        setParam (ParamIDs::macroCompression, 85.0f);
        setParam (ParamIDs::macroSpace, 0.0f);
        setParam (ParamIDs::hpfFrequency, 400.0f);
        setParam (ParamIDs::eqBand4Gain, -12.0f); // aggressive high cut
        setParam (ParamIDs::eqBand4Freq, 3500.0f);
        setParam (ParamIDs::eqBand2Gain, 4.0f); // mid-range push
        setParam (ParamIDs::eqBand2Freq, 1200.0f);
        setParam (ParamIDs::compRatio, 12.0f);
        setParam (ParamIDs::compThreshold, -25.0f);
        setParam (ParamIDs::satDrive, 55.0f);
        setParam (ParamIDs::satType, 2.0f); // Soft Clip
        savePreset ("Telephone");
    }

    // Dreamy Ethereal preset — lots of reverb/delay, soft processing
    if (!getPresetFile ("Dreamy Ethereal").exists())
    {
        setParam (ParamIDs::macroWarmth, 45.0f);
        setParam (ParamIDs::macroPresence, 40.0f);
        setParam (ParamIDs::macroCompression, 35.0f);
        setParam (ParamIDs::macroSpace, 85.0f);
        setParam (ParamIDs::hpfFrequency, 80.0f);
        setParam (ParamIDs::revSize, 85.0f);
        setParam (ParamIDs::revDamping, 30.0f);
        setParam (ParamIDs::revPredelay, 60.0f);
        setParam (ParamIDs::dlySync, 1.0f);
        setParam (ParamIDs::dlyNote, 1.0f); // 1/8 note
        setParam (ParamIDs::dlyFeedback, 50.0f);
        setParam (ParamIDs::compRatio, 2.0f);
        savePreset ("Dreamy Ethereal");
    }

    // RnB Smooth preset — smooth, warm, moderate space
    if (!getPresetFile ("RnB Smooth").exists())
    {
        setParam (ParamIDs::macroWarmth, 55.0f);
        setParam (ParamIDs::macroPresence, 50.0f);
        setParam (ParamIDs::macroCompression, 60.0f);
        setParam (ParamIDs::macroSpace, 30.0f);
        setParam (ParamIDs::hpfFrequency, 90.0f);
        setParam (ParamIDs::satDrive, 25.0f);
        setParam (ParamIDs::satType, 1.0f); // Tube
        setParam (ParamIDs::compAttack, 12.0f);
        setParam (ParamIDs::compRelease, 120.0f);
        setParam (ParamIDs::compRatio, 3.5f);
        setParam (ParamIDs::eqBand1Gain, 2.0f); // warm low shelf
        setParam (ParamIDs::eqBand3Gain, 2.5f); // gentle presence
        setParam (ParamIDs::eqBand3Freq, 3500.0f);
        setParam (ParamIDs::revPredelay, 35.0f);
        savePreset ("RnB Smooth");
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
