cmake_minimum_required(VERSION 3.28)

project(SuperGame2025)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_DIR "C:\\Users\\st021513\\Downloads\\sfml\\lib\\cmake\\SFML")

find_package(SFML 2.6.2 COMPONENTS graphics audio REQUIRED)

add_executable(supergame src\\main.cpp src\\GameManager.cpp)
target_include_directories(supergame PRIVATE include)
target_compile_features(supergame PRIVATE cxx_std_17)
target_link_libraries(supergame PRIVATE sfml-graphics)

# Tests
enable_testing()

add_executable(
  dummy_test
  tests\\dummy.cpp
)
target_link_directories(dummy_test PRIVATE "C:\\Users\\st021513\\repos\\googletest\\build\\lib\\Debug")
target_include_directories(dummy_test PRIVATE "C:\\Users\\st021513\\repos\\googletest\\googletest\\include")

target_link_libraries(
  dummy_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(dummy_test)
