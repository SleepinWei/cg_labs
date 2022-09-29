cmake_minimum_required(VERSION 3.0.0)
project(demo VERSION 0.1.0)

find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ../.)
set(SRC_LIST main.cpp glad.c ./shader/Shader.h ./shader/Shader.cpp)

add_executable(circle ${SRC_LIST})

target_include_directories(circle PUBLIC include)
include_directories("./lib")
target_link_libraries(circle libglfw3.a ${OPENGL_LIBRARIES} 
	-ldl -lm -lXinerama -lXrandr -lXi -lXcursor -lX11 -lpthread)
