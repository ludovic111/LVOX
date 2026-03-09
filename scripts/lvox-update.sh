#!/bin/bash
# LVOX Auto-Updater
# Checks GitHub Releases for new versions and installs them.

set -euo pipefail

REPO="ludovic111/LVOX"
VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
AU_DIR="$HOME/Library/Audio/Plug-Ins/Components"
LOG_DIR="$HOME/Library/Logs/LVOX"
LOG_FILE="$LOG_DIR/update.log"

mkdir -p "$LOG_DIR"

log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') $1" >> "$LOG_FILE"
}

cleanup() {
    [ -n "${TMPDIR_WORK:-}" ] && rm -rf "$TMPDIR_WORK"
}
trap cleanup EXIT

# Get installed version from Info.plist
INSTALLED_VERSION="0.0.0"
PLIST="$VST3_DIR/LVOX.vst3/Contents/Info.plist"
if [ -f "$PLIST" ]; then
    INSTALLED_VERSION=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" "$PLIST" 2>/dev/null || echo "0.0.0")
fi

# Query latest release
RELEASE_JSON=$(curl -sf "https://api.github.com/repos/$REPO/releases/latest" 2>/dev/null) || {
    log "ERROR: Failed to fetch release info"
    exit 1
}

# Parse tag and asset URLs
read -r LATEST_TAG VST3_URL AU_URL <<< $(python3 -c "
import json, sys
data = json.loads(sys.stdin.read())
tag = data['tag_name'].lstrip('v')
vst3 = au = ''
for a in data.get('assets', []):
    if 'VST3' in a['name']:
        vst3 = a['browser_download_url']
    elif 'AU' in a['name']:
        au = a['browser_download_url']
print(tag, vst3, au)
" <<< "$RELEASE_JSON")

if [ -z "$LATEST_TAG" ] || [ -z "$VST3_URL" ]; then
    log "ERROR: Could not parse release data"
    exit 1
fi

# Compare versions
if [ "$INSTALLED_VERSION" = "$LATEST_TAG" ]; then
    log "Up to date (v$INSTALLED_VERSION)"
    exit 0
fi

log "Update available: v$INSTALLED_VERSION -> v$LATEST_TAG"

# Check if plugins are in use
if lsof "$VST3_DIR/LVOX.vst3" &>/dev/null || lsof "$AU_DIR/LVOX.component" &>/dev/null; then
    log "ERROR: Plugin files are in use by a DAW. Close your DAW and try again."
    exit 1
fi

# Download and install
TMPDIR_WORK=$(mktemp -d)

log "Downloading VST3..."
curl -sL "$VST3_URL" -o "$TMPDIR_WORK/vst3.zip"

if [ -n "$AU_URL" ]; then
    log "Downloading AU..."
    curl -sL "$AU_URL" -o "$TMPDIR_WORK/au.zip"
fi

# Install VST3
mkdir -p "$VST3_DIR"
rm -rf "$VST3_DIR/LVOX.vst3"
unzip -qo "$TMPDIR_WORK/vst3.zip" -d "$VST3_DIR/"
xattr -cr "$VST3_DIR/LVOX.vst3" 2>/dev/null || true

# Install AU
if [ -f "$TMPDIR_WORK/au.zip" ]; then
    mkdir -p "$AU_DIR"
    rm -rf "$AU_DIR/LVOX.component"
    unzip -qo "$TMPDIR_WORK/au.zip" -d "$AU_DIR/"
    xattr -cr "$AU_DIR/LVOX.component" 2>/dev/null || true
fi

log "Updated to v$LATEST_TAG"
echo "LVOX updated to v$LATEST_TAG"
