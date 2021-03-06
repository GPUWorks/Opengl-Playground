cmake_minimum_required(VERSION 3.6)
project(OpenGL_Playground)

#######################################################
# directories validation
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build directory!(And give it a clever name, like VisualStudio2015_64bits)")
endif()

if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your source directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif()

if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your build directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif()

#########################################################
# Project configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#########################################################
# Output information
message(STATUS "Root folder: ${PROJECT_SOURCE_DIR}")

#########################################################
# Demo projects
add_subdirectory(1-Startup)
add_subdirectory(2-SphereTexture)
add_subdirectory(3-Tessellation-Triangle)
add_subdirectory(4-UpdateTexture)
add_subdirectory(5-PlaneTexture)
add_subdirectory(6-PlaneProjection)
add_subdirectory(7-YUVShader)