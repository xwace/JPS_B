cmake_minimum_required(VERSION 3.15)

#set(CMAKE_C_COMPILER "/usr/local/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc")
#set(CMAKE_CXX_COMPILER "/usr/local/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++")

project(JPS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

find_package(Threads REQUIRED)

include_directories(./src)

aux_source_directory(${PROJECT_SOURCE_DIR}/src FILES)

message("file: ${FILES}")

add_executable(JPS ${FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS} Threads::Threads)
