#define _CRT_SECURE_NO_WARNINGS
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<memory>
#include "Shader.h"
#include "algorithm.hpp"

// Util functions that do not matter
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button(GLFWwindow* window, int, int, int);
void processInput(GLFWwindow* window);
int createWindow(GLFWwindow*& window, int width, int height, std::string title = "Demo");
int gladInit();

extern Screen screen;


void mainloop() {
    glfwInit();
    GLFWwindow* window; 
    createWindow(window, SCR_WIDTH, SCR_HEIGHT);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
    //glad
    gladInit();

    Shader shader("shader.vs", "shader.fs");
    shader.use();
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.6,.2,.0,1.0);
        processInput(window);
        shader.use();
        screen.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    mainloop();
}

int gladInit() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }
    return 0;
}

int createWindow(GLFWwindow*& window,
    int width,
    int height,
    std::string title) {
    //create a glfw window 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button);
    //glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return 0;
}

void drawCircle(Point c,Point p_){
    double r = sqrt((p_.x-c.x)*(p_.x-c.x) + (p_.y-c.y)*(p_.y-c.y));
    std::cout << "Create Circle: "<< " Center: " << c.x << ' ' << c.y << " Radius: " << r << '\n';
    std::shared_ptr<Algorithm> dda(new DDA);
    std::shared_ptr<Algorithm> bresham(new Bresham);

    dda->exec_algorithm(c, r);
    bresham->exec_algorithm(c, r);
}

void processInput(GLFWwindow* window)
{
    static bool enableCursor = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_button(GLFWwindow* window, int button, int action,int mods){
    static int click_cnt = 0; 
    static Point c; 
    static Point p; 
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        click_cnt += 1; 
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // std::cout << "Click Count: " << click_cnt << '\n';
        if(click_cnt == 1){
            c = {int(xpos),int(ypos)};
        }
        if(click_cnt == 2){
            p = {int(xpos),int(ypos)};
            drawCircle(c,p);
            click_cnt = 0;
        }
    }
}
