#define _CRT_SECURE_NO_WARNINGS
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<memory>
#include"./shader/Shader.h"

// Util functions that do not matter
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button(GLFWwindow* window, int, int, int);
void processInput(GLFWwindow* window);
int createWindow(GLFWwindow*& window, int width, int height, std::string title = "Demo");
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

class Buffer;
class Screen{
public:
    std::vector<float> vertices;
    GLuint VAO;
    GLuint VBO;
    bool dirty; 

    Screen();

    void draw();

    void append(Buffer& buffer);
};

class Buffer{
public:
    Buffer(int size);
    void putPixel(int x,int y);
public:
    std::vector<int> buffer;
    int cnt; 
};

// screen
Screen screen; 

void func1(Point c,double r){
    Buffer buff(12*int(ceil(r))+64); 
    int t = 10000;
    auto begTime = glfwGetTime();

    while(t--){
        buff.cnt = 0;
    //TODO: implement MID POINT algorithm to draw a circle here. 
    // c: center of circle
    // r: radius of circle 
    // to put a white pixel on position (x,y) on screen, call "buff.putPixel(x,y);"
    // here x,y are all integers. 
    int cx = c.x; int cy = c.y;
    int x = 0,y = int(r);
    
    int d = 1 - (int)r; 
    buff.putPixel(cx,cy+y);
    buff.putPixel(cx,cy-y);
    buff.putPixel(cx + y,cy);
    buff.putPixel(cx - y,cy);

    while(x <= y){
        x += 1; 
        if(d < 0){
            d += 2 * x + 1; 
        }
        else{
            y = y -1; 
            d += 2 * x + 1 - 2 * y; 
        }
        buff.putPixel(cx + x,cy + y);
        buff.putPixel(cx + x, cy - y);
        buff.putPixel(cx - x, cy -y);
        buff.putPixel(cx -x,cy + y);
        buff.putPixel(cx + y,cy + x);
        buff.putPixel(cx + y,cy-x);
        buff.putPixel(cx  -y,cy-x);
        buff.putPixel(cx  -y,cy + x);
    }

    // END of TODO
    }
    std::cout <<"func1: "<< (glfwGetTime() - begTime) * 1000 << "ms\n"; 
    screen.append(buff);
}

void func2(Point c,double r){
    Buffer buff(12*int(ceil(r))+64); 
    int t = 10000; 
    auto begTime = glfwGetTime();

    while(t--){
        buff.cnt=0;
    //TODO: implement some naive algorithm to draw a circle here 
    // c: center of circle 
    // r: radius of circle
    // to put a white pixel on position (x,y) on screen, call "buff.putPixel(x,y);"
    // here x,y are all integers. 

    int x = 0;
    int y = (int)r; 
    int cx = c.x;
    int cy = c.y;
    buff.putPixel(cx,cy+y);
    buff.putPixel(cx,cy-y);
    buff.putPixel(cx + y,cy);
    buff.putPixel(cx - y,cy);

    while(x <=y){
        x += 1; 
        y = int(sqrt(r * r - (x) * (x)));
        buff.putPixel(cx + x,cy + y);
        buff.putPixel(cx + x, cy - y);
        buff.putPixel(cx - x, cy -y);
        buff.putPixel(cx -x,cy + y);
        buff.putPixel(cx + y,cy + x);
        buff.putPixel(cx + y,cy-x);
        buff.putPixel(cx  -y,cy-x);
        buff.putPixel(cx  -y,cy + x);
    }

    // END of TODO
    }
    std::cout <<"func2: "<< (glfwGetTime() - begTime) * 1000 << "ms\n"; 
    screen.append(buff);
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

void drawCircle(Point c,Point p_){
    double r = sqrt((p_.x-c.x)*(p_.x-c.x) + (p_.y-c.y)*(p_.y-c.y));
    std::cout << "Create Cicle: "<< " Center: " << c.x << ' ' << c.y << " Radius: " << r << '\n';

    func1(c,r); 
    func2({c.x+25,c.y},r); 
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

void Screen::draw(){
    if(dirty){
        if(VAO){
            glDeleteVertexArrays(1,&VAO);
            glDeleteBuffers(1,&VBO);
        }
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        dirty = false;
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS,0,vertices.size()/2);
}
Screen::Screen(){
    dirty = false; 
    VAO = 0;
    VBO = 0;
}
void Screen::append(Buffer& buff){
    for(int i =0 ;i<buff.cnt/2;i++){
        float x = buff.buffer[2*i] * 2.0 / SCR_WIDTH - 1;
        float y = -buff.buffer[2*i+1] * 2.0 / SCR_HEIGHT + 1;
        vertices.emplace_back(x);
        vertices.emplace_back(y);
    }
    dirty = true;
}
void Buffer::putPixel(int x,int y){
    if(cnt + 2 > buffer.size()){
        std::cout <<"Too many pixels!"<<'\n';
        return;
    }
    buffer[cnt++] = x; 
    buffer[cnt++] = y; 
}
Buffer::Buffer(int size){
    std::vector<int>(size).swap(buffer);
    cnt = 0;
}   