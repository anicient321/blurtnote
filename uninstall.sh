#!/bin/bash
ICONDIR="$HOME/.local/share/icons"
DESKTOPDIR="$HOME/.local/share/applications"

echo -e "blurtnote UNinstaller\n\n+ uninstalling blurtnote from user $USER"

echo -e '+ need SU rights to uninstall...\n'
sudo -v
pkill -f blurtnote
sudo rm -rf "$ICONDIR/notes.svg"
sudo rm -rf "$DESKTOPDIR/blurtnote.desktop"
sudo rm -rf /opt/blurtnote
sudo rm -rf /usr/local/bin/blurtnote
echo -e '+++ uninstalled blurtnote!'
