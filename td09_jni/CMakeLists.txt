cmake_minimum_required(VERSION 3.20)
project(TD9_JNI)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 14)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)
include_directories(${JNI_INCLUDE_DIRS})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(HelloWorld.class COMMAND ${Java_JAVAC_EXECUTABLE} -h ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/HelloWorld.java)

add_library(HelloWorld SHARED HelloWorld.cpp)

add_dependencies(HelloWorld HelloWorld.class)
