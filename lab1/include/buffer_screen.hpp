#ifndef BUFFER_SCREEN
#define BUFFER_SCREEN
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



static const int SCR_WIDTH = 800;
static const int SCR_HEIGHT = 600;

class Buffer;
class Screen{
public:
    std::vector<float> vertices;
    GLuint VAO;
    GLuint VBO;
    bool dirty; 

    Screen(){
        dirty = false; 
        VAO = 0;
        VBO = 0;
        vertices.clear();
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
            glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
            glEnableVertexAttribArray(0);
            dirty = false;
        }
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS,0,vertices.size()/2);
    }

    void append(Buffer& buff);
};

class Buffer{
public:
    Buffer(int size){
        std::vector<int>(size).swap(buffer);
        cnt = 0;
    }
    inline void putPixel(const int x, const int y){
        if(cnt + 2 > buffer.size()){
            std::cout <<"Too many pixels!"<<'\n';
            return;
        }

        *(buffer.begin() + cnt++) = x;
        *(buffer.begin() + cnt++) = y;
    }
public:
    std::vector<int> buffer;
    int cnt; 
};


//admit the other files use (aims at main.cpp)
Screen screen;  

void Screen::append(Buffer& buff){
    for(auto it = buff.buffer.begin(); it != buff.buffer.end(); it += 2){
        float x = (*it) * 2.0 / SCR_WIDTH - 1;
        float y = - *(it + 1) * 2.0 / SCR_HEIGHT + 1;
        vertices.emplace_back(x);
        vertices.emplace_back(y);
    }
    dirty = true;
}



#endif