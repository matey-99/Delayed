# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_EDITOR_LIBRARY ${OPENGL_LIBRARIES})

# imgui
set(IMGUI_DIR "${EDITOR_THIRDPARTY_DIR}/imgui")
execute_process(COMMAND git submodule update --init ${IMGUI_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
					
add_library("imgui" STATIC "${IMGUI_DIR}/imgui.cpp"
					"${IMGUI_DIR}/imgui_demo.cpp"
					"${IMGUI_DIR}/imgui_draw.cpp"
					"${IMGUI_DIR}/imgui_widgets.cpp"
					"${IMGUI_DIR}/imgui_tables.cpp")
target_include_directories("imgui" PRIVATE "${IMGUI_DIR}")

set(IMGUI_LIBRARY "imgui")
set(IMGUI_INCLUDE_DIR "${IMGUI_DIR}")

# ImGuizmo
set(IMGUIZMO_DIR "${EDITOR_THIRDPARTY_DIR}/ImGuizmo")
execute_process(COMMAND git submodule update --init ${IMGUIZMO_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
					
add_library("ImGuizmo" STATIC "${IMGUIZMO_DIR}/ImGuizmo.cpp")
target_include_directories("ImGuizmo" PRIVATE "${IMGUIZMO_DIR}" "${IMGUI_DIR}")

set(IMGUIZMO_LIBRARY "ImGuizmo")
set(IMGUIZMO_INCLUDE_DIR "${IMGUIZMO_DIR}")