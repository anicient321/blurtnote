#!/bin/bash
if [ -f build/blurtnote ]; then
    echo -e '+ Copying BlurtNote Assets...'
    cp -r icons/notes.svg $HOME/.local/share/icons/blurtnote/notes.svg
    cp -rv blurtnote.desktop $HOME/.local/share/applications
    echo -e '+ Need SU rights to install...'
    sudo -v
    sudo cp -r build/blurtnote /opt/blurtnote
    mkdir -p $HOME/.local/share/icons/blurtnote
    echo -e '+++ Installed BlurtNote to disk'
else
    echo "ERR: Binary does not exist, run buildncompile.sh"
    exit 1
fi
