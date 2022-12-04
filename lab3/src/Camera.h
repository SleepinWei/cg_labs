#pragma once
#include"aliase.h"

class Ray;

class Camera {
    public:
        Camera(
            vec3 lookfrom,
            vec3 lookat,
            vec3 up,
            float fov, //degree
            int width, int height
        );
        Ray get_ray(float u, float v)const;

    public:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;

        float aspect_ratio;
        float viewport_height;
        float viewport_width;
        float focal_length;

        int width, height;
};
