chcp 65001

cmake -B build -G "MinGW Makefiles"

cmake --build build

cd build

.\RunTests.exe