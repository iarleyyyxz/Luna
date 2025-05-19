cmake_minimum_required(VERSION 3.1)
project(Luna)

# Adicione o ImGui como submódulo (se ainda não o fez)
# Certifique-se de que você clonou o ImGui com --recurse-submodules
add_subdirectory(imgui)
# Opções para habilitar docking e viewports
option(IMGUI_ENABLE_DOCKING "Enable ImGui docking" ON)
option(IMGUI_ENABLE_VIEWPORTS "Enable ImGui viewports" ON)

if(IMGUI_ENABLE_DOCKING)
    add_definitions(-DIMGUI_BUILD_DOCKING_BRANCH)
endif()

if(IMGUI_ENABLE_VIEWPORTS)
    add_definitions(-DIMGUI_ENABLE_VIEWPORTS) # Flag necessária para viewports
endif()

# Find GLFW
find_package(GLFW 3.3 REQUIRED)
if(NOT GLFW_FOUND)
    message(FATAL_ERROR "GLFW not found.  Please install GLFW.")
endif()

# Find GLEW
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found.  Please install GLEW.")
endif()
include_directories(${GLEW_INCLUDE_DIRS})


# Include directories
include_directories(imgui ${GLFW_INCLUDE_DIR})


add_executable(my_app main.cpp)
target_link_libraries(my_app imgui glfw glew)

# Add this to link the necessary libraries
target_link_libraries(my_app
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
)
