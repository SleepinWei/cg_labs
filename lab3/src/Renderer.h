#pragma once
#include<string>
#include<memory>
#include"aliase.h"
class Ray;
class Scene;
using std::string;
using std::shared_ptr;

class Renderer {
public:
	Renderer();
	~Renderer();
	// function
	color trace(const Ray& ray,const shared_ptr<Scene>& scene,int depth); // trace ray
	void render(const shared_ptr<Scene>& scene); // scene rendering
	void writePic(const string& filename,const shared_ptr<Scene>& scene); // write to ppm picture
public:
	// variables
	int width, height;
	int* image; // result pic
	const color background = color(180.0f, 255.0f,255.0f)/255.0f;
};
