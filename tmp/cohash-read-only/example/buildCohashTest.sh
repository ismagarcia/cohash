# Build for ARM 
rm -rf CMakeFiles/ Makefile CMakeCache.txt  cmake_install.cmake
cmake ./ -DCMAKE_TOOLCHAIN_FILE=./toolchain.cmake
make

# Build for X86_64
#rm -rf CMakeFiles/ Makefile CMakeCache.txt  cmake_install.cmake
#cmake ./
#make
