#ifndef ALG
#define ALG
#include "buffer_screen.hpp"
#include "point.hpp"
#include <math.h>


class Algorithm{
   public:
   virtual void exec_algorithm(const Point & c, double r)=0; 
};

class DDA: public Algorithm{
    public:
    virtual void exec_algorithm(const Point &c, double r) override final{
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
        std::cout <<"func1 dda: "<< (glfwGetTime() - begTime) * 1000 << "ms\n"; 
        Screen::GetInstance()->append(buff);
    }
};

class Bresham: public Algorithm{
    public:
    virtual void exec_algorithm(const Point & c, double r) override final{
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
        std::cout <<"func2 bresham: "<< (glfwGetTime() - begTime) * 1000 << "ms\n"; 
        Screen::GetInstance()->append(buff);
    }
};

#endif