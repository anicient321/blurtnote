# BlurtNote
**BlurtNote** is a quick-capture application for dumping thoughts instantly.
![BlurtNote Banner](./assets/image.png)

It's cross-platform and developed in **C++ using the Qt framework**. The application is designed to run on both **Linux and Windows** operating systems.

The project focuses on building a minimal and functional text editor with a graphical user interface, basic file handling, and theme customization. 

## Description
BlurtNote is a minimal note-taking app designed for speed. Capture fleeting thoughts without friction.

## Features
- Fast thought capture

## Compilation & installation
You can also use the pre-build binaries for both platforms from the Releases tab.
If you are building anything Windows related - make sure you have (```MinGW-w64```) installed!

### Windows
```bash
git clone https://github.com/anicient321/blurtnote.git
cd blurtnote
mkdir build
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
cd build
blurtnote.exe
```

### Linux
```bash
git clone https://github.com/anicient321/blurtnote.git
cd blurtnote
mkdir build
cmake -B build -S . -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu
cmake --build build
cd build
./blurtnote
```
You can use the convenience script in the root directory (```./buildnrun.sh```) to compile & run BlurtNote as needed

NOTE: buildnrun.sh and install.sh also download dependencies, except MinGW-w64, as the script does not yet support building Windows binaries
```bash
chmod +x buildnrun.sh
./buildnrun.sh
```
Alternatively, to install without building, you can use the (```./install.sh```) script
or, use the oneliner
```bash
curl -fsSL https://raw.githubusercontent.com/anicient321/blurtnote/refs/heads/v1.1.0/install.sh | bash
```
```bash
chmod +x install.sh
./install.sh
```
And you can uninstall via the (```./uninstall.sh```) script

## Usage
```bash
blurtnote
```

## Basic troubleshooting


## License
GPL2.0, refer to LICENSES\GPL2.0
