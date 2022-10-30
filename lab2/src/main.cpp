#define _CRT_SECURE_NO_WARNINGS
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<memory>
#include<iostream>
#include"utils/Shader.h"
#include"utils/Utils.h"
#include"utils/Camera.h"
#include"utils/global.h"
#include"manager/InputManager.h"
#include"geometry/Model.h"

extern std::shared_ptr<Camera> camera;
extern std::shared_ptr<InputManager> inputManager;
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

int main() {
    glfwInit();
    GLFWwindow* window;
    createWindow(window, SCR_WIDTH, SCR_HEIGHT);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad
    gladInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    {
        camera = std::make_shared<Camera>();
        inputManager = std::make_shared<InputManager>();
    }
    Shader shader("shader/shader.vs", "shader/shader.fs");
    Shader depthShader("shader/shader.vs", "shader/depth.fs");

    Model myModel;
    myModel.loadFromFile("./asset/backpack.obj");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        inputManager->tick();

		camera->tick();
        {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glm::mat4 projection = camera->GetPerspective();
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -.2f));
            model = glm::scale(model, glm::vec3(0.1f));
            shader.use();
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            shader.setFloat("delta", 0.0f);
            shader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            myModel.render(shader);

            shader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
            shader.setFloat("delta", 0.001f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            myModel.render(shader);
        }

        inputManager->reset();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}