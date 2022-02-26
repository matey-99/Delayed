# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARY ${OPENGL_LIBRARIES})

# assimp
find_library(ASSIMP_LIBRARY "assimp" "/usr/lib" "/usr/local/lib")
find_path(ASSIMP_INCLUDE_DIR "assimp/mesh.h" "/usr/include" "/usr/local/include")

if((NOT ASSIMP_LIBRARY) OR (NOT ASSIMP_INCLUDE_DIR))
	set(ASSIMP_DIR "${THIRDPARTY_DIR}/assimp")
	
	message("Unable to find assimp, cloning...")
    execute_process(COMMAND git submodule update --init ${ASSIMP_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build package with shared libraries.")
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "If the supplementary tools for Assimp are built in addition to the library.")
	set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")

    add_subdirectory("${ASSIMP_DIR}")

	set(ASSIMP_LIBRARY "assimp")
	set(ASSIMP_INCLUDE_DIR "${ASSIMP_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/assimp/include")
endif()

set(CMAKE_DEBUG_POSTFIX "")

# glfw
find_library(GLFW_LIBRARY "glfw" "/usr/lib" "/usr/local/lib")
find_path(GLFW_INCLUDE_DIR "glfw/glfw.h" "/usr/include" "/usr/local/include")

if((NOT GLFW_LIBRARY) OR (NOT GLFW_INCLUDE_DIR))
	set(GLFW_DIR "${THIRDPARTY_DIR}/glfw")

	message("Unable to find glfw, cloning...")
    execute_process(COMMAND git submodule update --init ${GLFW_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
	set(GLFW_INSTALL        OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory("${GLFW_DIR}")

	set(GLFW_LIBRARY "glfw" "${GLFW_LIBRARIES}")
	set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
endif()

# glad
set(GLAD_DIR "${THIRDPARTY_DIR}/glad")
add_library("glad" STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

set(GLAD_LIBRARY "glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

# glm
set(GLM_DIR "${THIRDPARTY_DIR}/glm")
execute_process(COMMAND git submodule update --init ${GLM_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(GLM_INCLUDE_DIR "${GLM_DIR}")

# stb_image
set(STB_IMAGE_DIR "${THIRDPARTY_DIR}/stb_image")
add_library("stb_image" STATIC "${STB_IMAGE_DIR}/stb_image.cpp")
target_include_directories("stb_image" PRIVATE "${STB_IMAGE_DIR}")

set(STB_IMAGE_LIBRARY "stb_image")
set(STB_IMAGE_INCLUDE_DIR "${STB_IMAGE_DIR}")

# yaml-cpp
find_library(YAML_CPP_LIBRARY "yaml-cpp" "/usr/lib" "/usr/local/lib")
find_path(YAML_CPP_INCLUDE_DIR "yaml-cpp/yaml.h" "/usr/include" "/usr/local/include")

if((NOT YAML_CPP_LIBRARY) OR (NOT YAML_CPP_INCLUDE_DIR))
	set(YAML_CPP_DIR "${THIRDPARTY_DIR}/yaml-cpp")

	message("Unable to find yaml-cpp, cloning...")
    execute_process(COMMAND git submodule update --init ${YAML_CPP_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(YAML_CPP_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the yaml-cpp example programs")
	set(YAML_CPP_BUILD_TESTS    OFF CACHE INTERNAL "Build the yaml-cpp test programs")
	set(YAML_CPP_BUILD_DOCS     OFF CACHE INTERNAL "Build the yaml-cpp documentation")
	set(YAML_CPP_INSTALL        OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory("${YAML_CPP_DIR}")

	set(YAML_CPP_LIBRARY "yaml-cpp" "${YAML_CPP_LIBRARIES}")
	set(YAML_CPP_INCLUDE_DIR "${YAML_CPP_DIR}/include")
endif()