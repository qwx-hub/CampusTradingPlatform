cmake_minimum_required(VERSION 3.14)
project(CampusTradingPlatform)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Windows 下的一个重要设置，防止编译报错
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# 开始下载并加载
FetchContent_MakeAvailable(googletest)

# ==========================================
# 第二部分：构建你的测试程序
# ==========================================

# -------------------------------------------------------
# 2. 定义核心逻辑库 (Core Library)
# 关键点：这里不包含 main.cpp，只包含业务逻辑文件
# -------------------------------------------------------
add_library(trading_core
    src/Platform.cpp
    src/User.cpp
    src/Item.cpp
    src/SearchEngine.cpp
)

# 指定头文件路径，方便 include
target_include_directories(trading_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

# -------------------------------------------------------
# 3. 定义主程序 (Main Application)
# 只有这个目标才包含 main.cpp
# -------------------------------------------------------
add_executable(TradingApp src/main.cpp)
target_link_libraries(TradingApp PRIVATE trading_core)

# -------------------------------------------------------
# 4. 定义单元测试 (Unit Tests)
# -------------------------------------------------------

enable_testing()


# 3. 启用测试发现功能（可选，方便集成）
# enable_testing()

add_executable(RunTests tests/TestPlatform.cpp)

# 将测试程序链接到 gtest_main (提供测试入口) 和 trading_core (你的业务代码)
target_link_libraries(RunTests PRIVATE gtest_main trading_core)

include(GoogleTest)
gtest_discover_tests(RunTests)