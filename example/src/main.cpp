//
// Created by bjk on 19. 8. 2..
//

#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <GLEngine/Camera.hpp>
#include <GLEngine/RenderObject.hpp>

class HelloCubeMaterial : public GLEngine::Material {
public:
    explicit HelloCubeMaterial() : GLEngine::Material() {
        std::ostringstream vertex, fragment;
        vertex << "#version 330 core\n"
        << "layout(location = 0) in vec3 pos;\n"
        << "layout(location = 1) in vec3 color;\n"
        << "uniform mat4 worldTransform; uniform mat4 cameraTransform; uniform mat4 projectionMatrix;\n"
        << "out vec3 fragColor;\n"
        << "void main() { "
        << "gl_Position = projectionMatrix * inverse(cameraTransform) * worldTransform * vec4(pos, 1);\n"
        << "fragColor = color; }";

        fragment << "#version 330 core\n"
        << "in vec3 fragColor;\n"
        << "out vec4 outputColor;\n"
        << "void main() { "
        << "outputColor = vec4(pow(fragColor, vec3(1/2.2)), 1);"
        << "}";

        m_program->attachShader(vertex.str(), GL_VERTEX_SHADER);
        m_program->attachShader(fragment.str(), GL_FRAGMENT_SHADER);
        m_program->linkShader();
    }
};

class HelloCubeApplication {
private:
    GLFWwindow* m_window = nullptr;
    float g_window_width = 1024.f;
    float g_window_height = 768.f;
    int g_framebuffer_width = 1024;
    int g_framebuffer_height = 768;

    std::shared_ptr<GLEngine::Camera> m_camera;
    std::shared_ptr<GLEngine::Mesh> m_cubeMesh;
    std::shared_ptr<HelloCubeMaterial> m_cubeMaterial;
    std::shared_ptr<GLEngine::Transform> m_parentTransform;
    std::shared_ptr<GLEngine::RenderObject> m_cube;

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(g_window_width, g_window_height, "HelloCube", nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
    }

    void initGL() {
        glewInit();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        //glEnable(GL_TEXTURE_2D);
        //glEnable(GL_TEXTURE_CUBE_MAP);
    }

    void initScene() {
        m_camera = std::make_shared<GLEngine::Camera>();
        m_camera->getTransform()->setPosition(glm::vec3(0.f, 0.f, 6.f));
        m_camera->setProjection(glm::perspective(glm::radians(60.f),
                4.f/3.f, 0.01f, 1000.f));

        m_cubeMesh = std::make_shared<GLEngine::Mesh>();
        m_cubeMesh->addAttribute(3); // position
        m_cubeMesh->addAttribute(3); // color
        m_cubeMesh->setHasIndex(true);

        std::vector<glm::vec3> pos, color;
        std::vector<unsigned> indices;
        pos = {glm::vec3(-1.f, -1.f, 1.f),
               glm::vec3(1.f, -1.f, 1.f),
               glm::vec3(1.f, 1.f, 1.f),
               glm::vec3(-1.f, 1.f, 1.f),
               glm::vec3(-1.f, -1.f, -1.f),
               glm::vec3(1.f, -1.f, -1.f),
               glm::vec3(1.f, 1.f, -1.f),
               glm::vec3(-1.f, 1.f, -1.f)};
        color = {glm::vec3(1.f, 0.f, 0.f),
                 glm::vec3(0.f, 1.f, 0.f),
                 glm::vec3(0.f, 0.f, 1.f),
                 glm::vec3(0.5f, 0.5f, 0.f),
                 glm::vec3(0.f, 0.5f, 0.5f),
                 glm::vec3(0.5f, 0.f, 0.5f),
                 glm::vec3(0.f, 0.f, 0.f),
                 glm::vec3(0.3f, 0.3f, 0.3f)};
        indices = {0, 1, 3, 1, 2, 3,
                   0, 3, 4, 4, 3, 7,
                   1, 5, 2, 5, 6, 2,
                   3, 6, 7, 3, 2, 6,
                   0, 4, 5, 0, 5, 1,
                   4, 7, 6, 4, 6, 5};

        for(int i = 0; i < 8; ++i){
            m_cubeMesh->addVertexData(pos[i]);
            m_cubeMesh->addVertexData(color[i]);
        }
        for(auto i : indices) {
            m_cubeMesh->addIndexData(i);
        }
        m_cubeMesh->setNumElements(indices.size());
        m_cubeMesh->createMesh();

        m_cubeMaterial = std::make_shared<HelloCubeMaterial>();

        m_cube = std::make_shared<GLEngine::RenderObject>(m_cubeMesh, m_cubeMaterial);
        m_parentTransform = std::make_shared<GLEngine::Transform>();
        m_parentTransform->setOrientation(glm::rotate(glm::mat4(1.f), glm::radians(30.f), glm::vec3(1.f, 0.f, 0.f)));
        m_cube->getTransform()->setParent(m_parentTransform);
    }

    void mainLoop() {
        auto prev_time = static_cast<float>(glfwGetTime());
        while(!glfwWindowShouldClose(m_window)) {
            auto total_time = static_cast<float>(glfwGetTime());
            auto elapsed_time = total_time - prev_time;
            prev_time = total_time;

            glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_cube->getTransform()->setOrientation(glm::rotate(glm::mat4(1.f), total_time, glm::vec3(0.f, 0.f, 1.f)));
            m_cube->render(*m_camera);

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

    void cleanup() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

public:
    void run() {
        initWindow();
        initGL();
        initScene();
        mainLoop();
        cleanup();
    }
};

int main(int argc, char* argv[]) {
    HelloCubeApplication app;

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what();
        return 1;
    }

    return 0;
}