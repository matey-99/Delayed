cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(PBL VERSION 0.1)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Engine/ThirdParty")
set(EDITOR_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Editor/ThirdParty")

# add thirdparties
include(Engine/ThirdParty/thirdparty.cmake)
include(Editor/ThirdParty/thirdparty.cmake)

# subdirectories
add_subdirectory(Engine/Source)
add_subdirectory(Editor/Source)