# Root CMake file that configures the project

cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")

project(SandboxGame)

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(SandboxGame ${CMAKE_SOURCE_DIR}/Source/Game/main.cpp)
target_include_directories(SandboxGame PRIVATE ${CMAKE_SOURCE_DIR}/Packages/Vendor/include)
target_link_libraries(SandboxGame PRIVATE glfw)
target_link_libraries(SandboxGame PRIVATE imgui::imgui)