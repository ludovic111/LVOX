cmake_minimum_required(VERSION 3.22)
project(LVOX VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

juce_add_plugin(LVOX
    COMPANY_NAME "LudovicMarie"
    PLUGIN_MANUFACTURER_CODE Lvmc
    PLUGIN_CODE Lvox
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "LVOX"
    BUNDLE_ID "com.ludovicmarie.lvox"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(LVOX PRIVATE
    # Core
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/Parameters.cpp

    # DSP
    Source/DSP/MicCorrection.h
    Source/DSP/DSPChain.cpp
    Source/DSP/NoiseGate.cpp
    Source/DSP/HighPassFilter.cpp
    Source/DSP/DeEsser.cpp
    Source/DSP/ParametricEQ.cpp
    Source/DSP/Compressor.cpp
    Source/DSP/Saturation.cpp
    Source/DSP/LVOXReverb.cpp
    Source/DSP/Delay.cpp
    Source/DSP/Limiter.cpp

    # UI
    Source/UI/FrutigerAeroLookAndFeel.cpp
    Source/UI/SimpleModePanel.cpp
    Source/UI/AdvancedModePanel.cpp
    Source/UI/ModulePanel.cpp
    Source/UI/Panels/NoiseGatePanel.cpp
    Source/UI/Panels/HighPassPanel.cpp
    Source/UI/Panels/DeEsserPanel.cpp
    Source/UI/Panels/EQPanel.cpp
    Source/UI/Panels/CompressorPanel.cpp
    Source/UI/Panels/SaturationPanel.cpp
    Source/UI/Panels/ReverbPanel.cpp
    Source/UI/Panels/DelayPanel.cpp
    Source/UI/Panels/SendBusPanel.cpp
    Source/UI/Panels/LimiterPanel.cpp
    Source/UI/Components/EQCurveComponent.h
    Source/UI/Components/GlossyKnob.cpp
    Source/UI/Components/GlowButton.cpp
    Source/UI/Components/MacroKnob.cpp
    Source/UI/Components/PresetSelector.cpp
    Source/UI/Components/MeterComponent.cpp

    # Preset
    Source/Preset/PresetManager.cpp
)

target_include_directories(LVOX PRIVATE Source)

target_compile_definitions(LVOX PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(LVOX
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
