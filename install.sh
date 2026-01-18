#!/bin/bash
set -euo pipefail
DERP="anicient321/blurtnote"
BRANCH="1.1.0"
TMPDIR=$(mktemp -d)
ICONDIR="$HOME/.local/share/icons"
DESKTOPDIR="$HOME/.local/share/applications"
NOTESHASH="173d6c65927459e2d11ddb9ad1b3d245495f861e918a7a4949442ffdbd5971c4"

echo -e "blurtnote installer\n\n+ installing blurtnote for user $USER"

echo -e '+ need SU rights to install...\n'
sudo -v
echo "+ installing dependencies..."

install_deps() {
    if command -v apt >/dev/null 2>&1; then
        sudo apt update -yqq >/dev/null 2>>"$TMPDIR/err.log" || echo "apt update failed, see $TMPDIR/err.log"
        sudo apt install -yqq curl libqt6core6 libqt6gui6 libqt6widgets6 libqt6network6 >/dev/null 2>>"$TMPDIR/err.log" || echo "apt install failed, see $TMPDIR/err.log"
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y curl qt6-qtbase >/dev/null 2>>"$TMPDIR/err.log" || echo "dnf install failed, see $TMPDIR/err.log"
    elif command -v pacman >/dev/null 2>&1; then
        sudo pacman -Syu --noconfirm curl qt6-base >/dev/null 2>>"$TMPDIR/err.log" || echo "pacman install failed, see $TMPDIR/err.log"
    elif command -v zypper >/dev/null 2>&1; then
        sudo zypper install -y curl libQt6Core6 libQt6Gui6 libQt6Widgets6 >/dev/null 2>>"$TMPDIR/err.log" || echo "zypper install failed, see $TMPDIR/err.log"
    elif command -v apk >/dev/null 2>&1; then
        sudo apk add --no-cache curl qt6-qtbase >/dev/null 2>>"$TMPDIR/err.log" || echo "apk install failed, see $TMPDIR/err.log"
    else
        echo "- could not install dependencies\n- likely because the system is incompatible :( see $TMPDIR/err.log for details"
        exit 1
    fi
}
install_deps

trap 'rc=$?; rm -rf "$TMPDIR"; exit $rc' EXIT

download() {
    url=$1; dest=$2
    if command -v curl >/dev/null 2>&1; then
        curl -fsSL -o "$dest" "$url"
    elif command -v wget >/dev/null 2>&1; then
        wget -q -O "$dest" "$url"
    else
        echo "ERR: curl or wget required"
        exit 1
    fi
}

check_internet() {
    ping -c 1 -W 2 8.8.8.8 >/dev/null 2>&1
}

if [ -f build/blurtnote ]; then
    cp blurtnote blurtnote-x86_64
    mv blurtnote-x86_64 $TMPDIR
else
    check_internet || { echo "- an internet connection is required to download assets"; exit 1; }
    download "https://github.com/$DERP/releases/download/release-$BRANCH/blurtnote-x86_64" "$TMPDIR/blurtnote-x86_64"
fi

if [ ! -f icons/notes.svg ] || [ "$(sha256sum icons/notes.svg | awk '{print $1}')" != "$NOTESHASH" ]; then
    check_internet || { echo "- an internet connection is required to download assets"; exit 1; }
    download "https://raw.githubusercontent.com/$DERP/refs/heads/v$BRANCH/src/icons/notes.svg" "$TMPDIR/notes.svg"
fi

echo -e '+ copying blurtnote assets...'
cp "${ICONDIR}/notes.svg" "$ICONDIR/" 2>/dev/null || cp "$TMPDIR/notes.svg" "$ICONDIR/" 2>/dev/null || { echo "ERR: notes.svg is somehow still missing"; exit 1; }

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
sudo rm -rf /opt/blurtnote
sudo cp "$TMPDIR/blurtnote-x86_64" /opt/blurtnote
sudo chown $USER:$USER /opt/blurtnote
sudo chmod 755 /opt/blurtnote
sudo ln -sf /opt/blurtnote /usr/local/bin

if [[ ! -s "$TMPDIR/err.log" ]] || [[ -z $(grep -o '[^[:space:]]' "$TMPDIR/err.log") ]]; then
    rm -rf "$TMPDIR/err.log"
else
    echo -e "- installation completed, but something went wrong :(\nsee $TMPDIR/err.log for details"
fi

# it really does take a WHILE.
#echo -e '+ updating system icon and desktop caches...\nthis might take a while'
#command -v update-desktop-database >/dev/null 2>&1 && update-desktop-database "$DESKTOPDIR" >/dev/null 2>&1 || true
#command -v gtk-update-icon-cache >/dev/null 2>&1 && gtk-update-icon-cache -f -t "$(dirname "$ICONDIR")" >/dev/null 2>&1 || true

echo -e '+++ installed blurtnote!'
