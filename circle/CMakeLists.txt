cmake_minimum_required(VERSION 3.0.0)
project(demo)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

if(OpenGL_FOUND AND glfw3_FOUND)

	set(EXECUTABLE_OUTPUT_PATH ../.)
	set(SRC_LIST main.cpp glad.c ./shader/Shader.h ./shader/Shader.cpp)

	add_executable(circle ${SRC_LIST})

	target_include_directories(demo PUBLIC ${PROJECT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/cmake)

	target_link_libraries(demo ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} -ldl -lm -lXinerama -lXrandr -lXi -lXcursor -lX11 -lpthread)

endif()
