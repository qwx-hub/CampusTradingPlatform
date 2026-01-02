单元测试：
cmake -B build -G "MinGW Makefiles"

cmake --build build

cd build

.\RunTests.exe

CI配置：

git init

git add .

git commit -m "Add CI workflow and Unit Tests"

git branch -M main

git push origin main
