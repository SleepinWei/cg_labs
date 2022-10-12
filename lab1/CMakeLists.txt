cmake_minimum_required(VERSION 3.0.0)
project(demo)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ./)

add_executable(circle main.cpp utils/glad.c utils/Shader.cpp include/buffer_screen.hpp include/point.hpp include/algorithm.hpp)

target_include_directories(circle PUBLIC ${PROJECT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/cmake)

target_link_libraries(circle ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} -ldl -lm -lXinerama -lXrandr -lXi -lXcursor -lX11 -lpthread)

