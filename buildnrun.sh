mkdir build
cmake -B build -S . -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu
cmake --build build
./build/blurtnote
