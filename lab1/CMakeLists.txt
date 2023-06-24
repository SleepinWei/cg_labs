cmake_minimum_required(VERSION 3.0.0)
project(demo)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(SRC_LIST main.cpp utils/Shader.cpp include/buffer_screen.hpp include/point.hpp include/algorithm.hpp external/glad/glad.c)

# set(LINK_LIBRARIES -ldl -lm -lXinerama -lXrandr -lXi -lXcursor -lX11 -lpthread)

add_executable(circle ${SRC_LIST})

target_include_directories(circle PUBLIC ${PROJECT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/external)

target_link_libraries(circle ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} -ldl)

