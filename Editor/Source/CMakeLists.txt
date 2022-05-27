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
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine/Source")

target_link_libraries(${EDITOR_PROJECT_NAME} "${PROJECT_NAME}_Library")

target_precompile_headers(${EDITOR_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE YAML_CPP_STATIC_DEFINE)
target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

#OTHER
add_custom_command(TARGET ${EDITOR_PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   		   ${FMOD_DLL}
						   $<TARGET_FILE_DIR:${EDITOR_PROJECT_NAME}>)

add_custom_command(TARGET ${EDITOR_PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   		   ${FREETYPE_DLL}
						   $<TARGET_FILE_DIR:${EDITOR_PROJECT_NAME}>)

# Copy .exe to .. directory
add_custom_command(TARGET ${EDITOR_PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   		   "$<TARGET_FILE:${EDITOR_PROJECT_NAME}>"
						   "${CMAKE_BINARY_DIR}/Editor/Source")

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
				source_group(${groupname} FILES ${CMAKE_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()
	
	# Run macro
	GroupSources(Editor/Source)
endif()