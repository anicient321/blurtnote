# BlurtNote
**BlurtNote** is a quick-capture application for dumping thoughts instantly.
![BlurtNote Banner](./image.png)

It's cross-platform and developed in **C++ using the Qt framework**. The application is designed to run on both **Linux and Windows** operating systems.

The project focuses on building a minimal and functional text editor with a graphical user interface, basic file handling, and theme customization. 

## Description
BlurtNote is a minimal note-taking app designed for speed. Capture fleeting thoughts without friction.

## Features
- Fast thought capture

## Compilation & installation
You can also use the pre-build binaries for both platforms from the Releases tab.

### Windows
```bash
git clone https://github.com/anicient321/blurtnote.git
cd blurtnote
build.bat
install.bat
```

### Linux
```bash
git clone https://github.com/anicient321/blurtnote.git
cd blurtnote
mkdir build
cmake -B build -S . -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu
cmake --build build
cd build && ./blurtnote
```
You can use the convenience script in the root directory (```./buildnrun.sh```) to compile & run BlurtNote as needed
```bash
chmod +x buildnrun.sh
./buildnrun.sh
```
And to install, you can use the (```./install.sh```) script

## Usage
```bash
blurtnote
```

## License
GPL2.0, refer to LICENSES\GPL2.0
