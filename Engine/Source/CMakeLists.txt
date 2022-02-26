# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	 *.c
	 *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	 *.h
	 *.hpp)

#EXECUTABLE

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Define the include DIRs
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC "${ASSIMP_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${YAML_CPP_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${IMGUIZMO_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${ASSIMP_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${YAML_CPP_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLAD_LIBRARY}"      "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${IMGUI_LIBRARY}"     "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${IMGUIZMO_LIBRARY}"  "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${STB_IMAGE_LIBRARY}" "${CMAKE_DL_LIBS}")

target_precompile_headers(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE YAML_CPP_STATIC_DEFINE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

#LIBRARY

add_library(${PROJECT_NAME}_Library STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME}_Library PROPERTY CXX_STANDARD 17)

# Define the include DIRs
target_include_directories(${PROJECT_NAME}_Library PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${ASSIMP_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${YAML_CPP_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME}_Library PUBLIC "${IMGUIZMO_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME}_Library "${OPENGL_LIBRARY}")
target_link_libraries(${PROJECT_NAME}_Library "${ASSIMP_LIBRARY}")
target_link_libraries(${PROJECT_NAME}_Library "${GLFW_LIBRARY}")
target_link_libraries(${PROJECT_NAME}_Library "${YAML_CPP_LIBRARY}")
target_link_libraries(${PROJECT_NAME}_Library "${GLAD_LIBRARY}"      "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME}_Library "${IMGUI_LIBRARY}"     "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME}_Library "${IMGUIZMO_LIBRARY}"  "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME}_Library "${STB_IMAGE_LIBRARY}" "${CMAKE_DL_LIBS}")

target_precompile_headers(${PROJECT_NAME}_Library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_compile_definitions(${PROJECT_NAME}_Library PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME}_Library PRIVATE YAML_CPP_STATIC_DEFINE)
target_compile_definitions(${PROJECT_NAME}_Library PRIVATE LIBRARY_SUFFIX="")

#OTHER

add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
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
				source_group(${groupname} FILES ${CMAKE_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()
	
	# Run macro
	GroupSources(Engine/Source)
endif()