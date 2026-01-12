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
cmake -B build -S . -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu
cmake --build build
mkdir build
cd build
./blurtnote
```

## Usage
Note; the built binary will be located in the "build" folder
```bash
./blurtnote
```

## License
GPL2.0, refer to LICENSES\GPL2.0
