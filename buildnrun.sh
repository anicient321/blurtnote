#!/bin/bash
if [ "$EUID" -eq 0 ]; then
  echo "ERR: do NOT run or build blurtnote as rooot"
  exit 1
fi

pkill -fe blurtnote
rm -rfv build
mkdir build

#!/bin/bash

echo -e "+ installing build dependencies..."
if command -v apt >/dev/null 2>&1; then
    sudo apt update -qq >/dev/null 2>>"err.log" || echo "apt update failed, see err.log"
    sudo apt install -yqq g++ cmake qt6-base-dev qt6-tools-dev qt6-base-dev-tools >/dev/null 2>>"err.log" || echo "apt install failed, see err.log"
elif command -v dnf >/dev/null 2>&1; then
    sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel >/dev/null 2>>"err.log" || echo "dnf install failed, see err.log"
elif command -v pacman >/dev/null 2>&1; then
    sudo pacman -S --noconfirm gcc cmake qt6-base qt6-tools >/dev/null 2>>"err.log" || echo "pacman install failed, see err.log"
elif command -v zypper >/dev/null 2>&1; then
    sudo zypper install -y gcc-c++ cmake qt6-base-devel qt6-tools-devel >/dev/null 2>>"err.log" || echo "zypper install failed, see err.log"
elif command -v apk >/dev/null 2>&1; then
    sudo apk add g++ cmake qt6-qtbase-dev qt6-qttools-libs >/dev/null 2>>"err.log" || echo "apk install failed, see err.log"
else
    echo "- something went wrong :( see $PWD/err.log for details"
    exit 1
fi

echo -e '+ building blurtnote for linux'
cmake -B build -S src -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu
cmake --build build

echo -e '\n+ done building, executing\n'
./build/blurtnote & disown
