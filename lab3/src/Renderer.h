#pragma once
#include<string>
#include"aliase.h"
class Ray;
class Scene;
using std::string;

class Renderer {
public:
	Renderer(int width, int height);
	Renderer();
	~Renderer();
	// function
	color trace(const Ray& ray,const Scene& scene); // trace ray
	void render(const Scene& scene); // scene rendering
	void writePic(const string& filename); // write to ppm picture
public:
	// variables
	int width, height;
	unsigned char* image; // result pic
private:
	void write_color(const color& c, int pos); // srgb conversion
};
