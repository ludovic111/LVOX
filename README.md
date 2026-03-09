# LVOX

A professional vocal mixing plugin for **FL Studio** (VST3) and **Logic Pro** (AU).

LVOX handles the full vocal processing chain — everything from noise gate to limiter — in a single plugin with a sleek **Frutiger Aero** interface.

## Install (Pre-Built)

### One-Line Install

```bash
curl -sL https://raw.githubusercontent.com/ludovic111/LVOX/master/scripts/install-remote.sh | bash
```

This downloads the latest universal binary (Intel + Apple Silicon), installs it, and sets up hourly auto-updates.

### Manual Download

Download the latest release from the [Releases page](https://github.com/ludovic111/LVOX/releases/latest), unzip, and copy:

- `LVOX.vst3` → `~/Library/Audio/Plug-Ins/VST3/`
- `LVOX.component` → `~/Library/Audio/Plug-Ins/Components/`

Then remove quarantine: `xattr -cr ~/Library/Audio/Plug-Ins/VST3/LVOX.vst3`

## Auto-Updates

The installer sets up a LaunchAgent that checks for new versions every hour. When an update is found, it downloads and installs automatically (only when your DAW isn't using the plugin).

- **Logs:** `~/Library/Logs/LVOX/update.log`
- **Check manually:** `/usr/local/bin/lvox-update`
- **Disable:** `launchctl bootout gui/$(id -u) ~/Library/LaunchAgents/com.ludovicmarie.lvox.updater.plist`
- **Uninstall:** `curl -sL https://raw.githubusercontent.com/ludovic111/LVOX/master/scripts/uninstall.sh | bash`

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
- **Simple Mode** — 4 macro knobs (Warmth, Presence, Compression, Space) + one-click presets
- **Advanced Mode** — full control over every parameter with per-module bypass

### Mic Selection
Built-in mic correction profiles that adjust DSP parameters for specific microphones:
- **UAD Sphere LX (C800 sim)** — HPF, de-esser, EQ, compressor, and saturation offsets
- **Shure MV7** — de-esser, EQ, compressor, and saturation offsets

### 26 Factory Presets

**Generic:** Podcast, Rap Vocal, Singing Lead, Background Vocal, Radio Voice, Lo-Fi Vocal, Bright Pop, Intimate ASMR, Aggressive Rock, Choir Stack, Telephone, Dreamy Ethereal, RnB Smooth

**Artist:** Drake, Travis Scott, The Weeknd, Billie Eilish, Adele, Beyonce, Kanye West, Post Malone, Frank Ocean, Ariana Grande, Juice WRLD, 21 Savage, Tyler The Creator

### Send/Bus Parallel Routing
Route reverb and delay as parallel sends with independent level control, instead of inline processing.

### Frutiger Aero Design
Glossy translucent panels, aqua/teal colour palette, color-coded module panels, glass-like knobs with radial gradients and glow, pill switch toggles, stereo meters with peak hold.

## Build from Source

Requires **CMake 3.22+** and **Xcode** on macOS.

```bash
# Clone with submodules
git clone --recursive https://github.com/ludovic111/LVOX.git
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
