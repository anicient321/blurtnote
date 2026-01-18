#!/bin/bash
set -euo pipefail
DERP="anicient321/blurtnote"
BRANCH="v1.1.0"
TMPDIR=$(mktemp -d)
ICONDIR="$HOME/.local/share/icons"
DESKTOPDIR="$HOME/.local/share/applications"
NOTESHASH="173d6c65927459e2d11ddb9ad1b3d245495f861e918a7a4949442ffdbd5971c4"

echo -e "blurtnote installer\n\n+ installing blurtnote for user $USER"

echo -e "+ installing dependencies..."
(command -v apt >/dev/null 2>&1 && sudo apt update -qq >/dev/null 2>&1 && sudo apt install -yqq curl libqt6core6 libqt6gui6 libqt6widgets6 libqt6network6 >/dev/null 2>&1 || \
 command -v dnf >/dev/null 2>&1 && sudo dnf install -y curl qt6-qtbase >/dev/null 2>&1 || \
 command -v pacman >/dev/null 2>&1 && sudo pacman -S --noconfirm curl qt6-base >/dev/null 2>&1 || \
 command -v zypper >/dev/null 2>&1 && sudo zypper install -y curl libQt6Core6 libQt6Gui6 libQt6Widgets6 >/dev/null 2>&1 || \
 command -v apk >/dev/null 2>&1 && sudo apk add curl qt6-qtbase >/dev/null 2>&1)

trap 'rc=$?; rm -rf "$TMPDIR"; exit $rc' EXIT
if command -v curl >/dev/null 2>&1; then
    curl -fsSL -o "$TMPDIR/blurtnote-x86_64" https://github.com/$DERP/releases/download/release-v$BRANCH/blurtnote-x86_64
elif command -v wget >/dev/null 2>&1; then
    wget -q -O "$TMPDIR/blurtnote-x86_64" https://github.com/$DERP/releases/download/release-v$BRANCH/blurtnote-x86_64
else
    echo "ERR: curl or wget required"
    exit 1
fi

if [ -f icons/notes.svg ] && \
   [ "$(sha256sum icons/notes.svg | awk '{print $1}')" = "$NOTESHASH" ]; then
    :
else
    if command -v curl >/dev/null 2>&1; then
        curl -fsSL -o "$TMPDIR/notes.svg" "https://raw.githubusercontent.com/$DERP/refs/heads/$BRANCH/src/icons/notes.svg"
    elif command -v wget >/dev/null 2>&1; then
        wget -q -O "$TMPDIR/notes.svg" "https://raw.githubusercontent.com/$DERP/refs/heads/$BRANCH/src/icons/notes.svg"
    else
        echo "ERR: curl or wget required"
        exit 1
    fi
fi

echo -e '+ copying blurtnote assets...'

if [ -f icons/notes.svg ]; then
    cp "icons/notes.svg" "$ICONDIR/"
elif [ -f "$TMPDIR/notes.svg" ]; then
    cp "$TMPDIR/notes.svg" "$ICONDIR/"
else
    echo "ERR: notes.svg is somehow still missing"
    exit 1
fi

cat > "$TMPDIR/blurtnote.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=Blurtnote
Comment=Dump your thoughts instantly
Exec=/opt/blurtnote
Icon=$ICONDIR/notes.svg
Terminal=false
Categories=Utility;
EOF

chmod +x "$TMPDIR/blurtnote.desktop"
cp "$TMPDIR/blurtnote.desktop" "$DESKTOPDIR/"

echo -e '+ need SU rights to install...\n'
sudo -v
sudo rm -rf /opt/blurtnote
sudo cp "$TMPDIR/blurtnote-x86_64" /opt/blurtnote
sudo chown $USER:$USER /opt/blurtnote
sudo chmod 755 /opt/blurtnote

#it really does take a while... a WHILE.
#echo -e '+ updating system icon and desktop caches...\nthis might take a while'
#if command -v update-desktop-database >/dev/null 2>&1; then
#    update-desktop-database "$DESKTOPDIR" >/dev/null 2>&1 || true
#fi
#if command -v gtk-update-icon-cache >/dev/null 2>&1; then
#    gtk-update-icon-cache -f -t "$(dirname "$ICONDIR")" >/dev/null 2>&1 || true
#fi

echo -e '+++ installed blurtnote!'