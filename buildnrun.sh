#!/bin/bash
if [ "$EUID" -eq 0 ]; then
  echo "ERR: do NOT run or build blurtnote as rooot"
  exit 1
fi

pkill -fe blurtnote
rm -rfv build
mkdir build

echo -e "+ installing build dependencies..."
(command -v apt >/dev/null 2>&1 && sudo apt update -qq >/dev/null 2>&1 && sudo apt install -yqq g++ cmake ninja-build qt6-base-dev qt6-tools-dev qt6-base-dev-tools >/dev/null 2>&1 || \
 command -v dnf >/dev/null 2>&1 && sudo dnf install -y gcc-c++ cmake ninja-build qt6-qtbase-devel qt6-qttools-devel >/dev/null 2>&1 || \
 command -v pacman >/dev/null 2>&1 && sudo pacman -S --noconfirm gcc cmake ninja qt6-base qt6-tools >/dev/null 2>&1 || \
 command -v zypper >/dev/null 2>&1 && sudo zypper install -y gcc-c++ cmake ninja qt6-base-devel qt6-tools-devel >/dev/null 2>&1 || \
 command -v apk >/dev/null 2>&1 && sudo apk add g++ cmake ninja qt6-qtbase-dev qt6-qttools-libs >/dev/null 2>&1)

echo -e '+ building blurtnote for linux'
cmake -B build -S src -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu
cmake --build build

echo -e '\n+ done building, executing\n'
./build/blurtnote & disown