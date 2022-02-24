# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	 *.c
	 *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	 *.h
	 *.hpp)

set(EDITOR_PROJECT_NAME ${PROJECT_NAME}_Editor)

# Define the executable
add_executable(${EDITOR_PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
set_property(TARGET ${EDITOR_PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Define the include DIRs
target_include_directories(${EDITOR_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${ASSIMP_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${YAML_CPP_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${IMGUIZMO_INCLUDE_DIR}")
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine/src")

target_link_libraries(${EDITOR_PROJECT_NAME} "${PROJECT_NAME}_Library")
target_link_libraries(${EDITOR_PROJECT_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${ASSIMP_LIBRARY}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${YAML_CPP_LIBRARY}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${GLAD_LIBRARY}"      "${CMAKE_DL_LIBS}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${IMGUI_LIBRARY}"     "${CMAKE_DL_LIBS}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${IMGUIZMO_LIBRARY}"  "${CMAKE_DL_LIBS}")
target_link_libraries(${EDITOR_PROJECT_NAME} "${STB_IMAGE_LIBRARY}" "${CMAKE_DL_LIBS}")

target_precompile_headers(${EDITOR_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE YAML_CPP_STATIC_DEFINE)
target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

add_custom_command(TARGET  ${EDITOR_PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
						   ${CMAKE_SOURCE_DIR}/Assets
						   ${CMAKE_CURRENT_BINARY_DIR}/Assets)

# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/${curdir} ${CMAKE_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "sources" groupname ${groupname})
				source_group(${groupname} FILES ${CMAKE_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()
	
	# Run macro
	GroupSources(src)
endif()