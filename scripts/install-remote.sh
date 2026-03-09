#!/bin/bash
# LVOX Remote Installer
# Usage: curl -sL https://raw.githubusercontent.com/ludovic111/LVOX/master/scripts/install-remote.sh | bash

set -euo pipefail

REPO="ludovic111/LVOX"
BRANCH="master"
BASE_URL="https://raw.githubusercontent.com/$REPO/$BRANCH/scripts"

echo "=== LVOX Remote Installer ==="
echo ""

TMPDIR_INSTALL=$(mktemp -d)
trap "rm -rf $TMPDIR_INSTALL" EXIT

# Download scripts
echo "Downloading installer scripts..."
curl -sf "$BASE_URL/lvox-update.sh" -o "$TMPDIR_INSTALL/lvox-update.sh"
curl -sf "$BASE_URL/com.ludovicmarie.lvox.updater.plist" -o "$TMPDIR_INSTALL/com.ludovicmarie.lvox.updater.plist"

# Install updater script
echo "Installing updater to /usr/local/bin/lvox-update..."
sudo cp "$TMPDIR_INSTALL/lvox-update.sh" /usr/local/bin/lvox-update
sudo chmod +x /usr/local/bin/lvox-update

# Run updater once to download latest release
echo "Downloading latest LVOX release..."
/usr/local/bin/lvox-update

# Install LaunchAgent
PLIST_DST="$HOME/Library/LaunchAgents/com.ludovicmarie.lvox.updater.plist"
mkdir -p "$HOME/Library/LaunchAgents"
cp "$TMPDIR_INSTALL/com.ludovicmarie.lvox.updater.plist" "$PLIST_DST"

# Load LaunchAgent
launchctl bootout gui/$(id -u) "$PLIST_DST" 2>/dev/null || true
launchctl bootstrap gui/$(id -u) "$PLIST_DST"

echo ""
echo "Done! LVOX is installed and will auto-update hourly."
echo "  VST3: ~/Library/Audio/Plug-Ins/VST3/LVOX.vst3"
echo "  AU:   ~/Library/Audio/Plug-Ins/Components/LVOX.component"
echo "  Logs: ~/Library/Logs/LVOX/update.log"
