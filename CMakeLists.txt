cmake_minimum_required(VERSION 3.28)

project(SuperGame2025)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR "C:\\Users\\st021513\\Downloads\\sfml\\lib\\cmake\\SFML")

find_package(SFML 2.6.2 COMPONENTS graphics audio REQUIRED)

add_executable(supergame src\\main.cpp src\\GameManager.cpp)
target_include_directories(supergame PRIVATE include)
target_compile_features(supergame PRIVATE cxx_std_17)
target_link_libraries(supergame PRIVATE sfml-graphics)
