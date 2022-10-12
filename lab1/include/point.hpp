#ifndef POINT_HPP
#define POINT_HPP

struct Point {
    int x, y; // pixel position on screen

    Point operator+(const Point &p){
        return {p.x + x, p.y + y};
    }

    Point operator-(const Point &p){
        return {x-p.x,y-p.y};
    }
};





#endif