project(openGL_Engine_example)

add_executable(HelloCube src/main.cpp)

target_include_directories(HelloCube PUBLIC HelloCube/include PRIVATE src)
target_link_libraries(HelloCube GLEngine glfw GLEW::GLEW ${OPENGL_LIBRARY} glm)