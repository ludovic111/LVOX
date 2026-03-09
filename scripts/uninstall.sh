#!/bin/bash
# LVOX Uninstaller

set -euo pipefail

echo "=== LVOX Uninstaller ==="
echo ""

# Unload LaunchAgent
PLIST="$HOME/Library/LaunchAgents/com.ludovicmarie.lvox.updater.plist"
if [ -f "$PLIST" ]; then
    echo "Unloading auto-updater..."
    launchctl bootout gui/$(id -u) "$PLIST" 2>/dev/null || true
    rm -f "$PLIST"
fi

# Remove updater script
if [ -f /usr/local/bin/lvox-update ]; then
    echo "Removing updater script..."
    sudo rm -f /usr/local/bin/lvox-update
fi

# Ask about plugins
read -p "Remove LVOX plugins? (y/N) " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Removing plugins..."
    rm -rf "$HOME/Library/Audio/Plug-Ins/VST3/LVOX.vst3"
    rm -rf "$HOME/Library/Audio/Plug-Ins/Components/LVOX.component"
fi

# Ask about presets
read -p "Remove user presets? (y/N) " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Removing presets..."
    rm -rf "$HOME/Library/Application Support/LVOX"
fi

# Clean up logs
rm -rf "$HOME/Library/Logs/LVOX"

echo ""
echo "LVOX uninstalled."
