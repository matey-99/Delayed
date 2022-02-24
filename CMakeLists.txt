cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(PBL VERSION 0.1)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Engine/thirdparty")

# add thirdparties
include(Engine/thirdparty/thirdparty.cmake)

# subdirectories
add_subdirectory(Engine/src)
add_subdirectory(Editor/src)