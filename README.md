# LVOX

A professional vocal mixing plugin for **FL Studio** and **Logic Pro**.

LVOX handles the full vocal processing chain — everything from noise gate to limiter — in a single plugin with a sleek **Frutiger Aero** interface.

## Features

### DSP Processing Chain
- **Noise Gate** — threshold, ratio, attack, release, range
- **High-Pass Filter** — 12/24/48 dB/oct slope
- **De-Esser** — frequency-selective compression with listen mode
- **4-Band Parametric EQ** — peak, low shelf, high shelf per band
- **Compressor** — sidechain HPF, soft knee, parallel compression (dry/wet mix)
- **Saturation** — Tape / Tube / Soft Clip / Hard Clip with 2x oversampling
- **Reverb** — room size, damping, pre-delay, lo/hi cut
- **Delay** — free or tempo-synced, filtered feedback
- **Output Limiter** — brickwall ceiling

### Two UI Modes
- **Simple Mode** — 4 macro knobs (Warmth, Presence, Compression, Space) + one-click voice presets
- **Advanced Mode** — full control over every parameter with per-module bypass

### Factory Presets
- Podcast
- Rap Vocal
- Singing Lead
- Background Vocal
- Radio Voice

### Frutiger Aero Design
Glossy translucent panels, aqua/teal colour palette, glass-like knobs with specular highlights, soft glow effects.

## Build

Requires **CMake 3.22+** and **Xcode** on macOS.

```bash
# Clone with submodules
git clone --recursive https://github.com/ludovicmarie/LVOX.git
cd LVOX

# Configure and build
cmake -B build -G Xcode
cmake --build build --config Release

# Universal binary (Intel + Apple Silicon)
cmake -B build -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build --config Release
```

Plugins auto-install to:
- **VST3**: `~/Library/Audio/Plug-Ins/VST3/LVOX.vst3`
- **AU**: `~/Library/Audio/Plug-Ins/Components/LVOX.component`

## Tech Stack
- **JUCE** — cross-platform audio plugin framework
- **C++20**
- **CMake** with Xcode generator
- Formats: VST3, AU, Standalone

## License
All rights reserved.
