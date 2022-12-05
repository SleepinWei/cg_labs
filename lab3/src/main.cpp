#define _CRT_SECURE_NO_WARNINGS
#include"aliase.h"
#include"Renderer.h"
#include"Camera.h"
#include"hittable.h"
#include"Material.h"
#include"Scene.h"

int main() {
	const int SCR_WIDTH = 1200;
	const int SCR_HEIGHT = 1200;
	Renderer renderer;
	auto scene = std::make_shared<Scene>();
	{
		auto camera = std::make_shared<Camera>(
			vec3(0.0f, 2.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f,
			SCR_WIDTH, SCR_HEIGHT);
		scene->camera = camera;
	}
	{
		auto white = std::make_shared<Material>(MatType::DIFFUSE,color(1.0f),1.0f);
		auto red = std::make_shared<Material>(MatType::DIFFUSE, color(1.0f, 0.0f, 0.0f), 1.0f);
		auto green = std::make_shared<Material>(MatType::DIFFUSE, color(0.0f, 1.0f, 0.0f), 1.0f);
		auto glass = std::make_shared<Material>(MatType::GLASS, color(1.0f, 1.0f, 1.0f), 1.5f);
		auto c_ground = std::make_shared<Material>(MatType::DIFFUSE, color(140.0f, 255.0f, 251.0f) / 255.0f, 1.5f);

		auto light =  std::make_shared<Material>(MatType::LIGHT, color(1.0f,1.0f, 1.0f), 1.5f);

		auto s1 = std::make_shared<Sphere>(vec3(0.0f,-500.0f,0.0f), 500.0f, c_ground);
		scene->add(s1);
		auto s2 = std::make_shared<Sphere>(vec3(0.0f,1.0f,0.0f), 1.0f, glass);
		scene->add(s2);
		auto s4 = std::make_shared<Sphere>(vec3(2.0f, 1.0f, 0.0f), 1.0f, red);
		scene->add(s4);
		auto s3 = std::make_shared<Sphere>(vec3(0.0f, 4.0f, 0.0f), 0.5f, light);
		scene->addLight(s3);

		auto s5 = std::make_shared<Sphere>(vec3(-2.0f, 1.0f, 0.0f), 1.0f, green);
		scene->add(s5);
	}
	renderer.render(scene);
	renderer.writePic("./out.ppm", scene);
}