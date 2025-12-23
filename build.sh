rm -fr build
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/linux.cmake
cmake --build ./build 