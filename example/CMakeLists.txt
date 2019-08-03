project(openGL_Engine_example)

#find_package(Assimp REQUIRED)
#find_package(SOIL CONFIG REQUIRED)

set(APP_SRC src/main.cpp)

add_executable(App ${APP_SRC} ${APP_HEADERS})

target_include_directories(App PUBLIC include PRIVATE src)
target_link_libraries(App GLEngine glfw GLEW::GLEW ${OPENGL_LIBRARY} glm)