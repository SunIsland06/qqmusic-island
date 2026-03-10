cmake_minimum_required(VERSION 3.20)
project(qqlyrics-backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core DBus WebSockets)

add_executable(qqlyrics
  src/main.cpp
  src/mpris_watcher.cpp
  src/lyrics_provider.cpp
  src/timeline_engine.cpp
  src/ipc_server.cpp
)

target_include_directories(qqlyrics PRIVATE src)
target_link_libraries(qqlyrics PRIVATE Qt6::Core Qt6::DBus Qt6::WebSockets)
