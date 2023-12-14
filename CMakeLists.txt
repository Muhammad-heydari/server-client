cmake_minimum_required(VERSION 3.5)

project(T3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5 REQUIRED COMPONENTS Network)

add_executable(T3
  main.cpp
  src/MyClient.cpp
  src/MyServer.cpp

  include/MyClient.h
  include/MyServer.h


  include/MyRunnable.h
  src/MyRunnable.cpp
)
target_link_libraries(T3 Qt5::Core Qt5::Network)

