cmake_minimum_required(VERSION 3.20)
project(qqmusic-island LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core DBus)

add_executable(qqmusic-island
  src/main.cpp
  src/mpris_watcher.cpp
)

target_include_directories(qqmusic-island PRIVATE src)
target_link_libraries(qqmusic-island PRIVATE Qt6::Core Qt6::DBus)
