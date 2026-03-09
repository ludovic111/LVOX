#!/bin/bash
# LVOX Installer
# Installs the auto-updater and downloads the latest LVOX release.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "=== LVOX Installer ==="
echo ""

# Install updater script
echo "Installing updater to /usr/local/bin/lvox-update..."
sudo cp "$SCRIPT_DIR/lvox-update.sh" /usr/local/bin/lvox-update
sudo chmod +x /usr/local/bin/lvox-update

# Run updater once to download latest release
echo "Downloading latest LVOX release..."
/usr/local/bin/lvox-update

# Install LaunchAgent
PLIST_DST="$HOME/Library/LaunchAgents/com.ludovicmarie.lvox.updater.plist"
mkdir -p "$HOME/Library/LaunchAgents"
cp "$SCRIPT_DIR/com.ludovicmarie.lvox.updater.plist" "$PLIST_DST"

# Load LaunchAgent
launchctl bootout gui/$(id -u) "$PLIST_DST" 2>/dev/null || true
launchctl bootstrap gui/$(id -u) "$PLIST_DST"

echo ""
echo "Done! LVOX is installed and will auto-update hourly."
echo "  VST3: ~/Library/Audio/Plug-Ins/VST3/LVOX.vst3"
echo "  AU:   ~/Library/Audio/Plug-Ins/Components/LVOX.component"
echo "  Logs: ~/Library/Logs/LVOX/update.log"
