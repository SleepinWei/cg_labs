#define _CRT_SECURE_NO_WARNINGS
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<memory>
#include"./shader/Shader.h"

// Util functions 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button(GLFWwindow* window, int, int, int);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);
int createWindow(GLFWwindow*& window, int width, int height, std::string title = "Demo");
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int gladInit();

int g_xpos, g_ypos; 

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

struct Point {
    int x, y; // pixel position on screen

    Point operator+(Point p){
        return {p.x + x, p.y + y};
    }

    Point operator-(Point p){
        return {x-p.x,y-p.y};
    }
};

class Screen{
public:
    std::vector<float> vertices;
    GLuint VAO;
    GLuint VBO;
    bool dirty; 

    Screen(){
        dirty = true; 
        VAO = 0;
        VBO = 0;
    }

    void draw(){
        if(dirty){
            if(VAO){
                glDeleteVertexArrays(1,&VAO);
                glDeleteBuffers(1,&VBO);
            }
            glGenVertexArrays(1,&VAO);
            glGenBuffers(1,&VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),&vertices[0],GL_STATIC_DRAW);
            glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
            glEnableVertexAttribArray(0);
            dirty = false;
        }
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS,0,vertices.size()/2);
    }

    void reserve(int size){
        if(vertices.size() + size >= vertices.capacity()){
            vertices.reserve(size + vertices.size());
        }
    }

    void putPixel(const Point& p){
        float x = p.x * 2.0 / SCR_WIDTH - 1;
        float y = -p.y * 2.0 / SCR_HEIGHT + 1;
        vertices.push_back(x);
        vertices.push_back(y);
        dirty = true;
    }
};

// screen
Screen screen; 

void drawCircle(Point c,Point p_){
    double r = sqrt((p_.x-c.x)*(p_.x-c.x) + (p_.y-c.y)*(p_.y-c.y));
    std::cout << "CreateCircle"  << " Center: " << c.x << ' ' << c.y << " Radius: " << r << '\n';
    int maxPointNum = int(ceil(r) * ceil(r) + 10);
    screen.reserve(maxPointNum);
    auto begTime = glfwGetTime();

    // TODO:
    // c: origin of circle 
    // r: radius 
    // to draw a pixel at point (x,y) on scrren, use "screen->putPixel({x,y});"

    Point p = {0,int(r)};
    float d = 1.0f - r; 
    screen.putPixel(c + p);
    screen.putPixel(c+Point{p.x,-p.y});
    screen.putPixel(c+Point{p.y,p.x});
    screen.putPixel(c+Point{-p.y,p.x});

    while(p.x <= p.y){
        p.x += 1; 
        if(d < 0){
            p.y = p.y; 
            d += 2 * p.x + 1; 
        }
        else{
            p.y = p.y -1; 
            d += 2 * p.x + 1 - 2 * p.y; 
        }
        screen.putPixel(c + Point{p.x,p.y});
        screen.putPixel(c + Point{p.x,-p.y});
        screen.putPixel(c + Point{-p.x,-p.y});
        screen.putPixel(c + Point{-p.x,p.y});
        screen.putPixel(c + Point{p.y,p.x});
        screen.putPixel(c + Point{p.y,-p.x});
        screen.putPixel(c + Point{-p.y,-p.x});
        screen.putPixel(c + Point{-p.y,p.x});
    }

    // END of TODO
    std::cout << (glfwGetTime() - begTime) * 1000 * 1000 << "us\n"; 
}

void mainloop() {
    glfwInit();
    GLFWwindow* window; 
    createWindow(window, SCR_WIDTH, SCR_HEIGHT);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
    //glad
    gladInit();

    Shader shader("./shader/shader.vs", "./shader/shader.fs");
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
