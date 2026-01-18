#!/bin/bash
ICONDIR="$HOME/.local/share/icons"
DESKTOPDIR="$HOME/.local/share/applications"

echo -e "blurtnote UNinstaller\n\n+ uninstalling blurtnote from user $USER"

echo -e '+ need SU rights to uninstall...\n'
sudo -v
sudo rm -rfv "$ICONDIR/notes.svg"
sudo rm -rfv "$DESKTOPDIR/blurtnote.desktop"
sudo rm -rfv /opt/blurtnote
echo -e '+++ uninstalled blurtnote!'
