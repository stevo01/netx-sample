rm -fr build
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/linux.cmake
cmake --build ./build
# sudo setcap cap_net_raw,cap_net_admin+eip ./build/sample-net-app 
echo "start app: ./build/sample-net-app"
