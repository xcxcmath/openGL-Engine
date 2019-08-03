cmake_minimum_required(VERSION 3.14)
project(openGL_Engine)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(GLEngine)
add_subdirectory(example)