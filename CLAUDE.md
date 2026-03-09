# LVOX — Claude Context

## What is LVOX?
A professional vocal mixing VST3/AU plugin built with **JUCE** (C++20) and **CMake**. It handles the full vocal processing chain (everything except autotune) and targets FL Studio (VST3) and Logic Pro (AU) on macOS.

## Build Instructions
```bash
# Configure (must use Xcode generator — Unix Makefiles has issues on this system)
cmake -B build -G Xcode

# Build Debug
cmake --build build --config Debug

# Build Release
cmake --build build --config Release

# Universal binary (Intel + Apple Silicon)
cmake -B build -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
```

Artifacts land in `build/LVOX_artefacts/{Debug,Release}/{VST3,AU,Standalone}/`.
With `COPY_PLUGIN_AFTER_BUILD TRUE`, they auto-install to `~/Library/Audio/Plug-Ins/`.

## Architecture

### DSP Chain (9 modules in series, each bypassable)
```
Inline mode (default):
Input Gain → NoiseGate → HPF → DeEsser → ParametricEQ(4-band)
  → Compressor → Saturation(2x oversampled) → Reverb → Delay → Limiter → Output Gain

Send mode (parallel buses):
Input Gain → NoiseGate → HPF → DeEsser → ParametricEQ(4-band)
  → Compressor → Saturation ──┬──────────────────→ Limiter → Output Gain
                               ├── Send → Rev Bus ↗
                               └── Send → Dly Bus ↗
```

All modules inherit from `DSPModule` (Source/DSP/DSPModule.h) which provides:
- `prepare(ProcessSpec)`, `process(AudioBlock)`, `reset()`
- `isBypassed()` via atomic param read
- `getParam(id)` helper to get `std::atomic<float>*` from APVTS

`DSPChain` (Source/DSP/DSPChain.h) owns all 9 modules and sequences them in `processBlock`.

### Parameter System
- ~93 parameters defined in `Source/Parameters.h` (IDs as `constexpr const char*`) and `Source/Parameters.cpp` (`createParameterLayout()`)
- All parameters use `juce::AudioProcessorValueTreeState` (APVTS)
- State saved/restored as XML in `PluginProcessor::getStateInformation`/`setStateInformation`
- Param ID convention: `module_param` (e.g. `comp_threshold`, `eq_band1_freq`, `rev_mix`, `send_rev_level`)

### UI
- **Simple Mode** (900x500): 4 macro knobs (Warmth, Presence, Compression, Space) that map to underlying DSP params. Macro mapping runs on a 15Hz timer in `SimpleModePanel::applyMacros()`.
- **Advanced Mode** (1100x700): Scrollable viewport with 9 `ModulePanel` subclasses, each with `GlossyKnob`/`GlowButton` components attached to APVTS via `SliderAttachment`/`ButtonAttachment`.
- **Frutiger Aero** styling: `FrutigerAeroLookAndFeel` (LookAndFeel_V4 subclass), colour constants in `ColourPalette.h`, gradient helpers in `GradientUtils.h`.

### Preset System
- `PresetManager` (Source/Preset/PresetManager.h): saves/loads XML presets
- Factory presets: Podcast, Rap Vocal, Singing Lead, Background Vocal, Radio Voice, Lo-Fi Vocal, Bright Pop, Intimate ASMR, Aggressive Rock, Choir Stack, Telephone, Dreamy Ethereal, RnB Smooth
- User presets directory: `~/Library/Application Support/LVOX/Presets/`

## Key Files
| File | Purpose |
|---|---|
| `CMakeLists.txt` | Plugin target, JUCE modules, build config |
| `Source/Parameters.h/.cpp` | All param IDs + createParameterLayout() |
| `Source/PluginProcessor.h/.cpp` | AudioProcessor: owns APVTS, DSPChain, PresetManager |
| `Source/PluginEditor.h/.cpp` | Editor: mode toggle, preset selector, meters |
| `Source/DSP/DSPModule.h` | Abstract base for all DSP modules |
| `Source/DSP/DSPChain.h/.cpp` | Sequences all 9 modules |
| `Source/DSP/*.h/.cpp` | Individual DSP modules (NoiseGate, HPF, DeEsser, ParametricEQ, Compressor, Saturation, LVOXReverb, Delay, Limiter) |
| `Source/UI/FrutigerAeroLookAndFeel.h/.cpp` | Custom Frutiger Aero LookAndFeel |
| `Source/UI/Helpers/ColourPalette.h` | Colour constants (aqua, teal, glow, glass) |
| `Source/UI/SimpleModePanel.h/.cpp` | Simple mode with macro knobs |
| `Source/UI/AdvancedModePanel.h/.cpp` | Advanced mode with scrollable module panels |
| `Source/UI/ModulePanel.h/.cpp` | Base class for per-module panels |
| `Source/UI/Panels/*.h/.cpp` | 10 concrete module panel UIs (9 DSP + SendBus) |
| `Source/UI/Components/*.h/.cpp` | GlossyKnob, GlowButton, MacroKnob, PresetSelector, MeterComponent |
| `Source/Preset/PresetManager.h/.cpp` | Preset save/load/factory creation |

## Macro → Parameter Mapping (Simple Mode)
- **Warmth** → sat_drive, sat_mix, eq_band1_gain (low shelf boost), eq_band4_gain (high shelf cut)
- **Presence** → eq_band3_gain (hi-mid boost), eq_band3_q, sat_tone
- **Compression** → comp_threshold, comp_ratio, comp_attack, comp_release, comp_makeup
- **Space** → rev_mix, rev_size, rev_predelay, dly_mix, dly_feedback, dly_time

## Conventions
- JUCE is a git submodule in `./JUCE/`
- C++20 standard
- No `using namespace juce;` — always prefix with `juce::`
- DSP modules read params via `std::atomic<float>*` obtained at construction time
- UI components use APVTS attachments (SliderAttachment, ButtonAttachment, ComboBoxAttachment)
- Reverb source file is named `LVOXReverb.h/.cpp` to avoid conflicts with JUCE's Reverb class

## Testing
- AU validation: `auval -v aufx Lvox Lvmc`
- Standalone app: `open build/LVOX_artefacts/Debug/Standalone/LVOX.app`
- DAW: load VST3 in FL Studio, AU in Logic Pro

## Known Issues / Future Work
- Parameter smoothing (SmoothedValue) not yet applied — may cause clicks on fast param changes
- EQ panel doesn't have frequency response curve visualization yet
- Compressor panel doesn't show gain reduction meter yet
- Delay feedback filters are declared but not fully integrated in the feedback path
- No universal binary build tested yet (needs `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"`)
- Factory presets are created on first run by setting params and saving — ideally should be embedded as binary data
